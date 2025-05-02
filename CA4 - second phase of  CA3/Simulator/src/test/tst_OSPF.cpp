#include "tst_OSPF.h"

#include "../main/IP/IPv4.h"

#define INTERVAL_DURATION 2
#define NUM_TICKS 25
#define NUM_TICKS_FIRST_RUN 15
#define NUM_TICKS_SECOND_RUN 5

void OSPFTest::initTestCase() {
    torus = false;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    runOSPF();
}

void OSPFTest::runOSPF() {
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

    network->runOSPF();
    for (int i = 0; i < NUM_TICKS; i++) {
        QThread::msleep(2);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
}

int OSPFTest::getASId(int routerId) {
    if (routerId < 17) {
        return 1;
    }
    return 2;
}

void OSPFTest::assertExists(int routerId, int pcId, std::list<int> gatewayIds) {
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

void OSPFTest::convertToTorus() {
    if (torus) {
        return;
    }
    torus = true;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();
    network->getAutonomousSystem(1)->convertMeshToTorus();
    runOSPF();
}



void OSPFTest::mesh_TopLeftCorner_ForwardingTableIsCorrect() {
    assertExists(1, 24, {24});
    assertExists(1, 25, {25});
    assertExists(1, 26, {2});
    assertExists(1, 27, {2});
    assertExists(1, 28, {2});
    assertExists(1, 29, {2});
    assertExists(1, 30, {2});
    assertExists(1, 31, {2});
}

void OSPFTest::mesh_TopRightCorner_ForwardingTableIsCorrect() {
    assertExists(4, 24, {3});
    assertExists(4, 25, {3});
    assertExists(4, 26, {3});
    assertExists(4, 27, {3});
    assertExists(4, 28, {3});
    assertExists(4, 29, {3});
    assertExists(4, 30, {30});
    assertExists(4, 31, {31});
}

void OSPFTest::mesh_BottomLeftCorner_ForwardingTableIsCorrect() {
    assertExists(13, 24, {9});
    assertExists(13, 25, {9});
    assertExists(13, 26, {9, 14});
    assertExists(13, 27, {9, 14});
    assertExists(13, 28, {9, 14});
    assertExists(13, 29, {9, 14});
    assertExists(13, 30, {9, 14});
    assertExists(13, 31, {9, 14});
}

void OSPFTest::mesh_BottomRightCorner_ForwardingTableIsCorrect() {
    assertExists(16, 24, {12, 15});
    assertExists(16, 25, {12, 15});
    assertExists(16, 26, {12, 15});
    assertExists(16, 27, {12, 15});
    assertExists(16, 28, {12, 15});
    assertExists(16, 29, {12, 15});
    assertExists(16, 30, {12});
    assertExists(16, 31, {12});
}
void OSPFTest::mesh_TopEdge_ForwardingTableIsCorrect() {
    assertExists(3, 24, {2});
    assertExists(3, 25, {2});
    assertExists(3, 26, {2});
    assertExists(3, 27, {2});
    assertExists(3, 28, {28});
    assertExists(3, 29, {29});
    assertExists(3, 30, {4});
    assertExists(3, 31, {4});
}

void OSPFTest::mesh_BottomEdge_ForwardingTableIsCorrect() {
    assertExists(14, 24, {10, 13});
    assertExists(14, 25, {10, 13});
    assertExists(14, 26, {10});
    assertExists(14, 27, {10});
    assertExists(14, 28, {10, 15});
    assertExists(14, 29, {10, 15});
    assertExists(14, 30, {10, 15});
    assertExists(14, 31, {10, 15});
}

void OSPFTest::mesh_LeftEdge_ForwardingTableIsCorrect() {
    assertExists(5, 24, {1});
    assertExists(5, 25, {1});
    assertExists(5, 26, {1, 6});
    assertExists(5, 27, {1, 6});
    assertExists(5, 28, {1, 6});
    assertExists(5, 29, {1, 6});
    assertExists(5, 30, {1, 6});
    assertExists(5, 31, {1, 6});
}

void OSPFTest::mesh_RightEdge_ForwardingTableIsCorrect() {
    assertExists(12, 24, {8, 11});
    assertExists(12, 25, {8, 11});
    assertExists(12, 26, {8, 11});
    assertExists(12, 27, {8, 11});
    assertExists(12, 28, {8, 11});
    assertExists(12, 29, {8, 11});
    assertExists(12, 30, {8});
    assertExists(12, 31, {8});
}

void OSPFTest::mesh_Middle_ForwardingTableIsCorrect() {
    assertExists(11, 24, {7, 10});
    assertExists(11, 25, {7, 10});
    assertExists(11, 26, {7, 10});
    assertExists(11, 27, {7, 10});
    assertExists(11, 28, {7});
    assertExists(11, 29, {7});
    assertExists(11, 30, {7, 12});
    assertExists(11, 31, {7, 12});
}

void OSPFTest::torus_TopLeftCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(1, 24, {24});
    assertExists(1, 25, {25});
    assertExists(1, 26, {2});
    assertExists(1, 27, {2});
    assertExists(1, 28, {2, 4});
    assertExists(1, 29, {2, 4});
    assertExists(1, 30, {4});
    assertExists(1, 31, {4});
}

void OSPFTest::torus_TopRightCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(4, 24, {1});
    assertExists(4, 25, {1});
    assertExists(4, 26, {3, 1});
    assertExists(4, 27, {3, 1});
    assertExists(4, 28, {3});
    assertExists(4, 29, {3});
    assertExists(4, 30, {30});
    assertExists(4, 31, {31});
}

