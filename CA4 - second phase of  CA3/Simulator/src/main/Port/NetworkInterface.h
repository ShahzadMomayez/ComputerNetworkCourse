#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include "Port.h"

#include <QObject>

class NetworkInterface : public QObject
{
    Q_OBJECT

public:
    NetworkInterface(PortPtr port, IPPtr toIP, int toId, bool border = false, bool hasSent = false,
                     bool broadcast = true);

    void  send(const PacketPtr data);
    bool  hasSent();
    bool  equals(IPPtr toIP);
    bool  equals(int toId);
    void  resetSent();
    bool  shouldBroadcast();
    void  setBroadcast(bool broadcast);
    bool  isBorder();
    int   getToId();
    void  setToIP(IPPtr toIP);

    // private:
    IPPtr getToIP();

public:
    PortPtr port;
    IPPtr   toIP;
    bool    sent;
    int     toId;    //neighbor's id
    bool    broadcast;
    bool    border;
};

typedef QSharedPointer<NetworkInterface> NetworkInterfacePtr;
#endif
