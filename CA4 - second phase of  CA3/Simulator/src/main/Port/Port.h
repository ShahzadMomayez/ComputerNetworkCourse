#ifndef PORT_H
#define PORT_H

#include "../Packet/Packet.h"

#include <QObject>

class Port : public QObject
{
    Q_OBJECT

public:
    explicit Port(QObject *parent = nullptr);
    ~Port() override;
    void sendPacket(const PacketPtr data);

Q_SIGNALS:
    void packetSent(const PacketPtr data);
    void packetReceived(const PacketPtr data);

public Q_SLOTS:
    // void sendPacket(const PacketPtr data);
    void onPacketReceived(const PacketPtr data);

private:
    uint8_t  number;
    uint64_t numberOfPacketsSent;
    QString  routerIP;
};

typedef QSharedPointer<Port> PortPtr;

#endif


