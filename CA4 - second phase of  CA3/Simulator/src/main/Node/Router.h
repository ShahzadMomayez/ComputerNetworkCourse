#ifndef ROUTER_H
#define ROUTER_H

#include "../DHCPServer/DHCPServer.h"
#include "../ForwardingTable/ForwardingTable.h"
#include "../Globals/Types.h"
#include "../LinkState/LinkStateDatabase.h"
#include "../Packet/Packet.h"
#include "../Port/NetworkInterface.h"
#include "../SpanningTree/SpanningTreeConfig.h"
#include "Node.h"

#include <QMap>
#include <QMutex>
#include <QObject>
#include <QVector>

class Router : public Node
{
    Q_OBJECT

public:
    explicit Router(int id, QString networkPortion, int bufferSize, int maxPortCount,
                    QObject *parent = nullptr);

    void addPort(PortPtr port, IPPtr ip, int toId, bool border = false);
    void broadcast(PacketPtr packet, NetworkInterfacePtr incomingInterface = nullptr);
    void broadcastAll(PacketPtr packet, NetworkInterfacePtr incomingInterface = nullptr);
    void broadcastAbroad(PacketPtr packet);
    void enableDHCP(std::string networkPortion);
    void discoverDHCP();

    QVector<NetworkInterfacePtr> getInterfaces() const;
    NetworkInterfacePtr          findInterface(int toId);
    ForwardingTablePtr           getForwardingTable();
    LinkStateDatabasePtr         getLinkStateDatabase();
    void                         freeLinkStateDatabase();
    bool                         isUsed() const;
    int                          getTotalPacketsReceived() const;
    int                          getTotalUserPacketsRouted() const;
    void                         broken();

public:
    QMutex interfacesLock;

protected:
    void requestDHCP(IPPtr ip, MACAddressPtr macAddress);

private:
    void                routeAll();
    int                 route(int bufferIndex = 0);
    bool                routeData(int bufferIndex);
    bool                routeSpanningTreeConfig(int bufferIndex);
    bool                routeDHCPDiscovery(int bufferIndex);
    bool                routeDHCPOffer(int bufferIndex);
    bool                routeDHCPRequest(int bufferIndex);
    bool                routeDHCPAcknowledge(int bufferIndex);
    bool                routeRIPUpdate(int bufferIndex);
    bool                routeOSPF(int bufferIndex);
    bool                routeBGP(int bufferIndex);
    void                bufferPacket(PacketPtr packet);
    bool                canBroadcastAll(NetworkInterfacePtr incomingInterface);
    bool                canBroadcast(NetworkInterfacePtr incomingInterface = nullptr);
    NetworkInterfacePtr findInterface(IPPtr ip);

private:
    QVector<PacketPtr>           buffer;
    ForwardingTablePtr           forwardingTable;
    QVector<NetworkInterfacePtr> interfaces;
    DHCPServerPtr                dhcpServer;
    int                          bufferSize;
    int                          maxPortCount;
    SpanningTreeConfigPtr        currentSpanningTreeConfig;
    LinkStateDatabasePtr         linkStateDatabase;
    int                          totalPacketsReceived   = 0;
    int                          totalUserPacketsRouted = 0;
    bool                         isBroken               = false;
    QMutex                       bufferLock;

Q_SIGNALS:
    void packetReceived(const PacketPtr data);
    void startSpanning(NetworkInterfacePtr incomingInterface = nullptr);

public Q_SLOTS:
    void sendSpanningTreeConfig(NetworkInterfacePtr incomingInterface = nullptr);
    void onPacketReceived(const PacketPtr packet);
    void onNextTick(const int &packetCount);
    void onOfferReady(const PacketPtr packet);
    void onAckReady(const PacketPtr packet);
};

typedef QSharedPointer<Router> RouterPtr;

#endif
