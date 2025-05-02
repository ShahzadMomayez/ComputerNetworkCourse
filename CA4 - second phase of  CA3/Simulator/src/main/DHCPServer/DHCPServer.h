#ifndef DHCPSERVER_H
#define DHCPSERVER_H

#include "../Packet/Packet.h"
#include "../MACAddress/MACAddress.h"

#include <QObject>

class DHCPServer : public QObject
{
    Q_OBJECT

public:   
    explicit DHCPServer(std::string networkPortion, int id, QObject *parent = nullptr);
    ~DHCPServer() override;

Q_SIGNALS:
    void discoveryReceived(const PacketPtr data);
    void requestReceived(const PacketPtr data);
    void offerReady(const PacketPtr data);
    void ackReady(const PacketPtr data);

public Q_SLOTS:
    void sendOffer(const PacketPtr data);
    void sendAck(const PacketPtr data);

private:
    IPPtr generateIP(int id);
    void log(int nodeId, MACAddressPtr macAddress, IPPtr ipv4);

    std::string networkPortion;
    int id;
};

typedef QSharedPointer<DHCPServer> DHCPServerPtr;
#endif
