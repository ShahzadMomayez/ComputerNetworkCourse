#include "ApplicationContext.h"

#include <iostream>

#include <QDebug>
#include <QDir>
#include <QFileInfo>

#define SPANNING_TREE_TICKS     15
#define DHCP_TICKS              300
#define ROUTING_ALGORITHM_TICKS 25
#define BGP_TICKS               15
#define CONFIGURATION_TICKS     (SPANNING_TREE_TICKS + DHCP_TICKS + ROUTING_ALGORITHM_TICKS + BGP_TICKS)

#define PACKET_LOSS             QString("Packet-loss").toLower()
#define HOP_COUNT_AVG           QString("Hop-count-avg").toLower()
#define WAITING_CYCLES_STAT     QString("Waiting-cycles-stat").toLower()
#define USED_ROUTERS            QString("Used-routers").toLower()
#define POOR_ROUTERS            QString("Poor-routers").toLower()
#define PRINT_ROUTING_TABLE     QString("Print-routing-table").toLower()
#define RESET                   QString("Reset").toLower()
#define EXIT                    QString("Exit").toLower()
#define CLEAN                   QString("Clean").toLower()

#define MESH                    QString("Mesh").toLower()
#define TORUS                   QString("Torus").toLower()
#define RIP_STRING              QString("RIP").toLower()
#define OSPF_STRING             QString("OSPF").toLower()
#define YES                     QString("Yes").toLower()
#define NO                      QString("No").toLower()

ApplicationContext::ApplicationContext(QString configPath, QObject* parent) :
    QObject(parent),
    configPath(configPath),
    config(ConfigParser::instance(configPath)),
    eventCoordinator(EventCoordinator::instance())
{
    this->network = Network::instance(configPath);
}

void
ApplicationContext::run()
{
    setUserConfig();
    setupNetwork();
    startSimulation();
    handleCommands();
}

void
ApplicationContext::setUserConfig()
{
    setTopologyType();
    setRoutingProtocol();
    setBrokenRoutersStatus();
}

void
ApplicationContext::setTopologyType()
{
    while(true)
    {
        qInfo().noquote().nospace() << "Select topology (" << MESH << "/" << TORUS << "):";

        QString input = getInput()[0];

        if(input == MESH)
        {
            topologyType = UT::TopologyType::Mesh;
            break;
        }
        else if(input == TORUS)
        {
            topologyType = UT::TopologyType::Torus;
            break;
        }
        else
        {
            qInfo() << "Error: Invalid topology type.";
        }
    }
}

void
ApplicationContext::setRoutingProtocol()
{
    while(true)
    {
        qInfo().noquote().nospace()
          << "Select routing protocol (" << RIP_STRING << "/" << OSPF_STRING << "):";

        QString input = getInput()[0];

        if(input == RIP_STRING)
        {
            routingProtocol = UT::RoutingProtocolType::RIP;
            break;
        }
        else if(input == OSPF_STRING)
        {
            routingProtocol = UT::RoutingProtocolType::OSPF;
            break;
        }
        else
        {
            qInfo() << "Error: Invalid routing protocol.";
        }
    }
}

void
ApplicationContext::setBrokenRoutersStatus()
{
    while(true)
    {
        qInfo().noquote().nospace() << "Break routers (" << YES << "/" << NO << "):";
        QString input = getInput()[0];

        if(input == YES)
        {
            brokenRouters = true;
            break;
        }
        else if(input == NO)
        {
            brokenRouters = false;
            break;
        }
        else
        {
            qInfo() << "Error: Invalid input.";
        }
    }
}

void
ApplicationContext::setupNetwork()
{
    network->create();

    if(topologyType == UT::TopologyType::Torus)
    {
        network->getAutonomousSystem(1)->convertMeshToTorus();
    }
    if(brokenRouters)
    {
        network->breakRouters();
    }

    qDebug() << "\n";
    UT_INFO "Setting up the network...\n\n";

    startEventCoordinator(CONFIGURATION_TICKS * config->getCycleDuration());

    runSpanningTreeAlg();

    runDHCP();

    runRoutingProtocol();

    runBGP();
}

void
ApplicationContext::runSpanningTreeAlg()
{
    QThread::msleep(100);
    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::SpanningTree);

    network->runSpanningTreeAlg();
    QThread::msleep(SPANNING_TREE_TICKS * config->getCycleDuration());

    network->runSpanningTreeAlg();
    QThread::msleep(SPANNING_TREE_TICKS * config->getCycleDuration());

    // eventCoordinator->showProgressBar(100);
    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::END);
}

void
ApplicationContext::runDHCP()
{
    QThread::msleep(1'000);
    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::DHCP);

    // qDebug() << UT_TerminalColorGreen << "DHCP is running:" << UT_TerminalColorReset;

    network->runDHCP();

    QThread::msleep(DHCP_TICKS * config->getCycleDuration());

    network->setInterfacesIPs();

    // eventCoordinator->showProgressBar(100);


    qDebug() << UT_TerminalColorGreen << "DHCP has completed.\n" << UT_TerminalColorReset;
    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::END);
}

