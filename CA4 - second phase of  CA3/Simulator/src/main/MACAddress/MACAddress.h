#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QObject>
#include <QString>
#include <QSharedPointer>

class MACAddress;
typedef QSharedPointer<MACAddress> MACAddressPtr;

class MACAddress : public QObject
{
    Q_OBJECT

public:
    explicit MACAddress(const QString &mac = "", QObject *parent = nullptr);

    bool equals(MACAddressPtr other);
    QString toString() const;
    static bool isValid(const QString &mac);

private:
    QString address;
};



#endif
