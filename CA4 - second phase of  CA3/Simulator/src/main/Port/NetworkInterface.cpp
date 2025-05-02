#include "NetworkInterface.h"

#include <QDebug>

NetworkInterface::NetworkInterface(PortPtr port, IPPtr toIP, int toId, bool border, bool hasSent,
                                   bool broadcast)
{
    this->port      = port;
    this->toIP      = toIP;
    this->sent      = hasSent;
    this->broadcast = broadcast;
    this->toId      = toId;
    this->border    = border;
}

void
NetworkInterface::send(const PacketPtr data)
{
    port->sendPacket(data);
    sent = true;
}

bool
NetworkInterface::hasSent()
{
    return sent;
}

bool
NetworkInterface::equals(IPPtr toIP)
{
    return this->toIP->equals(toIP);
}

void
NetworkInterface::resetSent()
{
    sent = false;
}

bool
NetworkInterface::shouldBroadcast()
{
    return broadcast && !border;
}

bool
NetworkInterface::equals(int toId)
{
    return this->toId == toId;
}

void
NetworkInterface::setBroadcast(bool broadcast)
{
    this->broadcast = broadcast;
}

bool
NetworkInterface::isBorder()
{
    return border;
}

IPPtr
NetworkInterface::getToIP()
{
    return toIP;
}

int
NetworkInterface::getToId()
{
    return toId;
}

void
NetworkInterface::setToIP(IPPtr toIP)
{
    this->toIP = toIP;
}
