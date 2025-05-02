#ifndef TST_BGP_H
#define TST_BGP_H

#include "../main/Network/Network.h"

#include <QObject>
#include <QTest>

class BGPTest : public QObject
{
    Q_OBJECT
private:
    void convertToTorus();
    void runBGP();
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

    void ringStarConnectedToMesh_CentralNode_ForwardingTableIsCorrect();
    void ringStarConnectedToMesh_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToMesh_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToMesh_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToMesh_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();

    void torus_TopLeftCorner_ForwardingTableIsCorrect();
    void torus_TopRightCorner_ForwardingTableIsCorrect();
    void torus_BottomLeftCorner_ForwardingTableIsCorrect();
    void torus_BottomRightCorner_ForwardingTableIsCorrect();
    void torus_TopEdge_ForwardingTableIsCorrect();
    void torus_BottomEdge_ForwardingTableIsCorrect();
    void torus_LeftEdge_ForwardingTableIsCorrect();
    void torus_RightEdge_ForwardingTableIsCorrect();
    void torus_Middle_ForwardingTableIsCorrect();

    void ringStarConnectedToTorus_CentralNode_ForwardingTableIsCorrect();
    void ringStarConnectedToTorus_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToTorus_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToTorus_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect();
    void ringStarConnectedToTorus_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect();

};

#endif    
