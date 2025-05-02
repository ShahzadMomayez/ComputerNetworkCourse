#ifndef OSPF_H
#define OSPF_H

#include "../Node/Router.h"
#include "../Packet/Packet.h"
#include "../Globals/Types.h"
#include "../ForwardingTable/ForwardingEntry.h"
#include "../LinkState/LinkStatePayload.h"

#include <QVector>
#include <QMap>
#include <QObject>
#include <limits>

class OSPF : public QObject {
    Q_OBJECT

public:
    explicit OSPF(QObject *parent = nullptr);

    static void initOSPF(RouterPtr router);
    static void runOSPF(RouterPtr router);
    static void sendLinkStateAdvertisement(RouterPtr router);
    static bool updateLinkStateDatabase(Router *router, const PacketPtr packet);

private:
    static void computeShortestPath(Router *router);

    struct LinkState {
        int cost;
        int neighborId;
    };

    static QMap<int, QVector<LinkState>> linkStateDatabase;
};

#endif
