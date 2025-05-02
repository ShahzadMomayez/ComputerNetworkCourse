#include "PortUtil.h"

#include <QObject>

PortUtil::PortUtil() {}


QPair<PortPtr, PortPtr> PortUtil::createLink() {
    PortPtr port1(new Port());
    PortPtr port2(new Port());

    QObject::connect(port1.get(), &Port::packetSent, port2.get(), &Port::onPacketReceived);
    QObject::connect(port2.get(), &Port::packetSent, port1.get(), &Port::onPacketReceived);

    return qMakePair(port1, port2);
}
