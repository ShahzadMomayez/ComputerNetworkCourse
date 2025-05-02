#include "Router.h"

#include "../DHCPServer/DHCPPayload.h"
#include "../EventCoordinator/EventCoordinator.h"
#include "../RoutingProtocols/BGP.h"
#include "../RoutingProtocols/OSPF.h"
#include "../RoutingProtocols/RIP.h"
#include "../RoutingProtocols/RoutingUpdate.h"
#include "../SpanningTree/SpanningTreeConfig.h"

#include <QDebug>

Router::Router(int id, QString networkPortion, int bufferSize, int maxPortCount, QObject* parent) :
    Node(id, networkPortion, parent)
{
    this->id                        = id;
    this->bufferSize                = bufferSize;
    this->maxPortCount              = maxPortCount;
    this->forwardingTable           = ForwardingTablePtr::create();
    this->currentSpanningTreeConfig = SpanningTreeConfigPtr::create(id, 0, id, id);
    this->linkStateDatabase         = LinkStateDatabasePtr::create();

    dhcpServer                      = nullptr;

    QObject::connect(this, &Router::packetReceived, this, &Router::onPacketReceived);

    QObject::connect(EventCoordinator::instance(), &EventCoordinator::nextTick, this,
                     &Router::onNextTick);

    QObject::connect(this, &Router::startSpanning, this, &Router::sendSpanningTreeConfig);
}

void
Router::addPort(PortPtr port, IPPtr toIP, int toId, bool border)
{
    if(interfaces.size() >= maxPortCount)
    {
        qDebug() << "Max port count reached";
        return;
    }

    port->moveToThread(thread());
    QObject::connect(port.get(), &Port::packetReceived, this, &Router::packetReceived);
    auto interface = NetworkInterfacePtr(new NetworkInterface(port, toIP, toId, border));
    interfaces.append(interface);
}

void
Router::broadcast(PacketPtr packet, NetworkInterfacePtr incommingInterface)
{
    if(isBroken) return;

    interfacesLock.lock();

    for(const NetworkInterfacePtr& interface : interfaces)
    {
        if(interface->shouldBroadcast() && interface != incommingInterface)
        {
            interface->send(packet);
        }
    }

    interfacesLock.unlock();
}

void
Router::broadcastAll(PacketPtr packet, NetworkInterfacePtr incommingInterface)
{
    if(isBroken) return;

    interfacesLock.lock();

    for(const NetworkInterfacePtr& interface : interfaces)
    {
        if(incommingInterface != interface && !interface->isBorder())
        {
            interface->send(packet);
        }
    }

    interfacesLock.unlock();
}

LinkStateDatabasePtr
Router::getLinkStateDatabase()
{
    return linkStateDatabase;
}

void
Router::broadcastAbroad(PacketPtr packet)
{
    if(isBroken) return;

    interfacesLock.lock();

    for(const NetworkInterfacePtr& interface : interfaces)
    {
        if(interface->isBorder())
        {
            interface->send(packet);
        }
    }

    interfacesLock.unlock();
}

void
Router::onPacketReceived(const PacketPtr packet)
{
    if(isBroken) return;

    if(packet != nullptr)
    {
        bufferPacket(packet);

        routeAll();
    }
}

NetworkInterfacePtr
Router::findInterface(IPPtr ip)
{
    for(const NetworkInterfacePtr& interface : interfaces)
    {
        if(interface->equals(ip))
        {
            return interface;
        }
    }
    return nullptr;
}

ForwardingTablePtr
Router::getForwardingTable()
{
    return forwardingTable;
}

void
Router::bufferPacket(PacketPtr packet)
{
    if(buffer.size() < bufferSize)
    {
        buffer.append(packet);
    }
    else
    {
        // qDebug() << id << "Packet dropped";
    }
}

void
Router::onNextTick(const int& packetCount)
{
    interfacesLock.lock();
    for(NetworkInterfacePtr interface : interfaces)
    {
        interface->resetSent();
    }
    interfacesLock.unlock();

    routeAll();
}

void
Router::sendSpanningTreeConfig(NetworkInterfacePtr incommingInterface)
{
    auto configPacket = PacketPtr(new Packet(UT::PacketControlType::SpanningTreeConfig));
    configPacket->setPayload(currentSpanningTreeConfig->clone(id));

    broadcastAll(configPacket, incommingInterface);
}

