QT     += core
CONFIG += no_keywords
CONFIG += c++20 console

# QMAKE_LFLAGS += -fuse-ld=lld


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    .clang-format \
    .clang-tidy \
    .gitignore \
    assets/main/config.json \
    assets/test/tst_config.json \
    assets/test/tst_ForwardingConfig.json


include($$PWD/src/main/main.pri)
# include($$PWD/src/test/test.pri)

RESOURCES += \
    Configs.qrc \
    Files.qrc

DEFINES += UT_SOURCE_DIR="\\\"$${PWD}\\\""
