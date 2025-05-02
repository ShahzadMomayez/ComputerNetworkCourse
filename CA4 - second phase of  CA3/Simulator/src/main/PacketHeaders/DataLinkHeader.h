#ifndef DATALINKHEADER_H
#define DATALINKHEADER_H

#include "../MACAddress/MACAddress.h"

class DataLinkHeader
{

private:
    MACAddress sourceMACAddress;
    MACAddress destinationMACAddress;
};

typedef QSharedPointer<DataLinkHeader> DataLinkHeaderPtr;
#endif
