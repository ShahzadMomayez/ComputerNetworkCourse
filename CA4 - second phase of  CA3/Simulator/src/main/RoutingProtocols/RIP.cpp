#include "RIP.h"

#include "../Node/Router.h"
#include "RoutingUpdate.h"

#include <QDebug>

RIP::RIP(RouterPtr router, QObject* parent) :
    QObject {parent}
{}

void
RIP::runRIP(PCPtr pc)
{
    PacketPtr packet        = PacketPtr(new Packet(UT::PacketControlType::RIP));
    auto      routingUpdate = RoutingUpdatePtr(new RoutingUpdate(pc->getId()));

    IPPtr     mask          = IPPtr(new IPv4("255.255.255.255"));
    auto      entry =
      ForwardingEntryPtr::create(pc->getIP(), mask, pc->getIP(), 0, UT::RoutingProtocolType::RIP);

    routingUpdate->getForwardingTable()->addEntry(entry);

    packet->setPayload(routingUpdate);

    pc->sendPacket(packet);
}

void
RIP::sendRoutingUpdate(Router* router, NetworkInterfacePtr incommingInterface)
{
    RoutingUpdate::sendRoutingUpdate(router, incommingInterface, UT::PacketControlType::RIP);
}

bool
RIP::updateForwardingTable(Router* router, const PacketPtr packet)
{
    return RoutingUpdate::updateForwardingTable(router, packet, UT::RoutingProtocolType::RIP);
}
