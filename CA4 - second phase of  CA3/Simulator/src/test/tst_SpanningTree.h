#ifndef TST_SPANNINGTREE_H
#define TST_SPANNINGTREE_H
#include <QTest>

#include "../main/Network/Network.h"

class SpanningTreeTest : public QObject
{
    Q_OBJECT

private:
    void runSpanningTreeAlg();
    void convertToTorus();
    int getASId(int routerId);
    void assertBroadcasts(int selfId, int neighborId);
    void assertNotBroadcasts(int selfId, int neighborId);
    void assertBroadcasts(int selfId, std::list<int> neighborIds);
    void assertNotBroadcasts(int selfId, std::list<int> neighborIds);

    NetworkPtr network;
    bool torus;

private Q_SLOTS:
    void initTestCase();
    void mesh_TopLeftCorner_BroadcastsToAll();
    void mesh_TopRightCorner_BroadcastsToAll();
    void mesh_BottomLeftCorner_NotBroadcastsToRight();
    void mesh_BottomRightCorner_NotBroadcastsToLeft();
    void mesh_TopEdge_BroadcastsToAll();
    void mesh_BottomEdge_NotBroadcastsToSides();
    void mesh_LeftEdge_NotBroadcastsToRight();
    void mesh_RightEdge_NotBroadcastsToLeft();
    void mesh_Middle_NotBroadcastsToSides();

    void torus_TopLeftCorner_BroadcastsToAll();
    void torus_TopRightCorner_NotBroadcastsToLeft();
    void torus_BottomLeftCorner_OnlyBroadcastsToBottom();
    void torus_BottomRightCorner_OnlyBroadcastsToBottom();
    void torus_TopEdgeLeftNode_BroadcastsToAll();
    void torus_TopEdgeRightNode_NotBroadcastsToRight();
    void torus_BottomEdge_OnlyBroadcastsToBottom();
    void torus_LeftEdgeTopNode_NotBroadcastsToSides();
    void torus_LeftEdgeBottomNode_OnlyBroadcastsToTop();
    void torus_RightEdgeTopNode_NotBroadcastsToSides();
    void torus_RightEdgeBottomNode_OnlyBroadcastsToTop();
    void torus_MiddleTopNode_NotBroadcastsToSides();
    void torus_MiddleBottomNode_OnlyBroadcastsToTop();

    void ringStar_FirstNode_BroadcastsToAll();
    void ringStar_SecondNode_BroadcastsToAll();
    void ringStar_ThirdNode_NotBroadcastToCenter();
    void ringStar_FourthNode_OnlyBroadcastToRight();
    void ringStar_FifthNode_OnlyBroadcastToLeft();
    void ringStar_SixthNode_BroadcastToAll();
    void ringStar_LastNode_OnlyBroadcastToRoot();

    void differentAutonomousSystems_NotBroadcastToEachOther();
    void routers_AnySituation_BroadcastToPCs();
};

#endif
