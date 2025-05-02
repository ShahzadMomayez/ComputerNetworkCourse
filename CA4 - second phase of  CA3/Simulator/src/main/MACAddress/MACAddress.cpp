#include "MACAddress.h"
#include <QRegularExpression>

MACAddress::MACAddress(const QString &mac, QObject *parent)
    : address(mac) {}

QString MACAddress::toString() const {
    return address;
}

bool MACAddress::isValid(const QString &mac) {
    QRegularExpression macRegex("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
    return macRegex.match(mac).hasMatch();
}

bool MACAddress::equals(MACAddressPtr other) {
    return address == other->address;
}
