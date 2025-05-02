#ifndef TST_DHCPSERVER_H
#define TST_DHCPSERVER_H

#include <QObject>
#include "../main/Network/Network.h"

class DHCPServerTest : public QObject
{
    Q_OBJECT
private:
    void runSpanningTreeAlg();
    void runDHCP();
    void convertToTorus();
    void assertRouterIP(int asId, int nodeId, QString expected);
    void assertPcIP(int pcId, QString expected);
    void assertMACAddressExists(int asId, int routerId);
    void assertMACAddressExists(int pcId);

    NetworkPtr network;
    bool torus;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void mesh_TopLeftCorner_AssignedCorrectIp();
    void mesh_TopRightCorner_AssignedCorrectIp();
    void mesh_BottomLeftCorner_AssignedCorrectIp();
    void mesh_BottomRightCorner_AssignedCorrectIp();
    void mesh_TopEdge_AssignedCorrectIp();
    void mesh_BottomEdge_AssignedCorrectIp();
    void mesh_LeftEdge_AssignedCorrectIp();
    void mesh_RightEdge_AssignedCorrectIp();
    void mesh_Middle_AssignedCorrectIp();
    void mesh_PC_AssignedCorrectIp();

    void mesh_TopLeftCorner_AssignedMACAddress();
    void mesh_TopRightCorner_AssignedMACAddress();
    void mesh_BottomLeftCorner_AssignedMACAddress();
    void mesh_BottomRightCorner_AssignedMACAddress();
    void mesh_TopEdge_AssignedMACAddress();
    void mesh_BottomEdge_AssignedMACAddress();
    void mesh_LeftEdge_AssignedMACAddress();
    void mesh_RightEdge_AssignedMACAddress();
    void mesh_Middle_AssignedMACAddress();
    void mesh_PC_AssignedMACAddress();

    void torus_TopLeftCorner_AssignedCorrectIp();
    void torus_TopRightCorner_AssignedCorrectIp();
    void torus_BottomLeftCorner_AssignedCorrectIp();
    void torus_BottomRightCorner_AssignedCorrectIp();
    void torus_TopEdge_AssignedCorrectIp();
    void torus_BottomEdge_AssignedCorrectIp();
    void torus_LeftEdge_AssignedCorrectIp();
    void torus_RightEdge_AssignedCorrectIp();
    void torus_Middle_AssignedCorrectIp();
    void torus_PC_AssignedCorrectIp();

    void torus_TopLeftCorner_AssignedMACAddress();
    void torus_TopRightCorner_AssignedMACAddress();
    void torus_BottomLeftCorner_AssignedMACAddress();
    void torus_BottomRightCorner_AssignedMACAddress();
    void torus_TopEdge_AssignedMACAddress();
    void torus_BottomEdge_AssignedMACAddress();
    void torus_LeftEdge_AssignedMACAddress();
    void torus_RightEdge_AssignedMACAddress();
    void torus_Middle_AssignedMACAddress();
    void torus_PC_AssignedMACAddress();

    void ringStar_CentralNode_AssignedCorrectIp();
    void ringStar_SpokeNode_AssignedCorrectIp();
    void ringStar_NonSpokeNode_AssignedCorrectIp();
    void ringStar_PC_AssignedCorrectIp();

    void ringStar_CentralNode_AssignedMACAddress();
    void ringStar_SpokeNode_AssignedMACAddress();
    void ringStar_NonSpokeNode_AssignedMACAddress();
    void ringStar_PC_AssignedMACAddress();

};

#endif

