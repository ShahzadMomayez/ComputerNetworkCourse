#include "Port.h"
#include <QObject>

Port::Port(QObject *parent) {}

Port::~Port() {}

void Port::sendPacket(const PacketPtr data) {
    numberOfPacketsSent++;
    Q_EMIT packetSent(data);
}



void Port::onPacketReceived(const PacketPtr data) {
    Q_EMIT packetReceived(data);
}
