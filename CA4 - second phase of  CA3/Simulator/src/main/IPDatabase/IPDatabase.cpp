#include "IPDatabase.h"

IPDatabase::IPDatabase(QObject *parent) :
    QObject {parent}
{}

IPPtr IPDatabase::getIP(int id) {
    return ipMap[id];
}

MACAddressPtr IPDatabase::getMACAddress(int id) {
    return macMap[id];
}

void IPDatabase::add(int id, IPPtr ip, MACAddressPtr macAddress, bool isPC) {
    ipMap[id] = ip;
    macMap[id] = macAddress;
    this->isPC[id] = isPC;
}

QList<int> IPDatabase::pcsId() {
    QList<int> result;

    for (auto id : ipMap.keys()) {
        if (isPC[id]) {
            result.append(id);
        }
    }
    return result;
}