void
ApplicationContext::runRoutingProtocol()
{
    QThread::msleep(1'000);
    // eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::RoutingProtocols);

    qDebug() << UT_TerminalColorYellow << "RoutingProtocols is running:" << UT_TerminalColorReset;

    if(routingProtocol == UT::RoutingProtocolType::RIP)
    {
        network->runRIP();
    }
    else
    {
        network->runOSPF();
    }

    QThread::msleep(ROUTING_ALGORITHM_TICKS * config->getCycleDuration());


    // if(routingProtocol == UT::RoutingProtocolType::OSPF)
    // {
    //     network->cleanupOSPFMemory();
    // }

    eventCoordinator->showProgressBar(100);

    qDebug() << UT_TerminalColorGreen << "RoutingProtocols has completed.\n"
             << UT_TerminalColorReset;

    // eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::END);

    // if(routingProtocol == UT::RoutingProtocolType::RIP)
    // {
    //     UT_INFO "RIP has completed.";
    // }
    // else
    // {
    //     UT_INFO "OSPF has completed.";
    // }
}

void
ApplicationContext::runBGP()
{
    // UT_INFO "INVOKED!";
    QThread::msleep(1'000);
    qDebug() << UT_TerminalColorYellow << "BGP is running:" << UT_TerminalColorReset;
    // eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::BGP);
    network->runBGP();
    eventCoordinator->showProgressBar(100);
    QThread::msleep(BGP_TICKS * config->getCycleDuration());
    qDebug() << UT_TerminalColorGreen << "BGP has completed.\n" << UT_TerminalColorReset;
    // eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::END);
}

void
ApplicationContext::startSimulation()
{
    QThread::msleep(1'000);
    int simulationDuration = config->getSimulationDuration();
    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::Simulation);

    startEventCoordinator(simulationDuration, true);
    QThread::msleep(simulationDuration);

    startEventCoordinator(10 * simulationDuration);
    QThread::msleep(10 * simulationDuration);

    eventCoordinator->setCurrentPhase(EventCoordinator::SimulationPhase::END);

    QThread::msleep(2'000);

    qDebug() << "\n";
    UT_SUCCESS "All Phases have completed!";
    qDebug() << "\n";

    // UT_INFO "Simulation has completed.";
}


void
ApplicationContext::handleCommands()
{
    UT_INFO "Enter command:";

    bool exit = false;

    while(!exit)
    {
        QStringList command = getInput();
        exit                = handleCommand(command[0], command.mid(1));
    }
}

void
ApplicationContext::startEventCoordinator(int simulationDuration, bool sendPacketCount)
{
    QThread::msleep(100);
    Q_EMIT eventCoordinator->startSimulation(config->getCycleDuration(), simulationDuration,
                                             sendPacketCount);
}

bool
ApplicationContext::handleCommand(QString command, QStringList args)
{
    if(command == PACKET_LOSS)
    {
        handlePacketLossCommand();
    }
    else if(command == HOP_COUNT_AVG)
    {
        handleHopCountAvgCommand();
    }
    else if(command == WAITING_CYCLES_STAT)
    {
        handleWaitingCyclesStatCommand();
    }
    else if(command == USED_ROUTERS)
    {
        handlleUsedRoutersCommand();
    }
    else if(command == POOR_ROUTERS)
    {
        handlePoorRoutersCommand();
    }
    else if(command == PRINT_ROUTING_TABLE)
    {
        handlePrintRoutingTable(args);
    }
    else if(command == RESET)
    {
        handleResetCommand();
        return true;
    }
    else if(command == EXIT)
    {
        return true;
    }
    else if(command == CLEAN)
    {
        handleCleanCommand();
    }
    else
    {
        qInfo() << "Invalid Command.";
    }

    return false;
}

void
ApplicationContext::handlePacketLossCommand()
{
    QVector<PCPtr> pcs                  = network->getPCs();
    int            totalPacketsSent     = 0;
    int            totalPacketsReceived = 0;

    for(const auto& pc : pcs)
    {
        totalPacketsSent     += pc->getTotalPacketsSent();
        totalPacketsReceived += pc->getTotalPacketsReceived();
    }

    if(totalPacketsSent == 0)
    {
        qInfo() << "No packets sent.";
        return;
    }

    int    lostPackets          = totalPacketsSent - totalPacketsReceived;
    double packetLossPercentage = (static_cast<double>(lostPackets) / totalPacketsSent) * 100.0;

    qInfo() << "Total Packets Sent:" << totalPacketsSent;
    qInfo() << "Total Packets Received:" << totalPacketsReceived;
    qInfo() << "Packet Loss Percentage:" << packetLossPercentage << "%";
}

void
ApplicationContext::handleHopCountAvgCommand()
{
    QVector<PCPtr> pcs          = network->getPCs();
    int            totalPackets = 0;
    double         totalHops    = 0.0;

    for(const auto& pc : pcs)
    {
        const QVector<int>& hopCounts  = pc->getHopCounts();
        totalHops                     += std::accumulate(hopCounts.begin(), hopCounts.end(), 0.0);
        totalPackets                  += hopCounts.size();
    }

    if(totalPackets == 0)
    {
        qInfo() << "No packets received.";
        return;
    }
    double averageHopCount = totalHops / totalPackets;

    qInfo() << "Total Packets Received:" << totalPackets;
    qInfo() << "Total Hops:" << totalHops;
    qInfo() << "Average Hop Count:" << averageHopCount;
}

void
ApplicationContext::handleWaitingCyclesStatCommand()
{
    QVector<PCPtr> pcs = network->getPCs();

    QVector<int>   allWaitingCycles;

    for(auto pc : pcs)
    {
        allWaitingCycles.append(pc->getWaitingCycles());
    }

    int    totalPackets       = allWaitingCycles.size();
    int totalWaitingCycles = std::accumulate(allWaitingCycles.begin(), allWaitingCycles.end(), 0.0);
    double averageWaitingCycles = (double)totalWaitingCycles / totalPackets;
    int minimumWaitingCycles = *std::min_element(allWaitingCycles.begin(), allWaitingCycles.end());
    int maximumWaitingCycles = *std::max_element(allWaitingCycles.begin(), allWaitingCycles.end());

    qInfo() << "Total Packets Received:" << totalPackets;
    qInfo() << "Total Waiting Cycles:" << totalWaitingCycles;
    qInfo() << "Average Waiting Cycles:" << averageWaitingCycles;
    qInfo() << "Minimum Waiting Cycles:" << minimumWaitingCycles;
    qInfo() << "Maximum Waiting Cycles:" << maximumWaitingCycles;
}

void
ApplicationContext::handlePrintRoutingTable(QStringList args)
{
    if(args.isEmpty())
    {
        qInfo() << "Error: Router ID required.";
        return;
    }

    int       routerId = args[0].toInt();

    RouterPtr router   = network->getRouter(routerId);

    if(router)
    {
        qInfo().noquote() << router->getForwardingTable()->show();
    }
    else
    {
        qInfo() << "Error: Router ID not valid.";
    }
}

void
ApplicationContext::handleResetCommand()
{
    EventCoordinator::release();
    eventCoordinator = EventCoordinator::instance();

    EventCoordinator::release();
    network = Network::instance(configPath);

    qInfo() << "Simulation has been reset to the starting point.";

    run();
}

QVector<int>
ApplicationContext::listUsedRouters()
{
    QVector<int>       usedRouters;
    QVector<RouterPtr> routers = network->getRouters();
    for(const auto& router : routers)
    {
        if(router->isUsed())
        {
            usedRouters.append(router->getId());
        }
    }
    return usedRouters;
}

void
ApplicationContext::handlePoorRoutersCommand()
{
    QVector<int>       poorRouters;
    QVector<RouterPtr> routers = network->getRouters();
    if(routers.isEmpty())
    {
        qInfo() << "No routers found";
        return;
    }

    int maxPacketsRouted = 0;
    for(const auto& router : routers)
    {
        int packetsRouted = router->getTotalPacketsReceived();
        if(packetsRouted > maxPacketsRouted)
        {
            maxPacketsRouted = packetsRouted;
        }
    }

    for(const auto& router : routers)
    {
        if(router->getTotalPacketsReceived() == maxPacketsRouted)
        {
            poorRouters.append(router->getId());
        }
    }

    qInfo() << "Max Packets Routed:" << maxPacketsRouted;
    qInfo().noquote() << "Poor Routers:" << vectorToString(poorRouters);
}

void
ApplicationContext::handlleUsedRoutersCommand()
{
    QVector<int> usedRouters = listUsedRouters();
    qInfo().noquote() << "Used Routers:" << vectorToString(usedRouters);
}

QString
ApplicationContext::vectorToString(QVector<int> vector)
{
    QStringList stringList;

    for(int value : vector)
    {
        stringList << QString::number(value);
    }

    return stringList.join(", ");
}

void
ApplicationContext::handleCleanCommand()
{
    removeLogFiles();
    qInfo() << "Log files have been removed.";
}

void
ApplicationContext::removeLogFiles()
{
    QDir dir("../../logs");

    if(dir.exists())
    {
        QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for(const QString& file : files)
        {
            dir.remove(file);
        }
    }
    else
    {
        qInfo() << "Error: Directory \"logs\" does not exist.";
    }
}

QStringList
ApplicationContext::getInput()
{
    std::string input;

    std::getline(std::cin, input);

    auto        line     = QString::fromStdString(input).toLower();

    QStringList wordList = line.split(' ', Qt::SkipEmptyParts);

    return wordList;
}

