#ifndef IPV6HEADER_H
#define IPV6HEADER_H

#include <QObject>
#include <cstdint>
#include "../IP/IPv6.h"

class IPv6Header : public QObject
{
    Q_OBJECT
private:
    uint32_t versionTrafficClassFlowLabel;
    uint16_t payloadLength;
    uint8_t nextHeader;
    uint8_t hopLimit;
    IPv6Ptr sourceIP;
    IPv6Ptr destIP;
};

typedef QSharedPointer<IPv6Header> IPv6HeaderPtr;

#endif 