int
Router::route(int bufferIndex)
{
    if(bufferIndex >= buffer.size())
    {
        return -1;
    }
    PacketPtr packet        = buffer[bufferIndex];
    bool      packetRemoved = false;

    switch(packet->getType())
    {
        case UT::PacketType::Data :
            packetRemoved = routeData(bufferIndex);
            break;
        case UT::PacketType::Control :
            switch(packet->getControlType())
            {
                case UT::PacketControlType::SpanningTreeConfig :
                    packetRemoved = routeSpanningTreeConfig(bufferIndex);
                    break;
                case UT::PacketControlType::DHCPDiscovery :
                    packetRemoved = routeDHCPDiscovery(bufferIndex);
                    break;
                case UT::PacketControlType::DHCPOffer :
                    packetRemoved = routeDHCPOffer(bufferIndex);
                    break;
                case UT::PacketControlType::DHCPRequest :
                    packetRemoved = routeDHCPRequest(bufferIndex);
                    break;
                case UT::PacketControlType::DHCPAcknowledge :
                    packetRemoved = routeDHCPAcknowledge(bufferIndex);
                    break;
                case UT::PacketControlType::RIP :
                    packetRemoved = routeRIPUpdate(bufferIndex);
                    break;
                case UT::PacketControlType::OSPF :
                    packetRemoved = routeOSPF(bufferIndex);
                    break;
                case UT::PacketControlType::BGP :
                    packetRemoved = routeBGP(bufferIndex);
                    break;
                default :
                    break;
            }
            break;
        default :
            break;
    }

    return packetRemoved ? bufferIndex : bufferIndex + 1;
}

bool
Router::routeData(int bufferIndex)
{
    PacketPtr packet  = buffer[bufferIndex];
    IPPtr     gateway = forwardingTable->getGatway(packet->getIPv4HeaderPtr()->getDestIP());

    NetworkInterfacePtr interface = findInterface(gateway);

    if(interface != nullptr && !interface->hasSent())
    {
        buffer.removeAt(bufferIndex);
        totalPacketsReceived++;
        totalUserPacketsRouted++;
        packet->addPath(id);
        interface->send(packet);
        return true;
    }
    return false;
}

bool
Router::routeRIPUpdate(int bufferIndex)
{
    PacketPtr           packet    = buffer[bufferIndex];
    auto                payload   = packet->getPayload().staticCast<RoutingUpdate>();
    NetworkInterfacePtr interface = findInterface(payload->getSenderId());
    if(!canBroadcastAll(interface))
    {
        return false;
    }

    buffer.removeAt(bufferIndex);
    if(RIP::updateForwardingTable(this, packet))
    {
        RIP::sendRoutingUpdate(this, interface);
    }

    return true;
}

bool
Router::routeBGP(int bufferIndex)
{
    PacketPtr           packet    = buffer[bufferIndex];
    auto                payload   = packet->getPayload().staticCast<RoutingUpdate>();
    NetworkInterfacePtr interface = findInterface(payload->getSenderId());
    if(!canBroadcastAll(interface))
    {
        return false;
    }

    buffer.removeAt(bufferIndex);
    if(BGP::updateForwardingTable(this, packet))
    {
        BGP::sendRoutingUpdate(this, interface);
    }

    return true;
}

bool
Router::routeOSPF(int bufferIndex)
{
    QMutexLocker        lock(&bufferLock);

    PacketPtr           packet           = buffer[bufferIndex]->clone();
    NetworkInterfacePtr prevHopInterface = findInterface(packet->prevHopId());

    if(canBroadcast(prevHopInterface))
    {
        OSPF::updateLinkStateDatabase(this, packet);
        buffer.removeAt(bufferIndex);
        packet->addPath(id);
        broadcast(packet, prevHopInterface);
        return true;
    }
    return false;
}

bool
Router::routeDHCPDiscovery(int bufferIndex)
{
    PacketPtr           packet      = buffer[bufferIndex]->clone();
    auto                dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();
    NetworkInterfacePtr interface   = findInterface(dhcpPayload->getPreviousHopId());

    if(canBroadcast(interface))
    {
        buffer.removeAt(bufferIndex);
        dhcpPayload->setPreviousHopId(id);

        if(dhcpServer != nullptr)
        {
            Q_EMIT dhcpServer->discoveryReceived(packet);
        }
        else
        {
            broadcast(packet, interface);
        }
        return true;
    }
    return false;
}

bool
Router::routeDHCPOffer(int bufferIndex)
{
    PacketPtr           packet      = buffer[bufferIndex]->clone();
    auto                dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();
    NetworkInterfacePtr interface   = findInterface(dhcpPayload->getPreviousHopId());

    if(canBroadcast(interface))
    {
        buffer.removeAt(bufferIndex);
        dhcpPayload->setPreviousHopId(id);

        if(dhcpPayload->getId() == id)
        {
            requestDHCP(dhcpPayload->getIP(), dhcpPayload->getMACAddress());
        }
        else
        {
            broadcast(packet, interface);
        }
        return true;
    }
    return false;
}

bool
Router::routeDHCPRequest(int bufferIndex)
{
    PacketPtr           packet      = buffer[bufferIndex]->clone();
    auto                dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();
    NetworkInterfacePtr interface   = findInterface(dhcpPayload->getPreviousHopId());

    if(canBroadcast(interface))
    {
        buffer.removeAt(bufferIndex);
        dhcpPayload->setPreviousHopId(id);

        if(dhcpServer != nullptr)
        {
            Q_EMIT dhcpServer->requestReceived(packet);
        }
        else
        {
            broadcast(packet, interface);
        }
        return true;
    }
    return false;
}

