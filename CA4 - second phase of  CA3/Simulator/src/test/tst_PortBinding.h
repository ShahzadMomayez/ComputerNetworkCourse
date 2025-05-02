#ifndef TST_PORTBINDING_H
#define TST_PORTBINDING_H

#include <QTest>
#include "../main/Network/Network.h"
#include <QSignalSpy>

class PortBindingTest : public QObject
{
    Q_OBJECT

private:
    void createRoutersSpy();
    void createPCsSpy();
    int getASId(int routerId);
    void broadcast(int routerId);
    void broadcastAbroad(int routerId);
    void assertEmits(int routerId);
    void assertNotEmits(int routerId);
    void assertNotEmitsExcept(int ASId, std::list<int> routerIds);
    void assertNotEmitsExcept(std::list<int> PCIds);

    NetworkPtr network;
    QMap<int, QMap<int, QSignalSpy*>> routersSpy;
    QMap<int, QSignalSpy*> PCsSpy;

private Q_SLOTS:
    void init();
    void mesh_TopLeftCorner_ConnectedToNeighbors();
    void mesh_TopRightCorner_ConnectedToNeighbors();
    void mesh_BottomLeftCorner_ConnectedToNeighbors();
    void mesh_BottomRightCorner_ConnectedToNeighbors();
    void mesh_TopEdge_ConnectedToNeighbors();
    void mesh_BottomEdge_ConnectedToNeighbors();
    void mesh_LeftEdge_ConnectedToNeighbors();
    void mesh_RightEdge_ConnectedToNeighbors();
    void mesh_Middle_ConnectedToNeighbors();

    void torus_TopLeftCorner_ConnectedToNeighbors();
    void torus_TopRightCorner_ConnectedToNeighbors();
    void torus_BottomLeftCorner_ConnectedToNeighbors();
    void torus_BottomRightCorner_ConnectedToNeighbors();
    void torus_TopEdge_ConnectedToNeighbors();
    void torus_BottomEdge_ConnectedToNeighbors();
    void torus_LeftEdge_ConnectedToNeighbors();
    void torus_RightEdge_ConnectedToNeighbors();
    void torus_Middle_ConnectedToNeighbors();

    void ringStar_Begin_ConnectedToNeighbors();
    void ringStar_End_ConnectedToNeighbors();
    void ringStar_Center_ConnectedToNeighbors();

    void diffrentAutonomousSystems_Connected_ConnectedToEachOther();
    void diffrentAutonomousSystems_NotConnected_NotConnectedToEachOther();

    void mesh_FirstConfig_ConnectedToPCs();
    void mesh_LastConfig_ConnectedToPCs();

    void ringStar_FirstConfig_ConnectedToPCs();
    void ringStar_LastConfig_ConnectedToPCs();

    void pc_ConnectedToRouter();

};

#endif
