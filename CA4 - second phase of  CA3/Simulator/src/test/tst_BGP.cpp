#include "tst_BGP.h"

#include "../main/IP/IPv4.h"

#define INTERVAL_DURATION 2
#define NUM_TICKS 15

void BGPTest::initTestCase() {
    torus = false;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    runBGP();
}

void BGPTest::runBGP() {
    network->runRIP();
    for (int i = 0; i < NUM_TICKS; i++) {
        QThread::msleep(INTERVAL_DURATION);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
    network->runBGP();
    for (int i = 0; i < NUM_TICKS; i++) {
        QThread::msleep(INTERVAL_DURATION);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
}

int BGPTest::getASId(int routerId) {
    if (routerId < 17) {
        return 1;
    }
    return 2;
}

void BGPTest::assertExists(int routerId, int pcId, std::list<int> gatewayIds) {
    IPPtr destination = network->getPC(pcId)->getIP();
    IPPtr gateway = network->getAutonomousSystem(getASId(routerId))->getRouter(routerId)->getForwardingTable()->getGatway(destination);
    
    bool assertion = false;
    for (int gatewayId : gatewayIds) {
       if (gateway->toString().endsWith(QString::number(gatewayId))) {
            assertion = true;
            break;
       }
    }
    QCOMPARE(assertion, true);
}

void BGPTest::convertToTorus() {
    if (torus) {
        return;
    }
    torus = true;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    network->getAutonomousSystem(1)->convertMeshToTorus();
    runBGP();
}



void BGPTest::mesh_TopLeftCorner_ForwardingTableIsCorrect() {
    assertExists(1, 32, {2, 5});
    assertExists(1, 33, {2, 5});
    assertExists(1, 34, {2, 5});
    assertExists(1, 35, {2, 5});
    assertExists(1, 36, {2, 5});
    assertExists(1, 37, {2, 5});
    assertExists(1, 38, {2, 5});
}

void BGPTest::mesh_TopRightCorner_ForwardingTableIsCorrect() {
    assertExists(4, 32, {8});
    assertExists(4, 33, {8});
    assertExists(4, 34, {8});
    assertExists(4, 35, {8});
    assertExists(4, 36, {8});
    assertExists(4, 37, {3, 8});
    assertExists(4, 38, {3, 8});
}

void BGPTest::mesh_BottomLeftCorner_ForwardingTableIsCorrect() {
    assertExists(13, 32, {14});
    assertExists(13, 33, {14});
    assertExists(13, 34, {14});
    assertExists(13, 35, {14});
    assertExists(13, 36, {14});
    assertExists(13, 37, {14});
    assertExists(13, 38, {14});
}

void BGPTest::mesh_BottomRightCorner_ForwardingTableIsCorrect() {
    assertExists(16, 32, {19});
    assertExists(16, 33, {19});
    assertExists(16, 34, {19});
    assertExists(16, 35, {19});
    assertExists(16, 36, {19});
    assertExists(16, 37, {15, 19});
    assertExists(16, 38, {15, 19});
}
void BGPTest::mesh_TopEdge_ForwardingTableIsCorrect() {
    assertExists(3, 32, {2, 4, 7});
    assertExists(3, 33, {2, 4, 7});
    assertExists(3, 34, {4, 7});
    assertExists(3, 35, {4, 7});
    assertExists(3, 36, {4, 7});
    assertExists(3, 37, {2, 7});
    assertExists(3, 38, {2, 7});
}

void BGPTest::mesh_BottomEdge_ForwardingTableIsCorrect() {
    assertExists(14, 32, {17});
    assertExists(14, 33, {17});
    assertExists(14, 34, {17, 15});
    assertExists(14, 35, {17, 15});
    assertExists(14, 36, {17, 15});
    assertExists(14, 37, {17});
    assertExists(14, 38, {17});
}

void BGPTest::mesh_LeftEdge_ForwardingTableIsCorrect() {
    assertExists(5, 32, {6, 9});
    assertExists(5, 33, {6, 9});
    assertExists(5, 34, {6, 9});
    assertExists(5, 35, {6, 9});
    assertExists(5, 36, {6, 9});
    assertExists(5, 37, {6, 9});
    assertExists(5, 38, {6, 9});
}

void BGPTest::mesh_RightEdge_ForwardingTableIsCorrect() {
    assertExists(12, 32, {16});
    assertExists(12, 33, {16});
    assertExists(12, 34, {16});
    assertExists(12, 35, {16});
    assertExists(12, 36, {16});
    assertExists(12, 37, {16, 11});
    assertExists(12, 38, {16, 11});
}

void BGPTest::mesh_Middle_ForwardingTableIsCorrect() {
    assertExists(11, 32, {10, 12, 15});
    assertExists(11, 33, {10, 12, 15});
    assertExists(11, 34, {12, 15});
    assertExists(11, 35, {12, 15});
    assertExists(11, 36, {12, 15});
    assertExists(11, 37, {10, 15});
    assertExists(11, 38, {10, 15});
}


void BGPTest::ringStarConnectedToMesh_CentralNode_ForwardingTableIsCorrect() {
    assertExists(23, 24, {17});
    assertExists(23, 25, {17});
    assertExists(23, 26, {17});
    assertExists(23, 27, {17});
    assertExists(23, 28, {19, 17});
    assertExists(23, 29, {19, 17});
    assertExists(23, 30, {19});
    assertExists(23, 31, {19});
}

void BGPTest::ringStarConnectedToMesh_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(21, 24, {22, 23});
    assertExists(21, 25, {22, 23});
    assertExists(21, 26, {22, 23});
    assertExists(21, 27, {22, 23});
    assertExists(21, 28, {20, 22, 23});
    assertExists(21, 29, {20, 22, 23});
    assertExists(21, 30, {20, 23});
    assertExists(21, 31, {20, 23});
}

void BGPTest::ringStarConnectedToMesh_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(17, 24, {14});
    assertExists(17, 25, {14});
    assertExists(17, 26, {14});
    assertExists(17, 27, {14});
    assertExists(17, 28, {14, 18});
    assertExists(17, 29, {14, 18});
    assertExists(17, 30, {14, 18});
    assertExists(17, 31, {14, 18});
}
void BGPTest::ringStarConnectedToMesh_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(22, 24, {17});
    assertExists(22, 25, {17});
    assertExists(22, 26, {17});
    assertExists(22, 27, {17});
    assertExists(22, 28, {17});
    assertExists(22, 29, {17});
    assertExists(22, 30, {17, 21});
    assertExists(22, 31, {17, 21});
}

