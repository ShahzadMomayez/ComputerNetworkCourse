#include "IPv6.h"

IPv6::IP(QObject *parent)
    : AbstractIP(parent)
{
    value = std::numeric_limits<__uint128_t>::max();
}

IPv6::IP(__uint128_t value, QObject *parent)
    : AbstractIP(parent)
{
    this->value = value;
}

IP<UT::IPVersion::IPv6>::~IP() {};

// IPPtr IP<UT::IPVersion::IPv6>::bitwiseAnd(IPPtr other) {
//     IPv6* ipv6 =  (IPv6*) other.get();
//     IPPtr result = IPPtr(new IPv6(ipv6->value & value));
//     return result;
// }
