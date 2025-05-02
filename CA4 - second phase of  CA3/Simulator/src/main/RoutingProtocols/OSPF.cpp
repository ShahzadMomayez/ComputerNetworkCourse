#include "OSPF.h"
#include "../Packet/Packet.h"
#include "../ForwardingTable/ForwardingEntry.h"
#include "../LinkState/LinkStatePayload.h"
#include "../LinkState/LinkState.h"
#include "../Network/Network.h"

#include <QQueue>
#include <QDebug>
#include <algorithm>


OSPF::OSPF(QObject *parent) : QObject(parent) {}

void OSPF::initOSPF(RouterPtr router) {
}

void OSPF::runOSPF(RouterPtr router) {
    router->interfacesLock.lock();

    for(auto interface : router->getInterfaces())
    {
        if (!interface->isBorder()) {
            router->getLinkStateDatabase()->add(router->getId(), 1, interface->getToId(), interface->getToIP());
        }
    }

    router->interfacesLock.unlock();

    sendLinkStateAdvertisement(router);
}

void OSPF::sendLinkStateAdvertisement(RouterPtr router) {
    int routerId = router->getId();
    LinkStatePayloadPtr payload = LinkStatePayloadPtr(new LinkStatePayload(routerId));
    router->interfacesLock.lock();
    for (auto interface : router->getInterfaces()) {
        if (!interface->isBorder()) {
            payload->addLink(1, interface->toId, interface->getToIP());
        }
    }
    router->interfacesLock.unlock();
    PacketPtr packet = PacketPtr(new Packet(UT::PacketControlType::OSPF));
    packet->setPayload(payload);
    packet->addPath(router->getId());
    router->broadcast(packet);
}

bool
OSPF::updateLinkStateDatabase(Router* router, const PacketPtr packet)
{
    auto linkStatePayload = packet->getPayload().staticCast<LinkStatePayload>();

    if(!linkStatePayload)
    {
        qDebug() << "Invalid link-state update packet.";
        return false;
    }

    int senderId = linkStatePayload->getSenderId();
    QVector<LinkStatePtr> links = linkStatePayload->getLinks();

    LinkStateDatabasePtr database = router->getLinkStateDatabase();

    for (auto link : links) {
        database->add(senderId, link->getCost(), link->getNeighborId(), link->getNeighborIP());
    }

    computeShortestPath(router);
    return true;
}

void
OSPF::computeShortestPath(Router* router)
{
    LinkStateDatabasePtr database = router->getLinkStateDatabase();

    QMap<int, int> distance;
    QMap<int, int> previous;
    QSet<int> visited;
    QQueue<int> queue;

    for (auto routerId : database->nodes()) {
        distance[routerId] = std::numeric_limits<int>::max();
    }
    distance[router->getId()] = 0;

    queue.enqueue(router->getId());

    while (!queue.isEmpty()) {
        int current = queue.dequeue();
        visited.insert(current);

        for (const LinkStatePtr& link : database->edges(current)) {
            if (visited.contains(link->getNeighborId())) continue;
            int newDistance = distance[current] + link->getCost();
            if (newDistance < distance.value(link->getNeighborId(), std::numeric_limits<int>::max())) {
                distance[link->getNeighborId()] = newDistance;
                previous[link->getNeighborId()] = current == router->getId() ? link->getNeighborId() : previous[current];
                queue.enqueue(link->getNeighborId());
            }
        }
    }
    
    router->getForwardingTable()->clear();

    for (auto dest : distance.keys()) {
        if (dest == router->getId() || distance[dest] == std::numeric_limits<int>::max()) continue;

        IPPtr destinationIP = database->getIP(dest);

        NetworkInterfacePtr nextHopInterface = router->findInterface(previous[dest]);
        if (!nextHopInterface) {
            if (router->getId() == 1)
                qDebug() << "No interface found for next hop to" << dest;
            continue;
        }

        IPPtr nextHopIP = nextHopInterface->getToIP();
        IPPtr mask = IPPtr(new IPv4("255.255.255.255"));

        auto newEntry = ForwardingEntryPtr(new ForwardingEntry(
          destinationIP, mask, nextHopIP, distance[dest], UT::RoutingProtocolType::OSPF
          ));
        router->getForwardingTable()->addEntry(newEntry);
    }
}

