#include "TCP.h"

#include "../EventCoordinator/EventCoordinator.h"
#include "../utils/ConfigParser.h"

int source_port= 443;
int dest_port = 443;
int seq_num =200;
int ack_num=300;

TCPProtocol::TCPProtocol(QObject *parent) :
    QObject {parent}, m_time(0)
{
    connect(EventCoordinator::instance(), &EventCoordinator::nextTick, this,
            &TCPProtocol::onNextTick);
}

void
TCPProtocol::connectTo(const IPPtr &remoteIP)
{
    Q_UNUSED(remoteIP)
    PacketPtr packet = PacketPtr::create(UT::PacketType::Control, m_time);

    auto tcpHeader = TCPHeaderPtr::create();

    tcpHeader->setFlags(UT::PacketControlType::TCP_SYN );

    tcpHeader->setSourcePort(source_port);
    tcpHeader->setDestPort(dest_port);
    tcpHeader->setSequenceNumber(seq_num);
    tcpHeader->setAcknowledgmentNumber(ack_num);

    packet->setTcpHeader(tcpHeader);
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Send SYN packet to the remote IP.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */

    sendPacketToRemoteIP(packet);
}

void
TCPProtocol::handleReceivedPacket(const PacketPtr packet)
{
    switch(packet->getTcpHeader()->flags())
    {
        case UT::PacketControlType::TCP_SYN :
            handleAck(packet);
            break;
        case UT::PacketControlType::TCP_ACK :
            handleSyn(packet);
            break;
        case UT::PacketControlType::TCP_PSH :
            handlePsh(packet);
            break;
        case UT::PacketControlType::TCP_FIN :
            handleFin(packet);
            break;
        default :
            break;
    }
}

void
TCPProtocol::wrapDataPacket(PacketPtr packet)
{
    auto tcpHeader  = TCPHeaderPtr::create();
    auto ip         = packet->getIPv4HeaderPtr()->destIP->toString();
    auto connection = m_connections[ip];

    tcpHeader->setSourcePort(443);
    tcpHeader->setDestPort(443);
    tcpHeader->setSequenceNumber(connection.sequenceNumber++);
    tcpHeader->setAcknowledgmentNumber(connection.acknowledgmentNumber);
    tcpHeader->setFlags(UT::PacketControlType::TCP_PSH);

    packet->setTcpHeader(tcpHeader);
}

void
TCPProtocol::handleAck(const PacketPtr packet)
{
    //Q_UNUSED(packet)
    auto tcpHeader = packet->getTcpHeader();
    qDebug() << "TCP 3-way handshake complete!";
    setConnectedTo(packet->getIPv4HeaderPtr()->sourceIP, true);
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Handle the received ACK packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
}

void
TCPProtocol::handleSyn(const PacketPtr packet)
{
    Q_UNUSED(packet)
    auto tcpHeader = packet->getTcpHeader();
    // Create and send a SYN-ACK packet
    PacketPtr synAckPacket = createSynAckPacket(packet->getIPv4HeaderPtr()->sourceIP);
    sendPacketToRemoteIP(synAckPacket);
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Handle the received SYN packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
}

void
TCPProtocol::handleFin(const PacketPtr packet)
{
    //Q_UNUSED(packet)

    auto tcpHeader = packet->getTcpHeader();
    PacketPtr finAckPacket = createFinAckPacket(packet->getIPv4HeaderPtr()->sourceIP);
    sendPacketToRemoteIP(finAckPacket);

    auto ip = packet->getIPv4HeaderPtr()->sourceIP->toString();
    if (m_connections.contains(ip))
    {
        auto& connection = m_connections[ip];
        connection.state = UT::TCPConnectionState::CLOSE_WAIT;
    }

    // At this point, the local side has sent the FIN-ACK, and now it's waiting for the other side
    // to send its ACK to complete the connection teardown.
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Handle the received FIN packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
}

void
TCPProtocol::handlePsh(const PacketPtr packet)
{
    auto ip         = packet->getIPv4HeaderPtr()->sourceIP->toString();
    auto connection = m_connections[ip];
    qDebug()<<"recived";
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Parse the payload and process it.
     * @attention store it in a buffer and after a while deliver it to the application.
     * @attention Send ACK to the source.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     **/

    if(connection.buffer.size() >= ConfigParser::instance()->getTCPWindowSize())
    {
        deliverBufferToApplication(ip);
    }
}

