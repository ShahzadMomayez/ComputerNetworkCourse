#include "tst_DHCPServer.h"
#include <QTest>
#include <QDir>
#include <QFileInfo>

#define INTERVAL_DURATION 2
#define NUM_TICKS_FIRST_RUN 15
#define NUM_TICKS_SECOND_RUN 5


void DHCPServerTest::initTestCase() {
    torus = false;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.444"));
    network->create();
    runSpanningTreeAlg();
    runDHCP();
}

void DHCPServerTest::cleanupTestCase() {
    QDir dir("../../logs");

    if (dir.exists()) {
        QStringList files = dir.entryList(QStringList("dhcp*"), QDir::Files);

        for (const QString &file : files) {
            dir.remove(file);
        }
    }
}

void DHCPServerTest::runSpanningTreeAlg() {
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

void DHCPServerTest::runDHCP() {
    network->runDHCP();
    
    for (int i = 0; i < 300; i++) {
        QThread::msleep(5);
        Q_EMIT EventCoordinator::instance()->nextTick(0);
    }
}

void DHCPServerTest::convertToTorus() {
    if (torus) {
        return;
    }
    torus = true;
    network = NetworkPtr(new Network("../../assets/test/tst_config.json", "192.168.444"));
    network->create();
    network->getAutonomousSystem(1)->convertMeshToTorus();
    runSpanningTreeAlg();
    runDHCP();
}

void DHCPServerTest::assertRouterIP(int asId, int nodeId, QString expected) {
    QString actual = network->getAutonomousSystem(asId)->getRouter(nodeId)->getIP()->toString();

    QCOMPARE(actual, expected);
}


void DHCPServerTest::assertPcIP(int pcId, QString expected) {
    QString actual = network->getPC(pcId)->getIP()->toString();

    QCOMPARE(actual, expected);
}

void DHCPServerTest::assertMACAddressExists(int asId, int routerId) {
    auto actual = network->getAutonomousSystem(asId)->getRouter(routerId)->getMACAddress();

    QVERIFY(actual != nullptr);
}

void DHCPServerTest::assertMACAddressExists(int pcId) {
    auto actual = network->getPC(pcId)->getMACAddress();

    QVERIFY(actual != nullptr);
}

void DHCPServerTest::mesh_TopLeftCorner_AssignedCorrectIp() {
    assertRouterIP(1, 1, "192.168.100.1");
}

void DHCPServerTest::mesh_TopRightCorner_AssignedCorrectIp() {
    assertRouterIP(1, 4, "192.168.100.4");
}

void DHCPServerTest::mesh_BottomLeftCorner_AssignedCorrectIp() {
    assertRouterIP(1, 13, "192.168.100.13");
}

void DHCPServerTest::mesh_BottomRightCorner_AssignedCorrectIp() {
    assertRouterIP(1, 16, "192.168.100.16");
}

void DHCPServerTest::mesh_TopEdge_AssignedCorrectIp() {
    assertRouterIP(1, 2, "192.168.100.2");
    assertRouterIP(1, 3, "192.168.100.3");
}

void DHCPServerTest::mesh_BottomEdge_AssignedCorrectIp() {
    assertRouterIP(1, 14, "192.168.100.14");
    assertRouterIP(1, 15, "192.168.100.15");
}

void DHCPServerTest::mesh_LeftEdge_AssignedCorrectIp() {
    assertRouterIP(1, 5, "192.168.100.5");
    assertRouterIP(1, 9, "192.168.100.9");
}

void DHCPServerTest::mesh_RightEdge_AssignedCorrectIp() {
    assertRouterIP(1, 8, "192.168.100.8");
    assertRouterIP(1, 12, "192.168.100.12");
}

void DHCPServerTest::mesh_Middle_AssignedCorrectIp() {
    assertRouterIP(1, 6, "192.168.100.6");
    assertRouterIP(1, 7, "192.168.100.7");
    assertRouterIP(1, 10, "192.168.100.10");
    assertRouterIP(1, 11, "192.168.100.11");
}

void DHCPServerTest::mesh_PC_AssignedCorrectIp() {
    convertToTorus();
    assertPcIP(24, "192.168.100.24");
    assertPcIP(25, "192.168.100.25");
    assertPcIP(26, "192.168.100.26");
    assertPcIP(27, "192.168.100.27");
    assertPcIP(28, "192.168.100.28");
    assertPcIP(29, "192.168.100.29");
    assertPcIP(30, "192.168.100.30");
    assertPcIP(31, "192.168.100.31");
}

void DHCPServerTest::mesh_TopLeftCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 1);
}

void DHCPServerTest::mesh_TopRightCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 4);
}

void DHCPServerTest::mesh_BottomLeftCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 13);
}

void DHCPServerTest::mesh_BottomRightCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 16);
}

void DHCPServerTest::mesh_TopEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 2);
    assertMACAddressExists(1, 3);
}

void DHCPServerTest::mesh_BottomEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 14);
    assertMACAddressExists(1, 15);
}

void DHCPServerTest::mesh_LeftEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 5);
    assertMACAddressExists(1, 9);
}

void DHCPServerTest::mesh_RightEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 8);
    assertMACAddressExists(1, 12);
}

void DHCPServerTest::mesh_Middle_AssignedMACAddress() {
    assertMACAddressExists(1, 6);
    assertMACAddressExists(1, 7);
    assertMACAddressExists(1, 10);
    assertMACAddressExists(1, 11);
}

