#include "RoutingUpdate.h"

RoutingUpdate::RoutingUpdate(int senderId, ForwardingTablePtr forwardingTable, QObject* parent) :
    Payload {"", parent}
{
    if(forwardingTable == nullptr)
    {
        forwardingTable = ForwardingTablePtr::create();
    }

    this->forwardingTable = forwardingTable;
    this->senderId        = senderId;
}

ForwardingTablePtr
RoutingUpdate::getForwardingTable()
{
    return forwardingTable;
}

int
RoutingUpdate::getSenderId()
{
    return senderId;
}

void
RoutingUpdate::sendRoutingUpdate(Router*               router,
                                 NetworkInterfacePtr   incommingInterface,
                                 UT::PacketControlType protocolType)
{
    PacketPtr packet = PacketPtr::create(protocolType);

    auto      routingUpdate =
      RoutingUpdatePtr::create(router->getId(), router->getForwardingTable()->clone());

    packet->setPayload(routingUpdate);
    router->broadcastAll(packet, incommingInterface);
}

bool
RoutingUpdate::updateForwardingTable(Router*                 router,
                                     const PacketPtr         packet,
                                     UT::RoutingProtocolType protocolType)
{
    bool  updated       = false;
    IPPtr mask          = IPv4Ptr::create("255.255.255.255");

    auto  routingUpdate = packet->getPayload().staticCast<RoutingUpdate>();
    IPPtr gateway       = router->findInterface(routingUpdate->getSenderId())->getToIP();

    for(auto newEntry : routingUpdate->getForwardingTable()->getEntries())
    {
        auto oldEntry = router->getForwardingTable()->findEntry(newEntry->getDestination());

        if(newEntry->isBetter(oldEntry))
        {
            auto forwardingEntry = ForwardingEntryPtr::create(newEntry->getDestination(),
                                                              mask,
                                                              gateway,
                                                              newEntry->getMetric() + 1,
                                                              protocolType);

            router->getForwardingTable()->replaceEntry(oldEntry, forwardingEntry);
            updated = true;
        }
    }

    return updated;
}
