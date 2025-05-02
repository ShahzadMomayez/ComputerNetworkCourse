#ifndef RIP_H
#define RIP_H

#include <QObject>
#include "../Node/Router.h"
#include "../Node/PC.h"
#include "../ForwardingTable/ForwardingTable.h"
#include "../Packet/Packet.h"

class RIP : public QObject {
    Q_OBJECT

public:
    explicit RIP(RouterPtr router, QObject *parent = nullptr);

    static void runRIP(PCPtr pc);
    static void sendRoutingUpdate(Router* router, NetworkInterfacePtr incommingInterface);
    static bool updateForwardingTable(Router* router, const PacketPtr packet);

private Q_SLOTS:
    // void onPacketReceived(const PacketPtr &packet);
};

#endif