void
TCPProtocol::deliverBufferToApplication(const QString &ip)
{
    if(m_connections.contains(ip))
    {
        auto connection = m_connections[ip];

        for(const auto &data : connection.buffer)
        {
            Q_EMIT dataReceived(ip, data);
        }

        connection.buffer.clear();
    }
}

void
TCPProtocol::setConnectedTo(const IPPtr &remoteIP, bool isConnected)
{
    if(m_connections.contains(remoteIP->toString()))
    {
        m_connections[remoteIP->toString()].isConnected = isConnected;
    }
    else
    {
        TCPConnection connection;
        m_connections.insert(remoteIP->toString(), connection);
    }
}

QVector<PacketPtr>
TCPProtocol::generatePackets(const IPPtr &remoteIP, const TCPConnection &connection,
                             const ChunkServer::Chunk &chunk)
{
    switch(connection.state)
    {
        case UT::TCPConnectionState::CLOSED :
            return QVector<PacketPtr> {createSynPacket(remoteIP)};
            break;

        case UT::TCPConnectionState::SYN_SENT :
            return QVector<PacketPtr> {createSynAckPacket(remoteIP)};
            break;

        case UT::TCPConnectionState::SYN_RECEIVED :
            return QVector<PacketPtr> {createAckPacket(remoteIP)};
            break;

        case UT::TCPConnectionState::ESTABLISHED :
            return createPshPacket(remoteIP, chunk);
            break;

        case UT::TCPConnectionState::FIN_WAIT_1 :
            break;

        case UT::TCPConnectionState::FIN_WAIT_2 :
            break;

        case UT::TCPConnectionState::CLOSING :
            break;

        case UT::TCPConnectionState::TIME_WAIT :
            break;

        case UT::TCPConnectionState::CLOSE_WAIT :
            break;

        case UT::TCPConnectionState::LAST_ACK :
            break;

        case UT::TCPConnectionState::LISTEN :
            break;
    }

    return {};
}

PacketPtr
TCPProtocol::createSynPacket(const IPPtr &destIP)
{
    //Q_UNUSED(destIP)
    auto packet = PacketPtr::create(UT::PacketType::Control, m_time);
    auto tcpHeader = TCPHeaderPtr::create();

    tcpHeader->setFlags(UT::PacketControlType::TCP_SYN);
    tcpHeader->setSourcePort(443);
    tcpHeader->setDestPort(443);
    tcpHeader->setSequenceNumber(100);

    packet->setTcpHeader(tcpHeader);
    return packet;
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create a SYN packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */

}


PacketPtr
TCPProtocol::createSynAckPacket(const IPPtr &destIP)
{


    PacketPtr packet = PacketPtr::create(UT::PacketType::Control, m_time);
    auto tcpHeader = TCPHeaderPtr::create();
    tcpHeader->setFlags( UT::PacketControlType::TCP_ACK);
    tcpHeader->setSourcePort(source_port);
    tcpHeader->setDestPort(dest_port);
    tcpHeader->setSequenceNumber(seq_num);
    tcpHeader->setAcknowledgmentNumber(ack_num);
    packet->setTcpHeader(tcpHeader);
    return packet;
    //Q_UNUSED(destIP)
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create a SYN-ACK packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
    //return PacketPtr::create(UT::PacketType::Control, m_time);
}

PacketPtr
TCPProtocol::createAckPacket(const IPPtr &destIP)
{
    PacketPtr packet = PacketPtr::create(UT::PacketType::Control, m_time);
    auto tcpHeader = TCPHeaderPtr::create();
    tcpHeader->setFlags(UT::PacketControlType::TCP_ACK);
    tcpHeader->setSourcePort(source_port);
    tcpHeader->setDestPort(dest_port);
    tcpHeader->setSequenceNumber(seq_num);
    tcpHeader->setAcknowledgmentNumber(ack_num);
    packet->setTcpHeader(tcpHeader);

    return packet;



   // Q_UNUSED(destIP)
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create an ACK packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
   // return PacketPtr::create(UT::PacketType::Control, m_time);
}

