#include "Packet.h"
#include "../EventCoordinator/EventCoordinator.h"

#include <QDebug>

Packet::Packet(UT::PacketType type, int sentTime) {
    this->type = type;
    this->sentTime = sentTime;
}

Packet::~Packet() {}

Packet::Packet(UT::PacketControlType controlType) {
    this->type = UT::PacketType::Control;
    this->controlType = controlType;
}

void Packet::setPayload(PayloadPtr payload) {
    this->payload = payload;
}

UT::PacketType Packet::getType() {
    return type;
}

PayloadPtr Packet::getPayload() {
    return payload;
}

int Packet::getHopCount() const {
    return path.size();
}

UT::PacketControlType Packet::getControlType() {
    return controlType;
}

PacketPtr Packet::clone() {
    auto other = PacketPtr(new Packet(type));
    
    other->controlType = controlType;
    other->ipv6Header = ipv6Header;
    other->ipv4Header = ipv4Header;
    other->dataLinkHeader = dataLinkHeader;
    other->tcpHeader = tcpHeader;
    other->payload = payload->clone();
    other->sequenceNumber = sequenceNumber;
    other->path = path;
    other->sentTime = sentTime;
    other->receivedTime = receivedTime;

    return other;
}

void Packet::addPath(int hopId) {
    path.append(hopId);
}
int Packet::prevHopId() {
    return path.last();
}

void Packet::setIPv4Header(IPv4HeaderPtr ipv4Header) {
    this->ipv4Header = ipv4Header;
}

IPv4HeaderPtr Packet::getIPv4HeaderPtr() {
    return ipv4Header;
}

QVector<int> Packet::getPath() {
    return path;
}

int Packet::getWaitingCycles() {
    return receivedTime - sentTime;
}

void Packet::setReceivedTime(int time) {
    receivedTime = time;
}

int
Packet::getSentTime() const
{
    return sentTime;
}

void
Packet::setSentTime(int newSentTime)
{
    sentTime = newSentTime;
}

TCPHeaderPtr
Packet::getTcpHeader() const
{
    return tcpHeader;
}

void
Packet::setTcpHeader(TCPHeaderPtr newTcpHeader)
{
    tcpHeader = newTcpHeader;
}
