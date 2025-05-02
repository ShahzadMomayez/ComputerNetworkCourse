#include <stdexcept>
#include "TopologyTypeUtil.h"

TopologyTypeUtil::TopologyTypeUtil() {}

UT::TopologyType TopologyTypeUtil::parseTopologyString(std::string topologyString) {
    if (topologyString == "Mesh") {
        return UT::TopologyType::Mesh;
    }
    else if (topologyString == "Torus") {
        return UT::TopologyType::Torus;
    }
    else if (topologyString == "RingStar"){
        return UT::TopologyType::RingStar;
    }
    else {
        throw std::invalid_argument("Invalid topology type");
    }
}
