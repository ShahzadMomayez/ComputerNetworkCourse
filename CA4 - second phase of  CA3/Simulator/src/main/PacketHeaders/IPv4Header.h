#ifndef IPV4HEADER_H
#define IPV4HEADER_H

#include "../IP/IPv4.h"

#include <cstdint>
#include <QObject>

class IPv4Header : public QObject
{
    Q_OBJECT

public:
    IPv4Header(IPv4Ptr sourceIP, IPv4Ptr destIP);
    IPv4Ptr getDestIP();

    uint8_t versionHeaderLength;
    uint8_t typeOfService;
    uint16_t totalLength;
    uint16_t identification;
    uint16_t flagsFragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t headerChecksum;
    IPv4Ptr sourceIP;
    IPv4Ptr destIP;
};

typedef QSharedPointer<IPv4Header> IPv4HeaderPtr;

#endif 
