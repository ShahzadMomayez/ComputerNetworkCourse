#ifndef EVENTCOORDINATOR_H
#define EVENTCOORDINATOR_H

#include "../DataGenerator/DataGenerator.h"
#include "../Globals/Globals.h"

#include <iostream>

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QVector>

class EventCoordinator : public QObject
{
    Q_OBJECT

public:
    enum class SimulationPhase : uint8_t
    {
        SpanningTree,
        DHCP,
        RoutingProtocols,
        BGP,
        Simulation,
        END
    };

public:
    static EventCoordinator *instance(QObject *parent = nullptr);
    static void              release();
    void                     stopSimulation();

    void                     showProgressBar(int percentage);

    SimulationPhase          currentPhase() const;
    void                     setCurrentPhase(SimulationPhase newCurrentPhase);

Q_SIGNALS:
    void nextTick(const int &packetCount);
    void startSimulation(int intervalDuration, int simulationDuration,
                         bool sendPacketCount = false);

private:
    explicit EventCoordinator(QObject *parent = nullptr);
    ~EventCoordinator()                    = default;

    inline static EventCoordinator *self   = nullptr;
    inline static QThread          *thread = new QThread();
    QTimer                         *timer;
    DataGenerator                  *dataGenerator;
    int                             simulationDuration;
    int                             elapsedTime;
    bool                            sendPacketCount;

    SimulationPhase                 m_currentPhase;

private Q_SLOTS:
    void onStartSimulation(int intervalDuration, int simulationDuration, bool sendPacketCount);
    void onTimeout();
};

#endif
