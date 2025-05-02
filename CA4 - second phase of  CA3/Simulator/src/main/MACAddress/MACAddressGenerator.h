#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include "../MACAddress/MACAddress.h"

#include <QObject>
#include <QVector>
#include <QSharedPointer>


class MACAddressGenerator;
typedef QSharedPointer<MACAddressGenerator> MACAddressGeneratorPtr;

class MACAddressGenerator : public QObject
{
    Q_OBJECT

public: 
    static MACAddressGeneratorPtr instance(QObject *parent = nullptr);

    MACAddressPtr generateUniqueAddress();

private:
    explicit MACAddressGenerator(QObject *parent = nullptr);

    MACAddressPtr generate();
    bool isUnique(MACAddressPtr macAddress);

    QVector<MACAddressPtr> macAddresses;
   inline static MACAddressGeneratorPtr self = nullptr;
};

#endif

