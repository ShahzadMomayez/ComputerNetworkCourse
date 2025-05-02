#ifndef PACKET_H
#define PACKET_H

#include "../Globals/Globals.h"
#include "../IP/IP.h"
#include "../IP/IPv4.h"
#include "../PacketHeaders/DataLinkHeader.h"
#include "../PacketHeaders/IPv4Header.h"
#include "../PacketHeaders/IPv6Header.h"
#include "../PacketHeaders/TCPHeader.h"
#include "../Payload/Payload.h"

#include <QObject>
#include <QVector>

class Packet;
typedef QSharedPointer<Packet> PacketPtr;

class Packet : public QObject
{
    Q_OBJECT

public:
    Packet(UT::PacketType type = UT::PacketType::Data, int sentTime = 0);
    Packet(UT::PacketControlType controlType);
    ~Packet();

    void                  addPath(int hopId);
    int                   prevHopId();
    void                  setPayload(PayloadPtr payload);
    int                   getHopCount() const;

    UT::PacketControlType getControlType();
    UT::PacketType        getType();
    PayloadPtr            getPayload();
    PacketPtr             clone();

    void                  setIPv4Header(IPv4HeaderPtr ipv4Header);
    IPv4HeaderPtr         getIPv4HeaderPtr();
    QVector<int>          getPath();
    int                   getWaitingCycles();
    void                  setReceivedTime(int time);

    int                   getSentTime() const;
    void                  setSentTime(int newSentTime);

    TCPHeaderPtr          getTcpHeader() const;
    void                  setTcpHeader(TCPHeaderPtr newTcpHeader);

private:
    UT::PacketType        type;
    UT::PacketControlType controlType;
    IPv4HeaderPtr         ipv4Header;
    IPv6HeaderPtr         ipv6Header;
    DataLinkHeaderPtr     dataLinkHeader;
    TCPHeaderPtr          tcpHeader;
    PayloadPtr            payload;
    int                   sequenceNumber;
    int                   sentTime;
    int                   receivedTime;

    // QVector<IPPtr> path;
    QVector<int>          path;

private
    Q_SLOT : void onNextTick(const int& packetCount);
};

#endif
