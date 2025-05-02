#include "AutonomousSystem.h"

#include "../RoutingProtocols/BGP.h"
#include "../RoutingProtocols/OSPF.h"
#include "../utils/PortUtil.h"

#include <cmath>

#include <QMetaObject>

AutonomousSystem::AutonomousSystem(int id, int nodeCount, int startId, int bufferSize,
                                   int routerPortCount, UT::TopologyType topologyType,
                                   QString networkPortion, QObject *parent) :
    QObject {parent}
{
    this->id              = id;
    this->nodeCount       = nodeCount;
    this->startId         = startId;
    this->bufferSize      = bufferSize;
    this->topologyType    = topologyType;
    this->routerPortCount = routerPortCount;
    this->networkPortion  = networkPortion;
}

AutonomousSystem::~AutonomousSystem()
{
    UT_INFO "FUCK?";

    for(auto thread : threads)
    {
        thread->quit();
    }
}

void
AutonomousSystem::createNetwork()
{
    createRouters();
    createLinks();
}

void
AutonomousSystem::createRouters()
{
    for(int i = 0; i < nodeCount; i++)
    {
        auto router = RouterPtr::create(startId + i, networkPortion, bufferSize, routerPortCount);
        auto thread = new QThread();

        router->moveToThread(thread);
        thread->start();

        router->setObjectName(QString("router_") + QString::number(startId + i));
        thread->setObjectName(QString("thread_") + QString::number(startId + i));

        connect(thread, &QThread::destroyed, this, [](QObject *obj) {
            if(obj)
                UT_ERROR obj->objectName() << "destroyed";
            else
                UT_ERROR "One thread destroyed.";
        });

        connect(router.get(), &QObject::destroyed, this, [](QObject *obj) {
            if(obj)
                UT_ERROR obj->objectName() << "destroyed";
            else
                UT_ERROR "One router destroyed.";
        });

        threads.append(thread);
        routers.append(router);
    }
}

void
AutonomousSystem::createLink(int i, int j)
{
    QPair<PortPtr, PortPtr> link = PortUtil::createLink();
    routers[i]->addPort(link.first, routers[j]->getIP(), routers[j]->getId());
    routers[j]->addPort(link.second, routers[i]->getIP(), routers[i]->getId());
}

void
AutonomousSystem::createLinks()
{
    int gridSize = std::sqrt(nodeCount);

    if(topologyType == UT::TopologyType::Mesh)
    {
        for(int i = 0; i < nodeCount; i++)
        {
            if((i + 1) % gridSize != 0)
            {
                createLink(i, i + 1);
            }
            if(i + gridSize < nodeCount)
            {
                createLink(i, i + gridSize);
            }
        }
    }

    else if(topologyType == UT::TopologyType::RingStar)
    {
        for(int i = 0; i < nodeCount - 1; i++)
        {
            if(i < nodeCount - 1)
            {
                createLink(i, (i + 1) % (nodeCount - 1));
            }
            if(i % 2 == 0)
            {
                createLink(i, nodeCount - 1);
            }
        }
    }

    else if(topologyType == UT::TopologyType::Torus)
    {
        for(int i = 0; i < nodeCount; i++)
        {
            if((i + 1) % gridSize != 0)
            {
                createLink(i, i + 1);
            }
            else
            {
                createLink(i, i - (gridSize - 1));
            }
            if(i + gridSize < nodeCount)
            {
                createLink(i, i + gridSize);
            }
            else
            {
                createLink(i, (i + gridSize) % nodeCount);
            }
        }
    }
}

void
AutonomousSystem::convertMeshToTorus()
{
    if(topologyType != UT::TopologyType::Mesh)
    {
        throw std::runtime_error("A non-mesh autonomous system cannot be converted into a torus");
    }

    int gridSize = std::sqrt(nodeCount);
    for(int i = 0; i < nodeCount; i++)
    {
        if((i + 1) % gridSize == 0)
        {
            createLink(i, i - (gridSize - 1));
        }

        if(i + gridSize >= nodeCount)
        {
            createLink(i, (i + gridSize) % nodeCount);
        }
    }
}

RouterPtr
AutonomousSystem::getRouter(int routerId)
{
    if(routerId - startId < routers.size())
    {
        return routers[routerId - startId];
    }

    UT_INFO "Router with id " << routerId << " not found in AS " << id;

    return nullptr;
}

bool
AutonomousSystem::is(int id)
{
    return this->id == id;
}

void
AutonomousSystem::runSpanningTreeAlg()
{
    for(auto router : routers)
    {
        router->sendSpanningTreeConfig();
    }
}

void
AutonomousSystem::runDHCP()
{
    for(auto router : routers)
    {
        router->discoverDHCP();
    }
}

void
AutonomousSystem::runOSPF()
{
    for(auto router : routers)
    {
        OSPF::runOSPF(router);
    }
}

void
AutonomousSystem::runBGP()
{
    for(auto router : routers)
    {
        BGP::runBGP(router);
    }
}

QVector<RouterPtr>
AutonomousSystem::getRouters()
{
    return routers;
}
