#include "PC.h"

#include "../DHCPServer/DHCPPayload.h"
#include "../EventCoordinator/EventCoordinator.h"
#include "../IP/IPv4.h"
#include "../utils/ConfigParser.h"
#include "src/main/DataGenerator/ChunkServer.h"
#include "src/main/DataGenerator/DataPayload.h"

#include <algorithm>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRandomGenerator>

PC::PC(int id, QString network, QObject* parent) :
    Node(id, network, parent), totalPacketsSent(0), totalPacketsReceived(0)
{
    this->ipDatabase = nullptr;
    this->time       = 0;
    QObject::connect(this, &PC::packetReceived, this, &PC::onPacketReceived);

    PC::connect(EventCoordinator::instance(), &EventCoordinator::nextTick, this, &PC::onNextTick);

    tcp.setSendPacketToRemoteIP([this](PacketPtr packet) { sendPacket(packet); });
    connect(&tcp, &TCPProtocol::dataReceived, this, &PC::handleDeliveredDataFromTCP);
}

void
PC::addPort(PortPtr port)
{
    port->moveToThread(thread());
    QObject::connect(port.get(), &Port::packetReceived, this, &PC::packetReceived);
    this->port = port;
}

void
PC::onPacketReceived(const PacketPtr packet)
{
    if(packet == nullptr)
    {
        return;
    }


    DHCPPayloadPtr dhcpPayload;

    switch(packet->getType())
    {
        case UT::PacketType::Data :
            dataPacketReceived(packet);
            break;

        case UT::PacketType::Control :
            switch(packet->getControlType())
            {
                case UT::PacketControlType::DHCPDiscovery :
                    break;

                case UT::PacketControlType::DHCPOffer :
                    dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();

                    if(dhcpPayload->getId() == id)
                        requestDHCP(dhcpPayload->getIP(), dhcpPayload->getMACAddress());

                    break;

                case UT::PacketControlType::DHCPRequest :
                    break;

                case UT::PacketControlType::DHCPAcknowledge :
                    dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();

                    if(dhcpPayload->getId() == id)
                    {
                        setIP(dhcpPayload->getIP());
                        setMACAddress(dhcpPayload->getMACAddress());
                        tcp.setOwnerIP(getIP());
                    }

                    break;

                default :
                    break;
            }

            break;
    }
}

void
PC::handleDeliveredDataFromTCP(const QString& from, const QByteArray& data)
{
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Handle the delivered data from TCP here.
     * @attention Handle Sequence Numbers.
     * @attention When All Packets are received, flush them on a file and terminate the application.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     **/

    buffer.append(data);

    qsizetype TOTAL_PACKET_COUNT = 22'000;

    if(buffer.size() == TOTAL_PACKET_COUNT)
    {
        return flushBufferToFile();
    }
}

void
PC::flushBufferToFile()
{
    QString fileDiretory = "../../ReceivedMusic";
    QString fileName     = "received_music.mp3";
    QString filePath     = fileDiretory + "_received_music.mp3";

    QDir    directory;

    if(!directory.exists(fileDiretory)) directory.mkdir(fileDiretory);

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        UT_WARNING "Couldn't open file for writing.";
        return;
    }

    for(const auto& packet : buffer)
    {
        file.write(packet);
    }

    file.close();
}

void
PC::dataPacketReceived(PacketPtr packet)
{
    if(!packet) return;

    totalPacketsReceived++;
    packet->setReceivedTime(time);
    hopCounts.append(packet->getHopCount());
    waitingCycles.append(packet->getWaitingCycles());

    if(!ConfigParser::instance()->useChunkServer()) return;

    if(!ConfigParser::instance()->getReceiverPCsIDs().contains(id))
    {
        UT_WARNING "PC" << id << "received packet not intended for it. Dropping.";
        return;
    }

    if(!getIP()->equals(packet->getIPv4HeaderPtr()->destIP))
    {
        UT_WARNING "PC" << id << "received packet not intended for it. Dropping.";
        return;
    }

    if(ConfigParser::instance()->getReceiverPCsIDs().contains(id))
    {
        static size_t packetCounter = 0;
        packetCounter++;
        UT_INFO "PC" << id << "received" << packetCounter << "th packet.";

        // remove this return
        return;
    }

    if(!tcp.ownerIP())
    {
        tcp.setOwnerIP(getIP());
    }


    UT_INFO "PC" << id << "ip:" << getIP()->toString()
                 << "received packet with payload size:" << packet->getPayload();

    tcp.handleReceivedPacket(packet);
}

void
PC::sendPacketFromDataGenerator(const int packetCount)
{
    if(packetCount == 0) return;

    QList<int> pcIds = ipDatabase->pcsId();

    pcIds.removeAll(id);

    std::shuffle(pcIds.begin(), pcIds.end(), *QRandomGenerator::global());

    QList<int> randomIds = pcIds.mid(0, packetCount);

    for(int destId : randomIds)
    {
        totalPacketsSent++;
        sendPacketTo(destId);
    }
}

void
PC::sendPacketFromChunkServer()
{
    ConfigParser*      config      = ConfigParser::instance();
    ChunkServer*       chunkServer = ChunkServer::instance();
    ChunkServer::Chunk chunk       = chunkServer->loadChunk(id);
    QList<qsizetype>   receivers   = config->getReceiverPCsIDs();

    if(chunk.size() == 0) return;


    for(qsizetype receiverId : receivers)
    {
        if(receiverId == id) continue;

        IPPtr destIP = ipDatabase->getIP(receiverId).staticCast<IPv4>();

        tcp.send(destIP, chunk);
    }

    if(chunk.length() == 0)
    {
        UT_WARNING "PC" << id << "sent the last packet.";
    }
}

bool
PC::is(int id)
{
    return this->id == id;
}

void
PC::sendPacket(PacketPtr data)
{
    if(port)
    {
        port->sendPacket(data);
    }
    else
    {
        qDebug() << "PC" << id << "has no port connected to send packets.";
    }
}

void
PC::onNextTick(const int& packetCount)
{
    time++;

    if(ConfigParser::instance()->useChunkServer())
    {
        if(packetCount == 0) return;
        return sendPacketFromChunkServer();
    }

    sendPacketFromDataGenerator(packetCount);
}

void
PC::broadcast(PacketPtr packet, NetworkInterfacePtr incommingInterface)
{
    Q_UNUSED(incommingInterface)
    sendPacket(packet);
}

void
PC::setIpDB(IPDatabasePtr ipDatabase)
{
    this->ipDatabase = ipDatabase;
}

void
PC::sendPacketTo(int destinationId)
{
    auto  packet     = PacketPtr::create(UT::PacketType::Data, time);
    IPPtr destIP = ipDatabase->getIP(destinationId);
    auto  ipv4Header = IPv4HeaderPtr::create(getIP().staticCast<IPv4>(), destIP.staticCast<IPv4>());
    packet->setIPv4Header(ipv4Header);
    sendPacket(packet);
}

int
PC::getTotalPacketsSent() const
{
    return totalPacketsSent;
}

int
PC::getTotalPacketsReceived() const
{
    return totalPacketsReceived;
}

double
PC::calculatePacketLoss() const
{
    if(totalPacketsSent == 0)
    {
        return 0.0;
    }
    int lostPackets = totalPacketsSent - totalPacketsReceived;
    return (static_cast<double>(lostPackets) / totalPacketsSent) * 100.0;
}

const QVector<int>&
PC::getHopCounts() const
{
    return hopCounts;
}

QVector<int>
PC::getWaitingCycles()
{
    return waitingCycles;
}