void OSPFTest::torus_BottomLeftCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(13, 24, {1});
    assertExists(13, 25, {1});
    assertExists(13, 26, {1, 14});
    assertExists(13, 27, {1, 14});
    assertExists(13, 28, {1, 14, 16});
    assertExists(13, 29, {1, 14, 16});
    assertExists(13, 30, {1, 16});
    assertExists(13, 31, {1, 16});
}

void OSPFTest::torus_BottomRightCorner_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(16, 24, {4, 13});
    assertExists(16, 25, {4, 13});
    assertExists(16, 26, {4, 13, 15});
    assertExists(16, 27, {4, 13, 15});
    assertExists(16, 28, {4, 15});
    assertExists(16, 29, {4, 15});
    assertExists(16, 30, {4});
    assertExists(16, 31, {4});
}
void OSPFTest::torus_TopEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(3, 24, {2, 4});
    assertExists(3, 25, {2, 4});
    assertExists(3, 26, {2});
    assertExists(3, 27, {2});
    assertExists(3, 28, {28});
    assertExists(3, 29, {29});
    assertExists(3, 30, {4});
    assertExists(3, 31, {4});
}

void OSPFTest::torus_BottomEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(14, 24, {2, 13});
    assertExists(14, 25, {2, 13});
    assertExists(14, 26, {2});
    assertExists(14, 27, {2});
    assertExists(14, 28, {2, 15});
    assertExists(14, 29, {2, 15});
    assertExists(14, 30, {2, 13, 15});
    assertExists(14, 31, {2, 13, 15});
}

void OSPFTest::torus_LeftEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(5, 24, {1});
    assertExists(5, 25, {1});
    assertExists(5, 26, {1, 6});
    assertExists(5, 27, {1, 6});
    assertExists(5, 28, {1, 6, 8});
    assertExists(5, 29, {1, 6, 8});
    assertExists(5, 30, {1, 8});
    assertExists(5, 31, {1, 8});
}

void OSPFTest::torus_RightEdge_ForwardingTableIsCorrect() {
    convertToTorus();
    assertExists(12, 24, {8, 9, 16});
    assertExists(12, 25, {8, 9, 16});
    assertExists(12, 26, {8, 9, 11});
    assertExists(12, 27, {8, 9, 11});
    assertExists(12, 28, {8, 11});
    assertExists(12, 29, {8, 11});
    assertExists(12, 30, {8, 16});
    assertExists(12, 31, {8, 16});
}

void OSPFTest::torus_Middle_ForwardingTableIsCorrect() {
    convertToTorus();    
    assertExists(11, 24, {7, 10, 12, 15});
    assertExists(11, 25, {7, 10, 12, 15});
    assertExists(11, 26, {7, 10, 15});
    assertExists(11, 27, {7, 10, 15});
    assertExists(11, 28, {7, 15});
    assertExists(11, 29, {7, 15});
    assertExists(11, 30, {7, 12, 15});
    assertExists(11, 31, {7, 12, 15});
}

void OSPFTest::ringStar_CentralNode_ForwardingTableIsCorrect() {
    assertExists(23, 32, {21});
    assertExists(23, 33, {21});
    assertExists(23, 34, {19, 21});
    assertExists(23, 35, {19, 21});
    assertExists(23, 36, {19, 21});
    assertExists(23, 37, {17, 21});
    assertExists(23, 38, {17, 21});
}

void OSPFTest::ringStar_SpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(21, 32, {32});
    assertExists(21, 33, {33});
    assertExists(21, 34, {20});
    assertExists(21, 35, {20});
    assertExists(21, 36, {20});
    assertExists(21, 37, {22});
    assertExists(21, 38, {22});
}

void OSPFTest::ringStar_SpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(17, 32, {22, 23});
    assertExists(17, 33, {22, 23});
    assertExists(17, 34, {18, 22, 23});
    assertExists(17, 35, {18, 22, 23});
    assertExists(17, 36, {18, 22, 23});
    assertExists(17, 37, {22});
    assertExists(17, 38, {22});
}
void OSPFTest::ringStar_NonSpokeNodeConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(22, 32, {21});
    assertExists(22, 33, {21});
    assertExists(22, 34, {21});
    assertExists(22, 35, {21});
    assertExists(22, 36, {21});
    assertExists(22, 37, {37});
    assertExists(22, 38, {38});
}

void OSPFTest::ringStar_NonSpokeNodeNotConnedtedToPCs_ForwardingTableIsCorrect() {
    assertExists(18, 32, {17, 19});
    assertExists(18, 33, {17, 19});
    assertExists(18, 34, {19});
    assertExists(18, 35, {19});
    assertExists(18, 36, {19});
    assertExists(18, 37, {17});
    assertExists(18, 38, {17});
}

