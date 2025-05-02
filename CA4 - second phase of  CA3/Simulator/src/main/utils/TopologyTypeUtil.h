#ifndef TOPOLOGYTYPEUTIL_H
#define TOPOLOGYTYPEUTIL_H

#include <string>
#include "../Globals/Globals.h"


class TopologyTypeUtil
{
public:
    TopologyTypeUtil();
    static UT::TopologyType parseTopologyString(std::string topologyString);
};

#endif
