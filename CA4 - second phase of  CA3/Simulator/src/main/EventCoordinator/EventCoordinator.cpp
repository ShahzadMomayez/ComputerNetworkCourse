#include "EventCoordinator.h"
#include <QDebug>
#include <QRandomGenerator>

EventCoordinator::EventCoordinator(QObject *parent) :
    QObject {parent},
    timer(new QTimer()),
    dataGenerator(new DataGenerator()),
    simulationDuration(0),
    elapsedTime(0)
{
    timer->moveToThread(thread);
    QObject::connect(timer, &QTimer::timeout, this, &EventCoordinator::onTimeout);
    QObject::connect(this, &EventCoordinator::startSimulation, this, &EventCoordinator::onStartSimulation);
}

EventCoordinator::SimulationPhase
EventCoordinator::currentPhase() const
{
    return m_currentPhase;
}

void
EventCoordinator::setCurrentPhase(SimulationPhase newCurrentPhase)
{
    m_currentPhase = newCurrentPhase;
}

EventCoordinator* EventCoordinator::instance(QObject *parent) {
    if (!self) {
        self = new EventCoordinator(parent);
        self->moveToThread(thread);
        thread->start();
    }
    return self;
}

void EventCoordinator::release() {
    if(self) {
        delete self;
        self = nullptr;
    }
}

void EventCoordinator::onStartSimulation(int intervalDuration, int simulationDuration, bool sendPacketCount) {
    if (timer->isActive()) {
        return;
    }

    elapsedTime              = 0;
    this->simulationDuration = simulationDuration;
    this->sendPacketCount = sendPacketCount;
    timer->start(intervalDuration);

    // qDebug() << "Simulation started with duration:" << simulationDuration
    //          << "ms and interval:" << intervalDuration << "ms.";
}

void EventCoordinator::stopSimulation() {
    if (timer->isActive()) {
        timer->stop();
    }
}

void
EventCoordinator::showProgressBar(int percentage)
{
    int barWidth = 50;
    std::cout << UT_TerminalColorCyan << "[";
    int pos = barWidth * percentage / 100;

    for(int i = 0; i < barWidth; ++i)
    {
        if(i < pos)
            std::cout << "=";
        else if(i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }

    if(percentage != 100)
    {
        std::cout
          << "] "
          << percentage
          // << QString("Cycle %1 / %2").arg(timeCounter, simulator_execution_period).toStdString()
          << " %\r";

        std::cout.flush();
    }
    else
    {
        std::cout << "] " << percentage << " DONE." << UT_TerminalColorReset;
        qDebug() << "";
    }
}

void EventCoordinator::onTimeout() {
    elapsedTime += timer->interval();

    static SimulationPhase prevPhase             = SimulationPhase::END;
    static bool            SpanningTreeDone      = false;
    static bool            DHCPDone              = false;
    static bool            RoutingProtocolsDone  = false;
    static bool            BGPDone               = false;
    static bool            SimulationDone        = false;

    if(m_currentPhase == SimulationPhase::END)
    {
        switch(prevPhase)
        {
            case SimulationPhase::SpanningTree :
                if(SpanningTreeDone) break;

                SpanningTreeDone = true;
                showProgressBar(100);
                qDebug() << UT_TerminalColorGreen << "Spanning tree algorithm has completed.\n"
                         << UT_TerminalColorReset;
                break;

            case SimulationPhase::DHCP :
                if(DHCPDone) break;

                DHCPDone = true;
                showProgressBar(100);
                qDebug() << UT_TerminalColorGreen << "DHCP has completed.\n"
                         << UT_TerminalColorReset;
                break;

            case SimulationPhase::RoutingProtocols :
                if(RoutingProtocolsDone) break;

                RoutingProtocolsDone = true;
                showProgressBar(100);
                qDebug() << UT_TerminalColorGreen << "RoutingProtocols has completed.\n"
                         << UT_TerminalColorReset;
                break;

            case SimulationPhase::BGP :
                if(BGPDone) break;

                BGPDone = true;
                showProgressBar(100);
                qDebug() << UT_TerminalColorGreen << "BGP has completed.\n"
                         << UT_TerminalColorReset;
                break;

            case SimulationPhase::Simulation :
                if(SimulationDone) break;

                SimulationDone = true;
                showProgressBar(100);
                qDebug() << UT_TerminalColorGreen << "Simulation has completed.\n"
                         << UT_TerminalColorReset;
                break;
        }
    }
    else
    {
        if(m_currentPhase != prevPhase)
        {
            switch(m_currentPhase)
            {
                case SimulationPhase::SpanningTree :
                    qDebug() << UT_TerminalColorYellow
                             << "Spanning tree algorithm is running:" << UT_TerminalColorReset;
                    break;

                case SimulationPhase::DHCP :
                    qDebug() << UT_TerminalColorYellow
                             << "DHCP is running:" << UT_TerminalColorReset;
                    break;

                case SimulationPhase::RoutingProtocols :
                    qDebug() << UT_TerminalColorYellow
                             << "RoutingProtocols is running:" << UT_TerminalColorReset;
                    break;

                case SimulationPhase::BGP :
                    qDebug() << UT_TerminalColorYellow
                             << "BGP is running:" << UT_TerminalColorReset;
                    break;

                case SimulationPhase::Simulation :
                    qDebug() << UT_TerminalColorYellow
                             << "Simulation is running:" << UT_TerminalColorReset;
                    break;
            }
        }

        prevPhase = m_currentPhase;

        // if(m_currentPhase == SimulationPhase::Simulation)
        showProgressBar((elapsedTime * 100) / simulationDuration);
    }

    int packetCount = sendPacketCount ?  dataGenerator->generateNumberOfPacket() : 0; 
    Q_EMIT nextTick(packetCount);

    if (elapsedTime >= simulationDuration) {
        stopSimulation();
    }
}