void DHCPServerTest::mesh_PC_AssignedMACAddress() {
    assertMACAddressExists(24);
    assertMACAddressExists(25);
    assertMACAddressExists(26);
    assertMACAddressExists(27);
    assertMACAddressExists(28);
    assertMACAddressExists(29);
    assertMACAddressExists(30);
    assertMACAddressExists(31);
}

void DHCPServerTest::torus_TopLeftCorner_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 1, "192.168.100.1");
}

void DHCPServerTest::torus_TopRightCorner_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 4, "192.168.100.4");
}

void DHCPServerTest::torus_BottomLeftCorner_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 13, "192.168.100.13");
}

void DHCPServerTest::torus_BottomRightCorner_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 16, "192.168.100.16");
}

void DHCPServerTest::torus_TopEdge_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 2, "192.168.100.2");
    assertRouterIP(1, 3, "192.168.100.3");
}

void DHCPServerTest::torus_BottomEdge_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 14, "192.168.100.14");
    assertRouterIP(1, 15, "192.168.100.15");
}

void DHCPServerTest::torus_LeftEdge_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 5, "192.168.100.5");
    assertRouterIP(1, 9, "192.168.100.9");
}

void DHCPServerTest::torus_RightEdge_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 8, "192.168.100.8");
    assertRouterIP(1, 12, "192.168.100.12");
}

void DHCPServerTest::torus_Middle_AssignedCorrectIp() {
    convertToTorus();
    assertRouterIP(1, 6, "192.168.100.6");
    assertRouterIP(1, 7, "192.168.100.7");
    assertRouterIP(1, 10, "192.168.100.10");
    assertRouterIP(1, 11, "192.168.100.11");
}

void DHCPServerTest::torus_PC_AssignedCorrectIp() {
    convertToTorus();
    assertPcIP(24, "192.168.100.24");
    assertPcIP(25, "192.168.100.25");
    assertPcIP(26, "192.168.100.26");
    assertPcIP(27, "192.168.100.27");
    assertPcIP(28, "192.168.100.28");
    assertPcIP(29, "192.168.100.29");
    assertPcIP(30, "192.168.100.30");
    assertPcIP(31, "192.168.100.31");
}

void DHCPServerTest::torus_TopLeftCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 1);
}

void DHCPServerTest::torus_TopRightCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 4);
}

void DHCPServerTest::torus_BottomLeftCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 13);
}

void DHCPServerTest::torus_BottomRightCorner_AssignedMACAddress() {
    assertMACAddressExists(1, 16);
}

void DHCPServerTest::torus_TopEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 2);
    assertMACAddressExists(1, 3);
}

void DHCPServerTest::torus_BottomEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 14);
    assertMACAddressExists(1, 15);
}

void DHCPServerTest::torus_LeftEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 5);
    assertMACAddressExists(1, 9);
}

void DHCPServerTest::torus_RightEdge_AssignedMACAddress() {
    assertMACAddressExists(1, 8);
    assertMACAddressExists(1, 12);
}

void DHCPServerTest::torus_Middle_AssignedMACAddress() {
    assertMACAddressExists(1, 6);
    assertMACAddressExists(1, 7);
    assertMACAddressExists(1, 10);
    assertMACAddressExists(1, 11);
}

void DHCPServerTest::torus_PC_AssignedMACAddress() {
    assertMACAddressExists(24);
    assertMACAddressExists(25);
    assertMACAddressExists(26);
    assertMACAddressExists(27);
    assertMACAddressExists(28);
    assertMACAddressExists(29);
    assertMACAddressExists(30);
    assertMACAddressExists(31);
}

void DHCPServerTest::ringStar_CentralNode_AssignedCorrectIp() {
    assertRouterIP(2, 23, "192.168.200.23");
}

void DHCPServerTest::ringStar_SpokeNode_AssignedCorrectIp() {
    assertRouterIP(2, 17, "192.168.200.17");
    assertRouterIP(2, 19, "192.168.200.19");
    assertRouterIP(2, 21, "192.168.200.21");
}

void DHCPServerTest::ringStar_NonSpokeNode_AssignedCorrectIp() {
    assertRouterIP(2, 18, "192.168.200.18");
    assertRouterIP(2, 20, "192.168.200.20");
    assertRouterIP(2, 22, "192.168.200.22");
}


void DHCPServerTest::ringStar_PC_AssignedCorrectIp() {
    assertPcIP(32, "192.168.200.32");
    assertPcIP(33, "192.168.200.33");
    assertPcIP(34, "192.168.200.34");
    assertPcIP(35, "192.168.200.35");
    assertPcIP(36, "192.168.200.36");
    assertPcIP(37, "192.168.200.37");
    assertPcIP(38, "192.168.200.38");
}

void DHCPServerTest::ringStar_CentralNode_AssignedMACAddress() {
    assertMACAddressExists(2, 23);
}

void DHCPServerTest::ringStar_SpokeNode_AssignedMACAddress() {
    assertMACAddressExists(2, 17);
    assertMACAddressExists(2, 19);
    assertMACAddressExists(2, 21);
}

void DHCPServerTest::ringStar_NonSpokeNode_AssignedMACAddress() {
    assertMACAddressExists(2, 18);
    assertMACAddressExists(2, 20);
    assertMACAddressExists(2, 22);
}

void DHCPServerTest::ringStar_PC_AssignedMACAddress() {
    assertMACAddressExists(32);
    assertMACAddressExists(33);
    assertMACAddressExists(34);
    assertMACAddressExists(35);
    assertMACAddressExists(36);
    assertMACAddressExists(37);
    assertMACAddressExists(38);
}
