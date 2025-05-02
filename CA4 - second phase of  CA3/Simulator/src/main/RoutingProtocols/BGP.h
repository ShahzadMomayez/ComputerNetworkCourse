#ifndef BGP_H
#define BGP_H
#include "../Node/Router.h"

#include <QObject>

class BGP : public QObject
{
    Q_OBJECT

public:
    explicit BGP(QObject *parent = nullptr);
    static void runBGP(RouterPtr router);
    static void sendRoutingUpdate(Router* router, NetworkInterfacePtr incommingInterface);
    static bool updateForwardingTable(Router* router, const PacketPtr packet);

Q_SIGNALS:
};

#endif
