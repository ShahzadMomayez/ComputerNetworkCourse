#include "tst_SpanningTree.h"

#define INTERVAL_DURATION 2
#define NUM_TICKS_FIRST_RUN 15
#define NUM_TICKS_SECOND_RUN 5

void SpanningTreeTest::initTestCase() {
    torus = false;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    runSpanningTreeAlg();
}

void SpanningTreeTest::runSpanningTreeAlg() {
    network->runSpanningTreeAlg();
    for (int i = 0; i < NUM_TICKS_FIRST_RUN; i++) {
        QThread::msleep(INTERVAL_DURATION);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
    network->runSpanningTreeAlg();
    for (int i =0; i < NUM_TICKS_SECOND_RUN; i++) {
        QThread::msleep(INTERVAL_DURATION);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
}

void SpanningTreeTest::convertToTorus() {
    if (torus) {
        return;
    }
    torus = true;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    network->getAutonomousSystem(1)->convertMeshToTorus();
    runSpanningTreeAlg();
}


int SpanningTreeTest::getASId(int routerId) {
    if (routerId < 17) {
        return 1;
    }
    return 2;
}

void SpanningTreeTest::assertBroadcasts(int selfId, int neighborId) {
    int ASId = getASId(selfId);
    bool actual = network->getAutonomousSystem(ASId)->getRouter(selfId)->findInterface(neighborId)->shouldBroadcast();
    QCOMPARE(actual, true);
}

void SpanningTreeTest::assertNotBroadcasts(int selfId, int neighborId) {
    int ASId = getASId(selfId);
    bool actual = network->getAutonomousSystem(ASId)->getRouter(selfId)->findInterface(neighborId)->shouldBroadcast();
    QCOMPARE(actual, false);
}

void SpanningTreeTest::assertBroadcasts(int selfId, std::list<int> neighborIds) {
    for (int id : neighborIds) {
        assertBroadcasts(selfId, id);
    }
}

void SpanningTreeTest::assertNotBroadcasts(int selfId, std::list<int> neighborIds) {
    for (int id : neighborIds) {
        assertNotBroadcasts(selfId, id);
    }
}

void SpanningTreeTest::mesh_TopLeftCorner_BroadcastsToAll() {
    assertBroadcasts(1, {2, 5});
}

void SpanningTreeTest::mesh_TopRightCorner_BroadcastsToAll() {
    assertBroadcasts(4, {3, 8});
}

void SpanningTreeTest::mesh_BottomLeftCorner_NotBroadcastsToRight() {
    assertBroadcasts(13, {9});
    assertNotBroadcasts(13, {14});
}

void SpanningTreeTest::mesh_BottomRightCorner_NotBroadcastsToLeft() {
    assertBroadcasts(16, {12});
    assertNotBroadcasts(16, {15});
}

void SpanningTreeTest::mesh_TopEdge_BroadcastsToAll() {
    assertBroadcasts(2, {1, 3, 6});

    assertBroadcasts(3, {2, 4, 7});
}

void SpanningTreeTest::mesh_BottomEdge_NotBroadcastsToSides() {
    assertBroadcasts(14, {10});
    assertNotBroadcasts(14, {13, 15});

    assertBroadcasts(15, {11});
    assertNotBroadcasts(15, {14, 16});
}

void SpanningTreeTest::mesh_LeftEdge_NotBroadcastsToRight() {
    assertBroadcasts(5, {1, 9});
    assertNotBroadcasts(5, {6});

    assertBroadcasts(9, {5, 13});
    assertNotBroadcasts(9, {10});
}

void SpanningTreeTest::mesh_RightEdge_NotBroadcastsToLeft() {
    assertBroadcasts(8, {4, 12});
    assertNotBroadcasts(8, {7});

    assertBroadcasts(12, {8, 16});
    assertNotBroadcasts(12, {11});
}

void SpanningTreeTest::mesh_Middle_NotBroadcastsToSides() {
    assertBroadcasts(6, {2, 10});
    assertNotBroadcasts(6, {5, 7});

    assertBroadcasts(7, {3, 11});
    assertNotBroadcasts(7, {6, 8});

    assertBroadcasts(10, {6, 14});
    assertNotBroadcasts(10, {9, 11});

    assertBroadcasts(11, {7, 15});
    assertNotBroadcasts(11, {10, 12});
}


void SpanningTreeTest::torus_TopLeftCorner_BroadcastsToAll() {
    convertToTorus();

    assertBroadcasts(1, {2, 4, 5, 13});
}

void SpanningTreeTest::torus_TopRightCorner_NotBroadcastsToLeft() {
    convertToTorus();

    assertBroadcasts(4, {1, 8, 16});
    assertNotBroadcasts(4, {3});
}

void SpanningTreeTest::torus_BottomLeftCorner_OnlyBroadcastsToBottom() {
    convertToTorus();

    assertBroadcasts(13, {1});
    assertNotBroadcasts(13, {9, 14, 16});
}

void SpanningTreeTest::torus_BottomRightCorner_OnlyBroadcastsToBottom() {
    convertToTorus();

    assertBroadcasts(16, {4});
    assertNotBroadcasts(16, {12, 13, 15});
}

void SpanningTreeTest::torus_TopEdgeLeftNode_BroadcastsToAll() {
    convertToTorus();

    assertBroadcasts(2, {1, 3, 6, 14});
}

void SpanningTreeTest::torus_TopEdgeRightNode_NotBroadcastsToRight() {
    convertToTorus();

    assertBroadcasts(3, {2, 7, 15});

    assertNotBroadcasts(3, {4});
}

void SpanningTreeTest::torus_BottomEdge_OnlyBroadcastsToBottom() {
    convertToTorus();

    assertBroadcasts(14, {2});
    assertNotBroadcasts(14, {10, 13, 15});

    assertBroadcasts(15, {3});
    assertNotBroadcasts(15, {11, 14, 16});
}

void SpanningTreeTest::torus_LeftEdgeTopNode_NotBroadcastsToSides() {
    convertToTorus();

    assertBroadcasts(5, {1, 9});
    assertNotBroadcasts(5, {6, 8});
}

void SpanningTreeTest::torus_LeftEdgeBottomNode_OnlyBroadcastsToTop() {
    convertToTorus();

    assertBroadcasts(9, {5});
    assertNotBroadcasts(9, {10, 12, 13});
}

void SpanningTreeTest::torus_RightEdgeTopNode_NotBroadcastsToSides() {
    convertToTorus();

    assertBroadcasts(8, {4, 12});
    assertNotBroadcasts(8, {7, 5});
}

void SpanningTreeTest::torus_RightEdgeBottomNode_OnlyBroadcastsToTop() {
    convertToTorus();

    assertBroadcasts(12, {8});
    assertNotBroadcasts(12, {11, 9, 16});
}

void SpanningTreeTest::torus_MiddleTopNode_NotBroadcastsToSides() {
    convertToTorus();

    assertBroadcasts(6, {2, 10});
    assertNotBroadcasts(6, {5, 7});

    assertBroadcasts(7, {3, 11});
    assertNotBroadcasts(7, {6, 8});
}

void SpanningTreeTest::torus_MiddleBottomNode_OnlyBroadcastsToTop() {
    convertToTorus();

    assertBroadcasts(10, {6});
    assertNotBroadcasts(10, {9, 11, 14});

    assertBroadcasts(11, {7});
    assertNotBroadcasts(11, {10, 12, 15});
}

void SpanningTreeTest::ringStar_FirstNode_BroadcastsToAll() {
    assertBroadcasts(17, {18, 22, 23});
}

void SpanningTreeTest::ringStar_SecondNode_BroadcastsToAll() {
    assertBroadcasts(18, {17, 19});
}

void SpanningTreeTest::ringStar_ThirdNode_NotBroadcastToCenter() {
    assertBroadcasts(19, {18, 20});
    assertNotBroadcasts(19, {23});
}

void SpanningTreeTest::ringStar_FourthNode_OnlyBroadcastToRight() {
    assertBroadcasts(20, {19});
    assertNotBroadcasts(20, {21});
}

void SpanningTreeTest::ringStar_FifthNode_OnlyBroadcastToLeft() {
    assertBroadcasts(21, {22});
    assertNotBroadcasts(21, {20, 23});
}

void SpanningTreeTest::ringStar_SixthNode_BroadcastToAll() {
    assertBroadcasts(22, {17, 21});
}

void SpanningTreeTest::ringStar_LastNode_OnlyBroadcastToRoot() {
    assertBroadcasts(23, {17});
    assertNotBroadcasts(23, {19, 21});
}

void SpanningTreeTest::differentAutonomousSystems_NotBroadcastToEachOther() {
    assertNotBroadcasts(14, {17});
    assertNotBroadcasts(15, {18});
    assertNotBroadcasts(16, {19});

    assertNotBroadcasts(17, {14});
    assertNotBroadcasts(18, {15});
    assertNotBroadcasts(19, {16});
}

void SpanningTreeTest::routers_AnySituation_BroadcastToPCs() {
    assertBroadcasts(1, {24, 25});
    assertBroadcasts(2, {26, 27});
    assertBroadcasts(3, {28, 29});
    assertBroadcasts(4, {30, 31});

    assertBroadcasts(20, {34, 35, 36});
    assertBroadcasts(21, {32, 33});
    assertBroadcasts(22, {37, 38});
}

