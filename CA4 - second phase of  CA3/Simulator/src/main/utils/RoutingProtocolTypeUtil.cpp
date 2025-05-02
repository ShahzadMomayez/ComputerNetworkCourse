#include "RoutingProtocolTypeUtil.h"

RoutingProtocolTypeUtil::RoutingProtocolTypeUtil() {}

QString RoutingProtocolTypeUtil::toString(UT::RoutingProtocolType type) {
    switch(type){
        case UT::RoutingProtocolType::RIP:
            return "RIP";
        case UT::RoutingProtocolType::OSPF:
            return "OSPF";
        case UT::RoutingProtocolType::BGP:
            return "BGP";
    }
}
