#ifndef IPV4_H
#define IPV4_H

#include <QObject>
#include <cstdint>
#include "IP.h"

template <>
class IP<UT::IPVersion::IPv4> : public AbstractIP
{
    // removed Q_OBJECT macro to avoid moc error
    // https://doc.qt.io/qt-6/moc.html
    // Q_OBJECT

public:
    explicit IP(QObject *parent = nullptr);
    explicit IP(uint32_t value, QObject *parent = nullptr);
    explicit IP(QString value, QObject *parent = nullptr);
    ~IP() override;
    int32_t fromString(QString ip);
    IPPtr bitwiseAnd(IPPtr other) override;
    bool equals(IPPtr other) override;
    QString toString() override;
    IPPtr clone() override;

private:
    uint32_t value;
};

typedef IP<UT::IPVersion::IPv4> IPv4;
typedef QSharedPointer<IPv4>  IPv4Ptr;

#endif  
