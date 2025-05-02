#ifndef GLOBALS_H
#define GLOBALS_H

#include "Macros.h"

namespace UT {

    enum class IPVersion {
        IPv4,
        IPv6
    };

    enum class PacketType {
        Data,
        Control
    };

    enum class PacketControlType : uint8_t
    {
        Request,
        Response,
        Acknowledge,
        Error,
        DHCPDiscovery,
        DHCPOffer,
        DHCPRequest,
        DHCPAcknowledge,
        DHCPNak,
        RIP,
        OSPF,
        BGP,
        SpanningTreeConfig,

        TCP_SYN,
        TCP_SYN_ACK,
        TCP_ACK,
        TCP_FIN,
        TCP_URG,
        TCP_PSH,
        TCP_RST
    };

    enum class DistributionType {
        Poisson,
        Pareto
    };

    enum class TopologyType {
        Mesh,
        RingStar,
        Torus
    };

    enum class RoutingProtocolType {
        RIP,
        OSPF,
        BGP
    };

    enum class TCPConnectionState
    {
        CLOSED,
        LISTEN,
        SYN_SENT,
        SYN_RECEIVED,
        ESTABLISHED,
        FIN_WAIT_1,
        FIN_WAIT_2,
        CLOSE_WAIT,
        CLOSING,
        LAST_ACK,
        TIME_WAIT
    };
}

#endif
