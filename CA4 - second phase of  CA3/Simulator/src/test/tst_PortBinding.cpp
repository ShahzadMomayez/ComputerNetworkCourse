#include "tst_PortBinding.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <QThread>

#define SLEEP_TIME 10

void PortBindingTest::init() {
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.100"));
    network->create();

    createRoutersSpy();
    createPCsSpy();
}

void PortBindingTest::createRoutersSpy() {
    QMap<int, QSignalSpy*> AS1Spies;
    QMap<int, QSignalSpy*> AS2Spies;

    for (int i = 1; i < 17; i++) {
        QSignalSpy* spy = new QSignalSpy(network->getAutonomousSystem(1)->getRouter(i).get(), &Router::packetReceived);
        AS1Spies.insert(i, spy);
    }
    for (int i = 17; i < 24; i++) {
        QSignalSpy* spy = new QSignalSpy(network->getAutonomousSystem(2)->getRouter(i).get(), &Router::packetReceived);
        AS2Spies.insert(i, spy);
    }
    routersSpy.insert(1, AS1Spies);
    routersSpy.insert(2, AS2Spies);

}

void PortBindingTest::createPCsSpy() {
    for (int i = 24; i < 39; i++) {
        QSignalSpy* spy = new QSignalSpy(network->getPC(i).get(), &PC::packetReceived);
        PCsSpy.insert(i, spy);
    }
}

int PortBindingTest::getASId(int routerId) {
    if (routerId < 17) {
        return 1;
    }
    return 2;
}

void PortBindingTest::broadcast(int routerId) {
    int ASId = getASId(routerId);
    network->getAutonomousSystem(ASId)->getRouter(routerId)->broadcastAll(nullptr);
}

void PortBindingTest::broadcastAbroad(int routerId) {
    int ASId = getASId(routerId);
    network->getAutonomousSystem(ASId)->getRouter(routerId)->broadcastAbroad(nullptr);
}

void PortBindingTest::assertEmits(int routerId) {
    int ASId = getASId(routerId);
    QCOMPARE(routersSpy.value(ASId).value(routerId)->count(), 1);
}

void PortBindingTest::assertNotEmits(int routerId) {
    int ASId = getASId(routerId);

    QCOMPARE(routersSpy.value(ASId).value(routerId)->count(), 0);
}

void PortBindingTest::assertNotEmitsExcept(int ASId, std::list<int> routerIds) {
    QThread::msleep(SLEEP_TIME);
    for (int i = 1; i < 24; i++) {
        auto it = std::find(routerIds.begin(), routerIds.end(), i);

        if (it != routerIds.end()) {
            assertEmits(i);
        }
        else if (getASId(i) == ASId){
            assertNotEmits(i);
        }
    }
}

void PortBindingTest::assertNotEmitsExcept(std::list<int> PCIds) {
    QThread::msleep(SLEEP_TIME);
    for (int i = 24; i < 39; i++) {
        auto it = std::find(PCIds.begin(), PCIds.end(), i);

        if (it != PCIds.end()) {
            QCOMPARE(PCsSpy.value(i)->count(), 1);
        }
        else {
            QCOMPARE(PCsSpy.value(i)->count(), 0);
        }
    }
}

void PortBindingTest::mesh_TopLeftCorner_ConnectedToNeighbors() {
    broadcast(1);

    assertNotEmitsExcept(1, {2, 5});
}

void PortBindingTest::mesh_TopRightCorner_ConnectedToNeighbors() {
    broadcast(4);

    assertNotEmitsExcept(1, {3, 8});
}

void PortBindingTest::mesh_BottomLeftCorner_ConnectedToNeighbors() {
    broadcast(13);

    assertNotEmitsExcept(1, {9, 14});
}

void PortBindingTest::mesh_BottomRightCorner_ConnectedToNeighbors() {
    broadcast(16);

    assertNotEmitsExcept(1, {12, 15});
}

