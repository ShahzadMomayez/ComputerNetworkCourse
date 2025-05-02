#ifndef ROUTINGPROTOCOLTYPEUTIL_H
#define ROUTINGPROTOCOLTYPEUTIL_H

#include "../Globals/Globals.h"
#include <QString>

class RoutingProtocolTypeUtil
{
public:
    RoutingProtocolTypeUtil();
    static QString toString(UT::RoutingProtocolType type);
};

#endif
