#ifndef TST_OSPF_H
#define TST_OSPF_H

#include <QObject>
#include <QTest>
#include "../main/Network/Network.h"

class OSPFTest : public QObject
{
    Q_OBJECT

private:
    void convertToTorus();
    void runOSPF();
    int getASId(int routerId);
    void assertExists(int routerId, int pcId, std::list<int> gatewayIds);
    
    NetworkPtr network;
    bool torus;
private Q_SLOTS:
    void initTestCase();
    void mesh_TopLeftCorner_ForwardingTableIsCorrect();
    void mesh_TopRightCorner_ForwardingTableIsCorrect();
    void mesh_BottomLeftCorner_ForwardingTableIsCorrect();
    void mesh_BottomRightCorner_ForwardingTableIsCorrect();
    void mesh_TopEdge_ForwardingTableIsCorrect();
    void mesh_BottomEdge_ForwardingTableIsCorrect();
    void mesh_LeftEdge_ForwardingTableIsCorrect();
    void mesh_RightEdge_ForwardingTableIsCorrect();
    void mesh_Middle_ForwardingTableIsCorrect();

    void torus_TopLeftCorner_ForwardingTableIsCorrect();
    void torus_TopRightCorner_ForwardingTableIsCorrect();
    void torus_BottomLeftCorner_ForwardingTableIsCorrect();
    void torus_BottomRightCorner_ForwardingTableIsCorrect();
    void torus_TopEdge_ForwardingTableIsCorrect();
    void torus_BottomEdge_ForwardingTableIsCorrect();
    void torus_LeftEdge_ForwardingTableIsCorrect();
    void torus_RightEdge_ForwardingTableIsCorrect();
    void torus_Middle_ForwardingTableIsCorrect();

    void ringStar_CentralNode_ForwardingTableIsCorrect();
    void ringStar_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStar_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStar_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStar_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();

};

#endif 
