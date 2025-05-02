#ifndef NODE_H
#define NODE_H

#include <QThread>
#include "../IP/IP.h"
#include "../Port/NetworkInterface.h"

class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(int id, IPPtr ip, QObject *parent = nullptr);
    explicit Node(int id, QString networkPortion, QObject *parent = nullptr);
    virtual void broadcast(PacketPtr packet, NetworkInterfacePtr incommingInterface=nullptr) = 0;
    IPPtr getIP();
    MACAddressPtr getMACAddress();
    int getId();
    virtual void discoverDHCP();


private:
    IPPtr ip;
    MACAddressPtr macAddress;

protected:
    void setIP(IPPtr ip);
    void setMACAddress(MACAddressPtr macAddress);
    virtual void requestDHCP(IPPtr ip, MACAddressPtr macAddress);
    
    int id;
Q_SIGNALS:
};

#endif
