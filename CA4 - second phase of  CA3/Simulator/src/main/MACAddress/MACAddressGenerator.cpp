#include "MACAddressGenerator.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <QSharedPointer>
#include "MACAddress.h"


MACAddressGeneratorPtr MACAddressGenerator::instance(QObject *parent) {
    if(!self) {
        self = MACAddressGeneratorPtr(new MACAddressGenerator(parent));
    }
    return self;
}

MACAddressGenerator::MACAddressGenerator(QObject *parent) {}

MACAddressPtr MACAddressGenerator::generateUniqueAddress() {
    MACAddressPtr macAddress;
    
    do {
        macAddress = generate();
    } while (!isUnique(macAddress));

    return macAddress;
}

MACAddressPtr MACAddressGenerator::generate() {
    std::ostringstream macAddress;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dis(0, 255);

    for (int i = 0; i < 6; ++i) {
        macAddress << std::hex << std::setw(2) << std::setfill('0') << dis(gen);
        if (i < 5) {
            macAddress << ":";
        }
    }

    MACAddressPtr macAddressPtr(new MACAddress(QString::fromStdString(macAddress.str())));
    return macAddressPtr;
}

bool MACAddressGenerator::isUnique(MACAddressPtr macAddress) {
    for (MACAddressPtr address : macAddresses) {
        if (macAddress->equals(address)) {
            return false;
        }
    }
    return true;
}
