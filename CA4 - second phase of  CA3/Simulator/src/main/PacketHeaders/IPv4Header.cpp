#include "IPv4Header.h"

IPv4Header::IPv4Header(IPv4Ptr sourceIP, IPv4Ptr destIP) {
    this->sourceIP = sourceIP;
    this->destIP = destIP;
}

IPv4Ptr IPv4Header::getDestIP() {
    return destIP;
}