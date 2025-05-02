#include "Network.h"

#include "../Node/Router.h"
#include "../RoutingProtocols/OSPF.h"
#include "../RoutingProtocols/RIP.h"
#include "../utils/PortUtil.h"
#include "src/main/DataGenerator/ChunkServer.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

Network::Network(QString configPath, QString networkPortion, QObject *parent) :
    QObject {parent}
{
    this->config         = ConfigParser::instance(configPath);
    this->networkPortion = networkPortion;
    this->ipDatabase     = IPDatabasePtr::create();

    auto chunkServer     = ChunkServer::instance();
    chunkServer->setFilePath(config->getStreamingContentPath());
    chunkServer->setSenderPCsIDs(config->getSenderPCsIDs());
    chunkServer->setSenderPCsCount(config->getSenderPCsIDs().count());
    chunkServer->setPacketSize(config->getPacketSize());
    chunkServer->generateChunks();
}

Network *
Network::instance(QString configPath, QString networkPortion, QObject *parent)
{
    if(!self)
    {
        self = new Network(configPath, networkPortion, parent);
    }

    return self;
}

void
Network::release()
{
    if(self)
    {
        delete self;
        self = nullptr;
    }
}

Network::~Network()
{
    for(auto thread : threads)
    {
        thread->quit();
    }
}

void
Network::create()
{
    createAutonomousSystems();
    createPCs();
    connectASs();
}

void
Network::createAutonomousSystems()
{
    int startId = 1;
    for(int i = 0; i < config->getAsCount(); i++)
    {
        AutonomousSystemPtr autonomousSystem(new AutonomousSystem(config->getASId(i),
                                                                  config->getASNodeCount(i),
                                                                  startId,
                                                                  config->getRouterBufferSize(),
                                                                  config->getRouterPortCount(),
                                                                  config->getASTopologyType(i),
                                                                  networkPortion));
        autonomousSystem->createNetwork();
        autonomousSystem->getRouter(config->getASDHCPServer(i))
          ->enableDHCP(config->getASNetworkPortion(i));
        autonomousSystems.append(autonomousSystem);
        startId += config->getASNodeCount(i);
    }
}

void
Network::createPCs()
{
    for(int i = 0; i < autonomousSystems.size(); i++)
    {
        AutonomousSystemPtr   AS          = autonomousSystems[i];
        QMap<int, QList<int>> gatewaysMap = config->getGateways(i);

        for(const int &routerId : gatewaysMap.keys())
        {
            QList<int> users = gatewaysMap.value(routerId);

            for(int user : users)
            {
                PCPtr pc = PCPtr(new PC(user, networkPortion));
                threads.append(new QThread());
                pc->moveToThread(threads.last());
                threads.last()->start();

                QPair<PortPtr, PortPtr> link = PortUtil::createLink();
                AS->getRouter(routerId)->addPort(link.second, pc->getIP(), pc->getId());
                pc->addPort(link.first);

                PCs.append(pc);
            }
        }
    }
}

void
Network::connectASs()
{
    for(int fromAs = 0; fromAs < autonomousSystems.size(); fromAs++)
    {

        for(auto &connectToAS : config->getConnectToASs(fromAs))
        {
            int toAS = getAutonomousSystemIndex(connectToAS.first);

            for(auto &pair : connectToAS.second)
            {
                createLink(fromAs, toAS, pair.first, pair.second);
            }
        }
    }
}

int
Network::getAutonomousSystemIndex(int id)
{
    int index = 0;
    for(AutonomousSystemPtr AS : autonomousSystems)
    {
        if(AS->is(id))
        {
            return index;
        }
        index++;
    }
    return -1;
}

void
Network::createLink(int fromAS, int toAS, int gateway, int connectTo)
{
    QPair<PortPtr, PortPtr> link    = PortUtil::createLink();
    RouterPtr               router1 = autonomousSystems[fromAS]->getRouter(gateway);
    RouterPtr               router2 = autonomousSystems[toAS]->getRouter(connectTo);

    router1->addPort(link.first, router2->getIP(), router2->getId(), true);
    router2->addPort(link.second, router1->getIP(), router1->getId(), true);
}

AutonomousSystemPtr
Network::getAutonomousSystem(int id)
{
    int idx = getAutonomousSystemIndex(id);
    return autonomousSystems[idx];
}

QVector<RouterPtr>
Network::getRouters()
{
    QVector<RouterPtr> allRouters;
    for(const auto &autonomousSystem : autonomousSystems)
    {
        QVector<RouterPtr> routers = autonomousSystem->getRouters();
        allRouters.append(routers);
    }

    return allRouters;
}

PCPtr
Network::getPC(int id)
{
    for(PCPtr pc : PCs)
    {
        if(pc->is(id))
        {
            return pc;
        }
    }
    return nullptr;
}

QVector<PCPtr>
Network::getPCs()
{
    return PCs;
}

void
Network::runSpanningTreeAlg()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        autonomousSystem->runSpanningTreeAlg();
    }
}

void
Network::runRIP()
{
    for(auto pc : PCs)
    {
        RIP::runRIP(pc);
    }
}

void
Network::runDHCP()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        autonomousSystem->runDHCP();
    }

    for(auto pc : PCs)
    {
        pc->discoverDHCP();
    }
}

void
Network::runOSPF()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        autonomousSystem->runOSPF();
    }
}

void
Network::runBGP()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        autonomousSystem->runBGP();
    }
}

void
Network::setInterfacesIPs()
{
    fillIpDB();

    for(auto autonomousSystem : autonomousSystems)
    {
        for(auto router : autonomousSystem->getRouters())
        {
            for(auto interface : router->getInterfaces())
            {
                interface->setToIP(ipDatabase->getIP(interface->getToId()));
            }
        }
    }
}

void
Network::fillIpDB()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        for(auto router : autonomousSystem->getRouters())
        {
            ipDatabase->add(router->getId(), router->getIP(), router->getMACAddress());
        }
    }

    for(auto pc : PCs)
    {
        ipDatabase->add(pc->getId(), pc->getIP(), pc->getMACAddress(), true);
    }

    setPCsIpDB();
}

void
Network::cleanupOSPFMemory()
{
    for(auto autonomousSystem : autonomousSystems)
    {
        for(auto router : autonomousSystem->getRouters())
        {
            router->freeLinkStateDatabase();
        }
    }
}

void
Network::setPCsIpDB()
{
    for(auto pc : PCs)
    {
        pc->setIpDB(ipDatabase);
    }
}

RouterPtr
Network::getRouter(int id)
{
    for(auto autonomousSystem : autonomousSystems)
    {
        RouterPtr router = autonomousSystem->getRouter(id);

        if(router)
        {
            return router;
        }
    }

    return nullptr;
}

void
Network::breakRouters()
{
    for(int i = 0; i < autonomousSystems.size(); i++)
    {
        QVector<int> brokenRouters = config->getBrokenRouters(i);

        for(auto routerId : brokenRouters)
        {
            autonomousSystems[i]->getRouter(routerId)->broken();
        }
    }
}
