#include "DHCPServer.h"

#include "../MACAddress/MACAddressGenerator.h"
#include "../Packet/Packet.h"
#include "DHCPPayload.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QTextStream>


DHCPServer::DHCPServer(std::string networkPortion, int id, QObject *parent) {
    this->networkPortion = networkPortion;
    this->id = id;

    QObject::connect(this, &DHCPServer::discoveryReceived, this, &DHCPServer::sendOffer);
    QObject::connect(this, &DHCPServer::requestReceived, this, &DHCPServer::sendAck);
}

DHCPServer::~DHCPServer() {}

void DHCPServer::sendOffer(const PacketPtr data) {
    PacketPtr packet = PacketPtr(new Packet(UT::PacketControlType::DHCPOffer));
    
    auto dhcpPayload = data->getPayload().staticCast<DHCPPayload>();
    dhcpPayload->setIP(generateIP(dhcpPayload->getId()));
    dhcpPayload->setMACAddress(MACAddressGenerator::instance()->generateUniqueAddress());

    packet->setPayload(dhcpPayload);

    Q_EMIT offerReady(packet);
}

void DHCPServer::sendAck(const PacketPtr data) {
    PacketPtr packet = PacketPtr(new Packet(UT::PacketControlType::DHCPAcknowledge));
    
    auto dhcpPayload = data->getPayload().staticCast<DHCPPayload>();

    packet->setPayload(dhcpPayload);
    
    Q_EMIT ackReady(packet);
    log(dhcpPayload->getId(), dhcpPayload->getMACAddress(), dhcpPayload->getIP());
}

IPPtr DHCPServer::generateIP(int id) {
    return IPv4Ptr(new IPv4(QString::fromStdString(networkPortion + "." + std::to_string(id))));
}

void DHCPServer::log(int nodeId, MACAddressPtr macAddress, IPPtr ipv4) {
    QString logsDir  = QString("%1/%2").arg(UT_SOURCE_DIR).arg("logs");
    QString filePath = QString("%1/dhcp_server_%2.txt").arg(logsDir, id);

    if(!QDir().exists(logsDir))
    {
        if(!QDir().mkdir(logsDir)) UT_ERROR "Failed to create logs directory: " << logsDir;
    }

    QFile file(filePath);

    bool  existed = file.exists();

    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        
        out << (existed ? "" : "node_id, mac_address, ipv4, ipv6\n");
        out << nodeId;
        out << ", ";
        out << macAddress->toString();
        out << ", ";
        out << ipv4->toString();
        out << "\n";

        file.close();
    }
    else
    {
        UT_ERROR "Failed to open log file:" << file.errorString();
    }
}
