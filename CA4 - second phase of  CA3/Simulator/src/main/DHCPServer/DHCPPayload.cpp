#include "DHCPPayload.h"

DHCPPayload::DHCPPayload(int id, MACAddressPtr macAddress, IPPtr ip, QObject *parent) :
    Payload("", parent)
{
    this->id = id;
    this->ip = ip;
    this->macAddress = macAddress;
    this->previousHopId = id;
}

int DHCPPayload::getId() {
    return id;
}
    

IPPtr DHCPPayload::getIP() {
    return ip;
}

void DHCPPayload::setPreviousHopId(int hopId) {
    previousHopId = hopId;
}

int DHCPPayload::getPreviousHopId() {
    return previousHopId;
}

void DHCPPayload::setIP(IPPtr ip) {
    this->ip = ip;
}

PayloadPtr DHCPPayload::clone() {
    auto other = DHCPPayloadPtr(new DHCPPayload(id, macAddress, ip));
    other->previousHopId = previousHopId;

    return other;
}

void DHCPPayload::setMACAddress(MACAddressPtr macAddress) {
    this->macAddress = macAddress;
}

MACAddressPtr DHCPPayload::getMACAddress() {
    return macAddress;
}