PacketPtr
TCPProtocol::createFinPacket(const IPPtr &destIP)
{

    PacketPtr packet = PacketPtr::create(UT::PacketType::Control, m_time);

    auto tcpHeader = TCPHeaderPtr::create();
    tcpHeader->setFlags(UT::PacketControlType::TCP_FIN);
    tcpHeader->setSourcePort(source_port);
    tcpHeader->setDestPort(dest_port);
    tcpHeader->setSequenceNumber(seq_num);
    tcpHeader->setAcknowledgmentNumber(ack_num);

    packet->setTcpHeader(tcpHeader);

    return packet;

    //Q_UNUSED(destIP)
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create a FIN packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
    //return PacketPtr::create(UT::PacketType::Control, m_time);
}

PacketPtr
TCPProtocol::createFinAckPacket(const IPPtr &destIP)
{


    auto packet = PacketPtr::create(UT::PacketType::Control, m_time);
    auto tcpHeader = TCPHeaderPtr::create();
    tcpHeader->setFlags(UT::PacketControlType::TCP_ACK);
    tcpHeader->setSourcePort(443);
    tcpHeader->setDestPort(443);
    tcpHeader->setSequenceNumber(100);
    tcpHeader->setAcknowledgmentNumber(101);

    packet->setTcpHeader(tcpHeader);
    return packet;

    //Q_UNUSED(destIP)
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create a FIN-ACK packet.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
    //return PacketPtr::create(UT::PacketType::Control, m_time);
}

QList<PacketPtr>
TCPProtocol::createPshPacket(const IPPtr &destIP, const ChunkServer::Chunk &data)
{
    // Q_UNUSED(destIP)
    // Q_UNUSED(data)
    QList<PacketPtr> toSend;
    if ((lastIndexSend-lastIndexAcked)<cwnd)
    {
        lastIndexSend++;
        IPv4HeaderPtr header = IPv4HeaderPtr::create(IPv4Ptr::create((IPv4*)m_ownerIP.get()),IPv4Ptr::create((IPv4*) destIP.get()));
        data[lastIndexSend]->setIPv4Header(header);
        toSend.append(data[lastIndexSend]);
    }
    // newPacket->setPayload()
    /**
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     * @attention Create PSH packets.
     * @attention You should follow TCP congestion control algorithms here.
     * @attention Return a list of packets to be sent.
     * ======================================================
     * ======================================================
     * ======================================================
     * ======================================================
     */
    return {};
}

IPPtr
TCPProtocol::ownerIP() const
{
    return m_ownerIP;
}

void
TCPProtocol::setOwnerIP(IPPtr newOwnerIP)
{
    m_ownerIP = newOwnerIP;
}

void
TCPProtocol::setSendPacketToRemoteIP(const std::function<void(PacketPtr)> &newSendPacketToRemoteIP)
{
    sendPacketToRemoteIP = newSendPacketToRemoteIP;
}

void
TCPProtocol::send(const IPPtr &destIP, const ChunkServer::Chunk &chunk)
{
    auto connection = m_connections[destIP->toString()];

    IPv4Ptr destinationIP  = destIP.staticCast<IPv4>();
    IPv4Ptr sourceIP       = m_ownerIP.staticCast<IPv4>();
    auto  ipv4Header     = IPv4HeaderPtr::create(sourceIP, destinationIP);
    connection.state = UT::TCPConnectionState::ESTABLISHED ;

    auto  sendingPackets = generatePackets(destIP, connection, chunk);

    for(auto &packet : sendingPackets)
    {
        packet->setIPv4Header(ipv4Header);
        wrapDataPacket(packet);
        sendPacketToRemoteIP(packet);
    }
}

void
TCPProtocol::onNextTick(const int &packetCount)
{
    Q_UNUSED(packetCount)
    m_time++;
}

bool
TCPProtocol::isConnectedTo(const IPPtr &remoteIP) const
{
    if(m_connections.contains(remoteIP->toString()))
    {
        return m_connections[remoteIP->toString()].isConnected;
    }

    return false;
}

