#ifndef DHCPPAYLOAD_H
#define DHCPPAYLOAD_H

#include "../Payload/Payload.h"
#include "../IP/IP.h"
#include "../MACAddress/MACAddress.h"

#include <QObject>
#include <QSharedPointer>

class DHCPPayload;
typedef QSharedPointer<DHCPPayload> DHCPPayloadPtr;

class DHCPPayload : public Payload
{
    Q_OBJECT

public:
    explicit DHCPPayload(int id, MACAddressPtr macAddress=nullptr, IPPtr ip=nullptr, QObject *parent = nullptr);
    int getId();
    IPPtr getIP();
    int getPreviousHopId();
    void setPreviousHopId(int hopId);
    void setIP(IPPtr ip);
    void setMACAddress(MACAddressPtr macAddress);
    MACAddressPtr getMACAddress();
    PayloadPtr clone();

private:
    int id;
    IPPtr ip;
    MACAddressPtr macAddress;
    int previousHopId;
};

#endif    
