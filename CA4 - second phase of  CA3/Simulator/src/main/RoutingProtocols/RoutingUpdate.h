#ifndef ROUTINGUPDATE_H
#define ROUTINGUPDATE_H

#include "../ForwardingTable/ForwardingTable.h"
#include "../Node/Router.h"
#include "../Packet/Packet.h"
#include "../Payload/Payload.h"

#include <QObject>
class RoutingUpdate : public Payload
{
    Q_OBJECT

public:
    explicit RoutingUpdate(int senderId, ForwardingTablePtr forwardingTable = nullptr,
                           QObject* parent = nullptr);

    static void        sendRoutingUpdate(Router* router, NetworkInterfacePtr incommingInterface,
                                         UT::PacketControlType protocolType);

    static bool        updateForwardingTable(Router* router, const PacketPtr packet,
                                             UT::RoutingProtocolType protocolType);

    ForwardingTablePtr getForwardingTable();
    int                getSenderId();

Q_SIGNALS:

private:
    int                senderId;
    ForwardingTablePtr forwardingTable;
};

typedef QSharedPointer<RoutingUpdate> RoutingUpdatePtr;

#endif
