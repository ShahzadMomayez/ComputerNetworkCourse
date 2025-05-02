#include "BGP.h"
#include "RoutingUpdate.h"

BGP::BGP(QObject *parent) :
    QObject {parent}
{}

void BGP::runBGP(RouterPtr router) {
    PacketPtr packet = PacketPtr(new Packet(UT::PacketControlType::BGP));
    auto routingUpdate = RoutingUpdatePtr(new RoutingUpdate(router->getId(), router->getForwardingTable()->clone()));
    packet->setPayload(routingUpdate);
    router->broadcastAbroad(packet);
}

void BGP::sendRoutingUpdate(Router* router, NetworkInterfacePtr incommingInterface) {
    RoutingUpdate::sendRoutingUpdate(router, incommingInterface, UT::PacketControlType::BGP);
}

bool BGP::updateForwardingTable(Router* router, const PacketPtr packet) {
    return RoutingUpdate::updateForwardingTable(router, packet, UT::RoutingProtocolType::BGP);
} 
