QT += core testlib
CONFIG += lrelease
CONFIG += no_keywords
CONFIG += c++20

# INCLUDEPATH += $$PWD\Globals

SOURCES += \
    $$PWD/tst_BGP.cpp \
    $$PWD/tst_DHCPServer.cpp \
    $$PWD/tst_Forwarding.cpp \
    $$PWD/tst_OSPF.cpp \
    $$PWD/tst_PortBinding.cpp \
    $$PWD/tst_RIP.cpp \
    $$PWD/tst_SpanningTree.cpp \
    $$PWD/tst_main.cpp


DISTFILES +=

HEADERS += \
    $$PWD/tst_BGP.h \
    $$PWD/tst_DHCPServer.h \
    $$PWD/tst_Forwarding.h \
    $$PWD/tst_OSPF.h \
    $$PWD/tst_PortBinding.h \
    $$PWD/tst_RIP.h \
    $$PWD/tst_SpanningTree.h

