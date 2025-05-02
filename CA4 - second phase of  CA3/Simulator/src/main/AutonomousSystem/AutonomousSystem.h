#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H

#include "../Node/Router.h"
#include "../Globals/Globals.h"

#include <QObject>
#include <QVector>

class AutonomousSystem : public QObject
{
    Q_OBJECT

public:
    explicit AutonomousSystem(int id, int nodeCount, int startId, int bufferSize,  int routerPortCount, UT::TopologyType topologyType, QString networkPortion, QObject *parent = nullptr);
    ~AutonomousSystem();
    void createNetwork();
    RouterPtr getRouter(int routerId);
    bool is(int id);
    void convertMeshToTorus();
    void runSpanningTreeAlg();
    void runDHCP();
    void runOSPF();
    void runBGP();
    QVector<RouterPtr> getRouters();

private:
    void createLink(int i, int j);
    void createRouters();
    void createLinks();

    int id;
    int nodeCount;
    int startId;
    int bufferSize;
    int routerPortCount;
    QString networkPortion;
    UT::TopologyType topologyType;
    QVector<RouterPtr> routers;
    QVector<QThread *> threads;
};

typedef QSharedPointer<AutonomousSystem> AutonomousSystemPtr;

#endif
