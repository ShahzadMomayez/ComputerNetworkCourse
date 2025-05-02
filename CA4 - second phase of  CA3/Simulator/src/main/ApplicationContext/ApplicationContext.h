#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include "../EventCoordinator/EventCoordinator.h"
#include "../Network/Network.h"
#include "../Node/Router.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

class ApplicationContext : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationContext(QString configPath, QObject* parent = nullptr);
    void run();

private:
    QStringList       getInput();
    void              handleCommands();
    bool              handleCommand(QString command, QStringList args);
    void              handlePacketLossCommand();
    void              handleHopCountAvgCommand();
    void              handleWaitingCyclesStatCommand();
    void              handlePrintRoutingTable(QStringList args);
    void              handlleUsedRoutersCommand();
    void              handlePoorRoutersCommand();
    void              handleCleanCommand();
    void              handleResetCommand();
    void              setUserConfig();
    void              setTopologyType();
    void              setRoutingProtocol();
    void              setBrokenRoutersStatus();
    void              setupNetwork();
    void              runSpanningTreeAlg();
    void              runDHCP();
    void              runRoutingProtocol();
    void              runBGP();
    void              startEventCoordinator(int simulationDuration, bool sendPacketCount = false);
    void              startSimulation();
    void              removeLogFiles();
    QVector<int>      listUsedRouters();
    QString           vectorToString(QVector<int> vector);

    Network*          network;
    QString           configPath;
    ConfigParser*     config;
    EventCoordinator* eventCoordinator;
    UT::TopologyType  topologyType;
    UT::RoutingProtocolType routingProtocol;
    bool                    brokenRouters;
};

#endif