void PortBindingTest::mesh_TopEdge_ConnectedToNeighbors() {
    broadcast(2);

    assertNotEmitsExcept(1, {1, 3, 6});
}

void PortBindingTest::mesh_BottomEdge_ConnectedToNeighbors() {
    broadcast(15);

    assertNotEmitsExcept(1, {11, 14, 16});
}
void PortBindingTest::mesh_LeftEdge_ConnectedToNeighbors() {
    broadcast(5);

    assertNotEmitsExcept(1, {1, 6, 9});
}

void PortBindingTest::mesh_RightEdge_ConnectedToNeighbors() {
    broadcast(12);

    assertNotEmitsExcept(1, {8, 11, 16});
}

void PortBindingTest::mesh_Middle_ConnectedToNeighbors() {
    broadcast(10);

    assertNotEmitsExcept(1, {6, 9, 11, 14});
}


void PortBindingTest::torus_TopLeftCorner_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(1);

    assertNotEmitsExcept(1, {2, 4, 5, 13});
}

void PortBindingTest::torus_TopRightCorner_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(4);

    assertNotEmitsExcept(1, {1, 3, 8, 16});
}

void PortBindingTest::torus_BottomLeftCorner_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(13);

    assertNotEmitsExcept(1, {1, 9, 14, 16});
}

void PortBindingTest::torus_BottomRightCorner_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(16);

    assertNotEmitsExcept(1, {4, 12, 13, 15});
}

void PortBindingTest::torus_TopEdge_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(2);

    assertNotEmitsExcept(1, {1, 3, 6, 14});
}

void PortBindingTest::torus_BottomEdge_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(14);

    assertNotEmitsExcept(1, {2, 10, 13, 15});
}
void PortBindingTest::torus_LeftEdge_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(5);

    assertNotEmitsExcept(1, {1, 6, 8, 9});
}

void PortBindingTest::torus_RightEdge_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(12);

    assertNotEmitsExcept(1, {9, 8, 11, 16});
}

void PortBindingTest::torus_Middle_ConnectedToNeighbors() {
    network->getAutonomousSystem(1)->convertMeshToTorus();
    broadcast(10);

    assertNotEmitsExcept(1, {6, 9, 11, 14});
}

void PortBindingTest::ringStar_Begin_ConnectedToNeighbors() {
    broadcast(17);

    assertNotEmitsExcept(2, {18, 22, 23});
}

void PortBindingTest::ringStar_End_ConnectedToNeighbors() {
    broadcast(22);

    assertNotEmitsExcept(2, {17, 21});
}

void PortBindingTest::ringStar_Center_ConnectedToNeighbors() {
    broadcast(23);

    assertNotEmitsExcept(2, {17, 19, 21});
}

void PortBindingTest::diffrentAutonomousSystems_Connected_ConnectedToEachOther() {
    broadcastAbroad(18);

    assertNotEmitsExcept(1, {15});
}

void PortBindingTest::diffrentAutonomousSystems_NotConnected_NotConnectedToEachOther() {
    broadcastAbroad(13);

    assertNotEmitsExcept(2, {});
}

void PortBindingTest::mesh_FirstConfig_ConnectedToPCs() {
    broadcast(1);

    assertNotEmitsExcept({24, 25});

}

void PortBindingTest::mesh_LastConfig_ConnectedToPCs() {
    broadcast(4);

    assertNotEmitsExcept({30, 31});
}

void PortBindingTest::ringStar_FirstConfig_ConnectedToPCs() {
    broadcast(20);

    assertNotEmitsExcept({34, 35, 36});
}

void PortBindingTest::ringStar_LastConfig_ConnectedToPCs() {
    broadcast(22);

    assertNotEmitsExcept({37, 38});
}

void PortBindingTest::pc_ConnectedToRouter() {
    network->getPC(24)->sendPacket(nullptr);

    assertNotEmitsExcept(1, {1});
}