bool
Router::routeDHCPAcknowledge(int bufferIndex)
{
    PacketPtr           packet      = buffer[bufferIndex]->clone();
    auto                dhcpPayload = packet->getPayload().staticCast<DHCPPayload>();
    NetworkInterfacePtr interface   = findInterface(dhcpPayload->getPreviousHopId());

    if(dhcpPayload->getId() == id)
    {
        buffer.removeAt(bufferIndex);
        setIP(dhcpPayload->getIP());
        setMACAddress(dhcpPayload->getMACAddress());
        return true;
    }
    else if(canBroadcast(interface))
    {
        dhcpPayload->setPreviousHopId(id);
        buffer.removeAt(bufferIndex);
        broadcast(packet, interface);
        return true;
    }
    return false;
}

bool
Router::routeSpanningTreeConfig(int bufferIndex)
{
    PacketPtr           packet    = buffer[bufferIndex];
    auto                newConfig = packet->getPayload().staticCast<SpanningTreeConfig>();
    NetworkInterfacePtr interface = findInterface(newConfig->getSenderId());

    if(!canBroadcastAll(interface))
    {
        return false;
    }
    else
    {
        buffer.removeAt(bufferIndex);

        bool isBetter        = newConfig->isBetter(currentSpanningTreeConfig);
        bool shouldBroadcast = isBetter || !currentSpanningTreeConfig->indicateLoop(newConfig, id);
        interface->setBroadcast(shouldBroadcast);

        if(isBetter)
        {
            currentSpanningTreeConfig->update(newConfig);
            sendSpanningTreeConfig(interface);
        }
        return true;
    }
}

NetworkInterfacePtr
Router::findInterface(int toId)
{
    interfacesLock.lock();
    for(const auto& interface : interfaces)
    {
        if(interface->equals(toId))
        {
            interfacesLock.unlock();
            return interface;
        }
    }
    interfacesLock.unlock();
    return nullptr;
}

bool
Router::canBroadcastAll(NetworkInterfacePtr incommingInterface)
{
    interfacesLock.lock();
    for(auto interface : interfaces)
    {
        if(interface != incommingInterface && !interface->isBorder() && interface->hasSent())
        {
            interfacesLock.unlock();
            return false;
        }
    }
    interfacesLock.unlock();
    return true;
}

QVector<NetworkInterfacePtr>
Router::getInterfaces() const
{
    return interfaces;
}

bool
Router::canBroadcast(NetworkInterfacePtr incommingInterface)
{
    interfacesLock.lock();
    for(auto interface : interfaces)
    {
        if(interface != incommingInterface && interface->shouldBroadcast() && interface->hasSent())
        {
            interfacesLock.unlock();
            return false;
        }
    }
    interfacesLock.unlock();
    return true;
}

void
Router::enableDHCP(std::string networkPortion)
{
    dhcpServer = DHCPServerPtr(new DHCPServer(networkPortion, id));
    dhcpServer->moveToThread(thread());

    QObject::connect(dhcpServer.get(), &DHCPServer::offerReady, this, &Router::onOfferReady);
    QObject::connect(dhcpServer.get(), &DHCPServer::ackReady, this, &Router::onAckReady);
}

void
Router::onOfferReady(const PacketPtr packet)
{
    bufferPacket(packet);

    routeAll();
}

void
Router::onAckReady(const PacketPtr packet)
{
    bufferPacket(packet);

    routeAll();
}

void
Router::discoverDHCP()
{
    if(isBroken) return;

    if(dhcpServer == nullptr)
    {
        Node::discoverDHCP();
    }
    else
    {
        PacketPtr      packet  = PacketPtr(new Packet(UT::PacketControlType::DHCPDiscovery));
        DHCPPayloadPtr payload = DHCPPayloadPtr(new DHCPPayload(id));

        packet->setPayload(payload);

        Q_EMIT dhcpServer->discoveryReceived(packet);
    }
}

void
Router::requestDHCP(IPPtr ip, MACAddressPtr macAddress)
{
    if(dhcpServer == nullptr)
    {
        Node::requestDHCP(ip, macAddress);
    }
    else
    {
        PacketPtr      packet  = PacketPtr(new Packet(UT::PacketControlType::DHCPRequest));
        DHCPPayloadPtr payload = DHCPPayloadPtr(new DHCPPayload(id, macAddress, ip));

        packet->setPayload(payload);
        Q_EMIT dhcpServer->requestReceived(packet);
    }
}

void
Router::routeAll()
{
    int next = 0;

    while(next != -1)
    {
        next = route(next);
    }
}

void
Router::freeLinkStateDatabase()
{
    linkStateDatabase = nullptr;
}

int
Router::getTotalPacketsReceived() const
{
    return totalPacketsReceived;
}

bool
Router::isUsed() const
{
    return totalPacketsReceived > 0;
}

void
Router::broken()
{
    isBroken = true;
}
