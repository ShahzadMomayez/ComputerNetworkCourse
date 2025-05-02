#ifndef IPV6_H
#define IPV6_H

#include <QObject>
// #include <QtTypes>
#include "IP.h"

template <>
class IP<UT::IPVersion::IPv6> : public AbstractIP
{
    // removed Q_OBJECT macro to avoid moc error
    // https://doc.qt.io/qt-6/moc.html
    // Q_OBJECT

public:
    explicit IP(QObject *parent = nullptr);
    explicit IP(__uint128_t ipValue, QObject *parent = nullptr);
    ~IP() override;
    IPPtr bitwiseAnd(IPPtr other) override {}
    bool equals(IPPtr other) override {}
    QString toString() override {}

private:
    __uint128_t value;
};

typedef IP<UT::IPVersion::IPv6> IPv6;
typedef QSharedPointer<IPv6>  IPv6Ptr;

#endif
