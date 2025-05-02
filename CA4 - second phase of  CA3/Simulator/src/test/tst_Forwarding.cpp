#include "tst_Forwarding.h"
#include <QTest>
#include <QThread>
#include <QObject>

#define SLEEP_TIME 10

void ForwardingTest::init() {    

    /* The network being tested in this class is shown below.
    // Note: The metric for each edge is displayed on the corresponding edge. 
    // Note: IP = "192.168.100.{id}"
    //
    //                   PC5
    //                     \
    //                      \ 0
    //                       \       2
    //                        R1-----------R2
    //                        |             |
    //                        |             |
    //                      5 |             | 1
    //                        |             |
    //                        |             |
    //                        R3-----------R4    
    //                       /       1       \
    //                    0 /                 \ 0
    //                     /                   \
    //                   PC7                   PC6
    */

    network = NetworkPtr(new Network("../../assets/test/tst_ForwardingConfig.json", "192.168.100"));
    network->create();
    fillForwardingTables();

    createNodesSpy();
}

void ForwardingTest::fillForwardingTables() {
    auto mask = IPPtr(new IPv4("255.255.255.255"));
    auto router1IP = IPPtr(new IPv4("192.168.100.1"));
    auto router2IP = IPPtr(new IPv4("192.168.100.2"));
    auto router3IP = IPPtr(new IPv4("192.168.100.3"));
    auto router4IP = IPPtr(new IPv4("192.168.100.4"));
    auto pc5IP = IPPtr(new IPv4("192.168.100.5"));
    auto pc6IP = IPPtr(new IPv4("192.168.100.6"));
    auto pc7IP = IPPtr(new IPv4("192.168.100.7"));

    network->getAutonomousSystem(1)->getRouter(1)->getForwardingTable()->addEntries({
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, pc5IP, 0)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router2IP, 3)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router3IP, 6)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router2IP, 4)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router3IP, 5)),
    });
    network->getAutonomousSystem(1)->getRouter(2)->getForwardingTable()->addEntries({
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router1IP, 2)),
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router4IP, 7)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router1IP, 8)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router4IP, 1)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router1IP, 7)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router4IP, 2)),
    });
    network->getAutonomousSystem(1)->getRouter(3)->getForwardingTable()->addEntries({
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router1IP, 5)),
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router4IP, 4)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router1IP, 8)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, router4IP, 1)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, pc7IP, 0)),
    });
    network->getAutonomousSystem(1)->getRouter(4)->getForwardingTable()->addEntries({
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router2IP, 3)),
        ForwardingEntryPtr(new ForwardingEntry(pc5IP, mask, router3IP, 6)),
        ForwardingEntryPtr(new ForwardingEntry(pc6IP, mask, pc6IP, 0)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router2IP, 8)),
        ForwardingEntryPtr(new ForwardingEntry(pc7IP, mask, router3IP, 1)),
    });
} 

void ForwardingTest::createNodesSpy() {
    for (int i = 1; i < 5; i++) {
        QSignalSpy* spy = new QSignalSpy(network->getAutonomousSystem(1)->getRouter(i).get(), &Router::packetReceived);
        nodesSpy.insert(i, spy);
    }
    for (int i = 5; i < 8; i++) {
        QSignalSpy* spy = new QSignalSpy(network->getPC(i).get(), &PC::packetReceived);
        nodesSpy.insert(i, spy);
    }
}

void ForwardingTest::assertEmits(int nodeId) {
    QCOMPARE(nodesSpy.value(nodeId)->count(), 1);
}

void ForwardingTest::assertNotEmits(int nodeId) {
    QCOMPARE(nodesSpy.value(nodeId)->count(), 0);
}

void ForwardingTest::assertNotEmitsExcept(std::list<int> nodeIds) {
    QThread::msleep(SLEEP_TIME);
    for (int i = 1; i < 8; i++) {
        auto it = std::find(nodeIds.begin(), nodeIds.end(), i);

        if (it != nodeIds.end()) {
            assertEmits(i);
        }
        else {
            assertNotEmits(i);
        }
    }
}

void ForwardingTest::sendPacket(int fromId, int toId) {
    PacketPtr packet = PacketPtr(new Packet());
    auto sourceIP = IPv4Ptr(new IPv4("192.168.100." + QString::number(fromId)));
    auto destIP = IPv4Ptr(new IPv4("192.168.100." + QString::number(toId)));
    auto ipv4Header = IPv4HeaderPtr(new IPv4Header(sourceIP, destIP));
    packet->setIPv4Header(ipv4Header);

    network->getPC(fromId)->sendPacket(packet);
}

void ForwardingTest::UnicastPacket_ShortestPathContainsTwoRouters_IsSuccessfullySent() {
    sendPacket(7, 6);

    assertNotEmitsExcept({3, 4, 6});
}

void ForwardingTest::UnicastPacket_ShortestPathContainsThreeRouters_IsSuccessfullySent() {
    sendPacket(6, 5);
    assertNotEmitsExcept({1, 2, 4, 5});
}

void ForwardingTest::UnicastPacket_ShortestPathContainsFourRouters_IsSuccessfullySent() {
    sendPacket(5, 7);
    assertNotEmitsExcept({1, 2, 3, 4, 7});
}

void ForwardingTest::UnicastPackets_ForwardOnDifferentPorts_IsSentInOneCycle() {
    sendPacket(5, 6);
    sendPacket(6, 7);

    QThread::msleep(SLEEP_TIME);
    QCOMPARE(nodesSpy.value(1)->count(), 1);
    QCOMPARE(nodesSpy.value(3)->count(), 1);
    QCOMPARE(nodesSpy.value(4)->count(), 2);
    QCOMPARE(nodesSpy.value(6)->count(), 1);
    QCOMPARE(nodesSpy.value(7)->count(), 1);
}

void ForwardingTest::UnicastPackets_ForwardOnSamePort_IsSentInTwoCycle() {
    sendPacket(5, 6);
    sendPacket(7, 6);
    QThread::msleep(SLEEP_TIME);
    QCOMPARE(nodesSpy.value(1)->count(), 1);
    QCOMPARE(nodesSpy.value(3)->count(), 1);
    QCOMPARE(nodesSpy.value(4)->count(), 2);
    QCOMPARE(nodesSpy.value(6)->count(), 1);
    
    Q_EMIT EventCoordinator::instance()->nextTick(0);
    nodesSpy.value(6)->wait();
    QCOMPARE(nodesSpy.value(6)->count(), 2);
}

