#ifndef PC_H
#define PC_H

#include "../EventCoordinator/EventCoordinator.h"
#include "../Globals/Types.h"
#include "../IPDatabase/IPDatabase.h"
#include "../TCP/TCP.h"
#include "Node.h"

#include <QDebug>
#include <QObject>

class PC : public Node
{
    Q_OBJECT

public:
    explicit PC(int id, QString network, QObject *parent = nullptr);

    virtual void broadcast(PacketPtr packet, NetworkInterfacePtr incommingInterface = nullptr);

    void         addPort(PortPtr port);
    bool         is(int id);
    void         sendPacket(PacketPtr data);
    void         setIpDB(IPDatabasePtr ipDatabase);

    int          getTotalPacketsSent() const;
    int          getTotalPacketsReceived() const;
    double       calculatePacketLoss() const;
    const QVector<int> &getHopCounts() const;
    QVector<int>        getWaitingCycles();


public:
Q_SIGNALS:
    void packetReceived(const PacketPtr data);

private
    Q_SLOT : void onPacketReceived(const PacketPtr data);
    void          onNextTick(const int &packetCount);

private:
    void                sendPacketTo(int destinationId);
    void                dataPacketReceived(PacketPtr packet);

    void                sendPacketFromDataGenerator(const int packetCount);
    void                sendPacketFromChunkServer();
    void                handleDeliveredDataFromTCP(const QString &from, const QByteArray &data);
    void                flushBufferToFile();

    IPDatabasePtr       ipDatabase;
    int                 time;
    PortPtr             port;
    int                 totalPacketsSent     = 0;
    int                 totalPacketsReceived = 0;
    QVector<int>        hopCounts;
    QVector<int>        waitingCycles;
    QVector<QByteArray> buffer;
    TCPProtocol         tcp;
};

typedef QSharedPointer<PC> PCPtr;



#endif