void BGPTest::ringStarConnectedToMesh_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(18, 24, {15, 17});
    assertExists(18, 25, {15, 17});
    assertExists(18, 26, {15, 17});
    assertExists(18, 27, {15, 17});
    assertExists(18, 28, {15});
    assertExists(18, 29, {15});
    assertExists(18, 30, {15, 19});
    assertExists(18, 31, {15, 19});
}

void BGPTest::torus_TopLeftCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(1, 32, {2, 4, 13});
    assertExists(1, 33, {2, 4, 13});
    assertExists(1, 34, {4, 13});
    assertExists(1, 35, {4, 13});
    assertExists(1, 36, {4, 13});
    assertExists(1, 37, {2, 13});
    assertExists(1, 38, {2, 13});
}

void BGPTest::torus_TopRightCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(4, 32, {16});
    assertExists(4, 33, {16});
    assertExists(4, 34, {16});
    assertExists(4, 35, {16});
    assertExists(4, 36, {16});
    assertExists(4, 37, {3, 16});
    assertExists(4, 38, {3, 16});
}

void BGPTest::torus_BottomLeftCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(13, 32, {14, 16});
    assertExists(13, 33, {14, 16});
    assertExists(13, 34, {16});
    assertExists(13, 35, {16});
    assertExists(13, 36, {16});
    assertExists(13, 37, {14});
    assertExists(13, 38, {14});
}

