#include "PortBindingManager.h"

PortBindingManager::PortBindingManager(QObject *parent) :
    QObject {parent}
{}

void
PortBindingManager::bind(const PortPtr &port1, const PortPtr &port2)
{
    // simulate full-duplex communication
    // connect(port1.get(), &Port::packetSent, port2.get(), &Port::receivePacket);
    // connect(port2.get(), &Port::packetSent, port1.get(), &Port::receivePacket);
}

bool
PortBindingManager::unbind(const PortPtr &port1, const PortPtr &port2)
{}
