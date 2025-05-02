#include "IPv4.h"
#include "qdebug.h"
#include <QDebug>


IPv4::IP(QObject *parent)
    : AbstractIP(parent)
{
    value = std::numeric_limits<uint32_t>::max();
}

IPv4::IP(uint32_t value, QObject *parent)
    : AbstractIP(parent) 
{
    this->value = value;
}

IPv4::IP(QString _value, QObject *parent)
    : AbstractIP(parent)
{
    this->value = fromString(_value);
}

bool IPv4::equals(IPPtr other) {
    if (other == nullptr) {
        return false;
    }
    
    IPv4* ipv4 = (IPv4*)other.get();
    return value == ipv4->value;
}

IPv4::~IP() {}

IPPtr IPv4::bitwiseAnd(IPPtr other) {
    IPv4* ipv4 =  (IPv4*) other.get();
    IPPtr result = IPPtr(new IPv4(ipv4->value & value));
    return result;
}

QString IPv4::toString()
{
    uint8_t octet1 = (value >> 24) & 0xFF;
    uint8_t octet2 = (value >> 16) & 0xFF;
    uint8_t octet3 = (value >> 8) & 0xFF;
    uint8_t octet4 = value & 0xFF;

    return QString("%1.%2.%3.%4").arg(octet1).arg(octet2).arg(octet3).arg(octet4);
}

int32_t IPv4::fromString(QString ip) {
    QStringList octets = ip.split('.');

    quint32 ipInt = 0;
    for (int i = 0; i < 4; ++i) {
        int octet = octets[i].toInt();
        ipInt |= (octet << (8 * (3 - i)));
    }
    return ipInt;
}

IPPtr IPv4::clone() {
    return IPPtr(new IPv4(toString()));
}