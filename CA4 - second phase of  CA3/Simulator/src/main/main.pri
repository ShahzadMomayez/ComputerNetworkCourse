INCLUDEPATH += $$PWD\Globals

SOURCES += \
    $$PWD/ApplicationContext/ApplicationContext.cpp \
    $$PWD/AutonomousSystem/AutonomousSystem.cpp \
    $$PWD/DHCPServer/DHCPPayload.cpp \
    $$PWD/DHCPServer/DHCPServer.cpp \
    $$PWD/DataGenerator/ChunkServer.cpp \
    $$PWD/DataGenerator/DataGenerator.cpp \
    $$PWD/DataGenerator/DataPayload.cpp \
    $$PWD/EventCoordinator/EventCoordinator.cpp \
    $$PWD/ForwardingTable/ForwardingEntry.cpp \
    $$PWD/ForwardingTable/ForwardingTable.cpp \
    $$PWD/Globals/Macros.h \
    $$PWD/IP/IP.cpp \
    $$PWD/IP/IPv4.cpp \
    $$PWD/IP/IPv6.cpp \
    $$PWD/IPDatabase/IPDatabase.cpp \
    $$PWD/LinkState/LinkState.cpp \
    $$PWD/LinkState/LinkStateDatabase.cpp \
    $$PWD/LinkState/LinkStatePayload.cpp \
    $$PWD/MACAddress/MACAddress.cpp \
    $$PWD/MACAddress/MACAddressGenerator.cpp \
    $$PWD/Network/Network.cpp \
    $$PWD/Node/Node.cpp \
    $$PWD/Node/PC.cpp \
    $$PWD/Node/Router.cpp \
    $$PWD/Packet/Packet.cpp \
    $$PWD/PacketHeaders/IPv4Header.cpp \
    $$PWD/PacketHeaders/TCPHeader.cpp \
    $$PWD/Payload/Payload.cpp \
    $$PWD/Port/NetworkInterface.cpp \
    $$PWD/PortBindingManager/PortBindingManager.cpp \
    $$PWD/Port/Port.cpp \
    $$PWD/RoutingProtocols/BGP.cpp \
    $$PWD/RoutingProtocols/OSPF.cpp \
    $$PWD/RoutingProtocols/RIP.cpp \
    $$PWD/RoutingProtocols/RoutingUpdate.cpp \
    $$PWD/SpanningTree/SpanningTreeConfig.cpp \
    $$PWD/TCP/TCP.cpp \
    $$PWD/TCP/TCPPayload.cpp \
    $$PWD/main.cpp \
    $$PWD/utils/ConfigParser.cpp \
    $$PWD/utils/PortUtil.cpp \
    $$PWD/utils/RoutingProtocolTypeUtil.cpp \
    $$PWD/utils/TopologyTypeUtil.cpp

HEADERS += \
    $$PWD/ApplicationContext/ApplicationContext.h \
    $$PWD/AutonomousSystem/AutonomousSystem.h \
    $$PWD/DHCPServer/DHCPPayload.h \
    $$PWD/DHCPServer/DHCPServer.h \
    $$PWD/DataGenerator/ChunkServer.h \
    $$PWD/DataGenerator/DataGenerator.h \
    $$PWD/DataGenerator/DataPayload.h \
    $$PWD/EventCoordinator/EventCoordinator.h \
    $$PWD/ForwardingTable/ForwardingEntry.h \
    $$PWD/ForwardingTable/ForwardingTable.h \
    $$PWD/Globals/Globals.h \
    $$PWD/Globals/Types.h \
    $$PWD/IP/IP.h \
    $$PWD/IP/IPv4.h \
    $$PWD/IP/IPv6.h \
    $$PWD/IPDatabase/IPDatabase.h \
    $$PWD/LinkState/LinkState.h \
    $$PWD/LinkState/LinkStateDatabase.h \
    $$PWD/LinkState/LinkStatePayload.h \
    $$PWD/MACAddress/MACAddress.h \
    $$PWD/MACAddress/MACAddressGenerator.h \
    $$PWD/Network/Network.h \
    $$PWD/Node/Node.h \
    $$PWD/Node/PC.h \
    $$PWD/Node/Router.h \
    $$PWD/Packet/Packet.h \
    $$PWD/PacketHeaders/BGPHeader.h \
    $$PWD/PacketHeaders/DataLinkHeader.h \
    $$PWD/PacketHeaders/IPv4Header.h \
    $$PWD/PacketHeaders/IPv6Header.h \
    $$PWD/PacketHeaders/TCPHeader.h \
    $$PWD/Payload/Payload.h \
    $$PWD/Port/NetworkInterface.h \
    $$PWD/PortBindingManager/PortBindingManager.h \
    $$PWD/Port/Port.h \
    $$PWD/RoutingProtocols/BGP.h \
    $$PWD/RoutingProtocols/OSPF.h \
    $$PWD/RoutingProtocols/RIP.h \
    $$PWD/RoutingProtocols/RoutingUpdate.h \
    $$PWD/SpanningTree/SpanningTreeConfig.h \
    $$PWD/TCP/TCP.h \
    $$PWD/TCP/TCPPayload.h \
    $$PWD/utils/ConfigParser.h \
    $$PWD/utils/PortUtil.h \
    $$PWD/utils/RoutingProtocolTypeUtil.h \
    $$PWD/utils/TopologyTypeUtil.h

DISTFILES +=

