#ifndef NETWORK_H
#define NETWORK_H

#include "../AutonomousSystem/AutonomousSystem.h"
#include "../IPDatabase/IPDatabase.h"
#include "../Node/PC.h"
#include "../utils/ConfigParser.h"

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QThread>

class Network : public QObject
{
    Q_OBJECT

    explicit Network(QString configPath, QString networkPortion = "192.168.111",
                     QObject *parent = nullptr);

public:
    static Network *instance(QString configPath, QString networkPortion = "192.168.111",
                             QObject *parent = nullptr);

    static void     release();

    ~Network();
    void                create();
    AutonomousSystemPtr getAutonomousSystem(int id);
    PCPtr               getPC(int id);
    RouterPtr           getRouter(int id);
    QVector<PCPtr>      getPCs();
    void                setInterfacesIPs();
    void                runSpanningTreeAlg();
    void                runDHCP();
    void                runRIP();
    void                runOSPF();
    void                runBGP();
    void                cleanupOSPFMemory();
    void                fillIpDB();
    QVector<RouterPtr>  getRouters();
    void                breakRouters();

private:
    void                         createAutonomousSystems();
    void                         createPCs();
    void                         connectASs();
    int                          getAutonomousSystemIndex(int id);
    void                         createLink(int fromAS, int toAS, int gateway, int connectTo);
    void                         setPCsIpDB();

    ConfigParser                *config;
    QVector<AutonomousSystemPtr> autonomousSystems;
    QVector<PCPtr>               PCs;
    QString                      networkPortion;
    QVector<QThread *>           threads;
    IPDatabasePtr                ipDatabase;

    inline static Network       *self = nullptr;
};

typedef QSharedPointer<Network> NetworkPtr;

#endif
