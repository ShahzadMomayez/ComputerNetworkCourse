#include "Node.h"
#include "../IP/IPv4.h"
#include "../Packet/Packet.h"
#include "../DHCPServer/DHCPPayload.h"

#include <QDebug>

Node::Node(int id, IPPtr ip, QObject *parent)
{
    this->ip = ip;
    this->id = id;
    this->macAddress = nullptr;
}

Node::Node(int id, QString networkPortion, QObject *parent)
{
    this->ip = IPv4Ptr(new IPv4(networkPortion + "." + QString::number(id))); // This is a default IP, only for faster testing
    this->id = id;
}

IPPtr Node::getIP() {
    return ip;
}

int Node::getId() {
    return id;
}

void Node::discoverDHCP() {
    PacketPtr packet = PacketPtr(new Packet(UT::PacketControlType::DHCPDiscovery));
    DHCPPayloadPtr payload = DHCPPayloadPtr(new DHCPPayload(id));

    packet->setPayload(payload);

    broadcast(packet);
}

void Node::requestDHCP(IPPtr ip, MACAddressPtr macAddress) {
    PacketPtr      packet  = PacketPtr::create(UT::PacketControlType::DHCPRequest);
    DHCPPayloadPtr payload = DHCPPayloadPtr::create(id, macAddress, ip);

    packet->setPayload(payload);

    broadcast(packet);
}

void Node::setIP(IPPtr ip) {
    this->ip = ip;
}

void Node::setMACAddress(MACAddressPtr macAddress) {
    this->macAddress = macAddress;
}

MACAddressPtr Node::getMACAddress() {
    return macAddress;
}
