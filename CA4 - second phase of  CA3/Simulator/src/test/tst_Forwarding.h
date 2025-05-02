#ifndef TST_FORWARDING_H
#define TST_FORWARDING_H

#include <QTest>
#include "../main/Network/Network.h"
#include <QSignalSpy>

class ForwardingTest : public QObject
{
    Q_OBJECT

private:
    void fillForwardingTables();
    void createNodesSpy();
    void assertEmits(int routerId);
    void assertNotEmits(int routerId);
    void assertNotEmitsExcept(std::list<int> routerIds);
    void sendPacket(int fromId, int toId);

    NetworkPtr network;
    QMap<int, QSignalSpy*> nodesSpy;

private Q_SLOTS:
    void init();
    void UnicastPacket_ShortestPathContainsTwoRouters_IsSuccessfullySent();
    void UnicastPacket_ShortestPathContainsThreeRouters_IsSuccessfullySent();
    void UnicastPacket_ShortestPathContainsFourRouters_IsSuccessfullySent();

    void UnicastPackets_ForwardOnDifferentPorts_IsSentInOneCycle();
    void UnicastPackets_ForwardOnSamePort_IsSentInTwoCycle();
};

#endif
