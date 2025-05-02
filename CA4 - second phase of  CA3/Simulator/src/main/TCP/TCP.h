#ifndef TCP_H
#define TCP_H

#include "../IP/IP.h"
#include "../Packet/Packet.h"
#include "src/main/DataGenerator/ChunkServer.h"

#include <QHash>
#include <QObject>

struct TCPConnection
{
    qsizetype              sequenceNumber;
    qsizetype              acknowledgmentNumber;
    QVector<QByteArray>    buffer;
    bool                   isConnected;
    qsizetype              windowSize;

    UT::TCPConnectionState state = UT::TCPConnectionState::CLOSED;

    // Each KB of data is 1 byte :)
    qsizetype              totalBytesSent;
    qsizetype              totalBytesReceived;
    qsizetype              totalBytesDelivered;
    qsizetype              totalBytesAcked;
    qsizetype              totalBytesInFlight;
    qsizetype              totalBytesInBuffer;
    qsizetype              totalBytesInApplication;    // Streaming Content Length
};

class TCPProtocol : public QObject
{
    Q_OBJECT

public:
    explicit TCPProtocol(QObject *parent = nullptr);

    void  connectTo(const IPPtr &remoteIP);
    void  disconnectFrom(const IPPtr &remoteIP);
    void  handleReceivedPacket(const PacketPtr packet);

    void  wrapDataPacket(PacketPtr packet);
    bool  isConnectedTo(const IPPtr &remoteIP) const;

    void  setSendPacketToRemoteIP(const std::function<void(PacketPtr)> &newSendPacketToRemoteIP);

    void  send(const IPPtr &destIP, const ChunkServer::Chunk &chunk);

    IPPtr ownerIP() const;
    void  setOwnerIP(IPPtr newOwnerIP);

Q_SIGNALS:
    void dataReceived(const QString &from, const QByteArray &data);

public Q_SLOTS:
    void onNextTick(const int &packetCount);

private:
    void               handleAck(const PacketPtr packet);
    void               handleSyn(const PacketPtr packet);
    void               handleFin(const PacketPtr packet);
    void               handlePsh(const PacketPtr packet);

    void               deliverBufferToApplication(const QString &ip);

    void               setConnectedTo(const IPPtr &remoteIP, bool isConnected);

    QVector<PacketPtr> generatePackets(const IPPtr &remoteIP, const TCPConnection &connection,
                                       const ChunkServer::Chunk &chunk);

    PacketPtr          createSynPacket(const IPPtr &destIP);
    PacketPtr          createSynAckPacket(const IPPtr &destIP);
    PacketPtr          createAckPacket(const IPPtr &destIP);
    PacketPtr          createFinPacket(const IPPtr &destIP);
    PacketPtr          createFinAckPacket(const IPPtr &destIP);
    QList<PacketPtr>   createPshPacket(const IPPtr &destIP, const ChunkServer::Chunk &data);


private:
    qsizetype                      m_time;
    QHash<QString, TCPConnection>  m_connections;
    IPPtr                          m_ownerIP = nullptr;
    int lastIndexSend=-1;
    int cwnd = -1;
    int lastIndexAcked=-1;
    std::function<void(PacketPtr)> sendPacketToRemoteIP;
};

#endif    // TCP_H
