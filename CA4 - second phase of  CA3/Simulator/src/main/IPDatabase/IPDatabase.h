#ifndef IPDATABASE_H
#define IPDATABASE_H

#include "../IP/IPv4.h"
#include "../MACAddress/MACAddress.h"

#include <QObject>
#include <QSharedPointer>
#include <QMap>

class IPDatabase : public QObject
{
    Q_OBJECT

public:
    explicit IPDatabase(QObject *parent = nullptr);
    IPPtr getIP(int id);
    MACAddressPtr getMACAddress(int id);
    void add(int id, IPPtr ip, MACAddressPtr macAddress, bool isPC=false);
    QList<int> pcsId();

private:
    QMap<int, IPPtr> ipMap;
    QMap<int, MACAddressPtr> macMap;
    QMap<int, bool> isPC;
};

typedef QSharedPointer<IPDatabase> IPDatabasePtr;

#endif