void BGPTest::torus_BottomRightCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(16, 32, {19});
    assertExists(16, 33, {19});
    assertExists(16, 34, {19});
    assertExists(16, 35, {19});
    assertExists(16, 36, {19});
    assertExists(16, 37, {15, 19});
    assertExists(16, 38, {15, 19});
}
void BGPTest::torus_TopEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(3, 32, {2, 4, 15});
    assertExists(3, 33, {2, 4, 15});
    assertExists(3, 34, {4, 15});
    assertExists(3, 35, {4, 15});
    assertExists(3, 36, {4, 15});
    assertExists(3, 37, {2, 15});
    assertExists(3, 38, {2, 15});
}

void BGPTest::torus_BottomEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(14, 32, {17});
    assertExists(14, 33, {17});
    assertExists(14, 34, {17, 15});
    assertExists(14, 35, {17, 15});
    assertExists(14, 36, {17, 15});
    assertExists(14, 37, {17});
    assertExists(14, 38, {17});
}

void BGPTest::torus_LeftEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(5, 32, {1, 6, 8, 9});
    assertExists(5, 33, {1, 6, 8, 9});
    assertExists(5, 34, {1, 8, 9});
    assertExists(5, 35, {1, 8, 9});
    assertExists(5, 36, {1, 8, 9});
    assertExists(5, 37, {1, 6, 9});
    assertExists(5, 38, {1, 6, 9});
}

void BGPTest::torus_RightEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(12, 32, {16});
    assertExists(12, 33, {16});
    assertExists(12, 34, {16});
    assertExists(12, 35, {16});
    assertExists(12, 36, {16});
    assertExists(12, 37, {16, 11});
    assertExists(12, 38, {16, 11});
}

void BGPTest::torus_Middle_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(11, 32, {10, 12, 15});
    assertExists(11, 33, {10, 12, 15});
    assertExists(11, 34, {12, 15});
    assertExists(11, 35, {12, 15});
    assertExists(11, 36, {12, 15});
    assertExists(11, 37, {10, 15});
    assertExists(11, 38, {10, 15});
}

void BGPTest::ringStarConnectedToTorus_CentralNode_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(23, 24, {17, 19});
    assertExists(23, 25, {17, 19});
    assertExists(23, 26, {17});
    assertExists(23, 27, {17});
    assertExists(23, 28, {19, 17});
    assertExists(23, 29, {19, 17});
    assertExists(23, 30, {19});
    assertExists(23, 31, {19});
}

void BGPTest::ringStarConnectedToTorus_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(21, 24, {20, 22, 23});
    assertExists(21, 25, {20, 22, 23});
    assertExists(21, 26, {22, 23});
    assertExists(21, 27, {22, 23});
    assertExists(21, 28, {20, 22, 23});
    assertExists(21, 29, {20, 22, 23});
    assertExists(21, 30, {20, 23});
    assertExists(21, 31, {20, 23});
}

void BGPTest::ringStarConnectedToTorus_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(17, 24, {14});
    assertExists(17, 25, {14});
    assertExists(17, 26, {14});
    assertExists(17, 27, {14});
    assertExists(17, 28, {14, 18});
    assertExists(17, 29, {14, 18});
    assertExists(17, 30, {14, 18});
    assertExists(17, 31, {14, 18});
}
void BGPTest::ringStarConnectedToTorus_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(22, 24, {17});
    assertExists(22, 25, {17});
    assertExists(22, 26, {17});
    assertExists(22, 27, {17});
    assertExists(22, 28, {17});
    assertExists(22, 29, {17});
    assertExists(22, 30, {17, 21});
    assertExists(22, 31, {17, 21});
}

void BGPTest::ringStarConnectedToTorus_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(18, 24, {15, 17, 19});
    assertExists(18, 25, {15, 17, 19});
    assertExists(18, 26, {15, 17});
    assertExists(18, 27, {15, 17});
    assertExists(18, 28, {15});
    assertExists(18, 29, {15});
    assertExists(18, 30, {15, 19});
    assertExists(18, 31, {15, 19});
}
