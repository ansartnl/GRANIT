include(../share/config.pri)

DEFINES += NETSNMPCLIENT_LIBRARY

QT       -= gui

TARGET = netSnmpClient
TEMPLATE = lib
DESTDIR = $$PWD/lib
DESTDIR = $$join(DESTDIR,,,$$POSTFIX)

I_PATH = $$PWD/include $$PWD/src $$PWD/src/processors
INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

SOURCES += \
    src/INetSnmpDataSource.cpp \
    src/NetSnmpClient.cpp \
    src/NetSnmpClientPrivate.cpp \
    src/RequestHandler.cpp \
    src/processors/GetValueProcessor.cpp \
    src/processors/HandshakeProcessor.cpp \
    src/processors/PingProcessor.cpp \
    src/processors/SetValueProcessor.cpp

HEADERS += \
    include/INetSnmpDataSource.h \
    include/NetSnmpClient.h \
    include/netSnmpClient_global.h \
    include/Vars.h \
    src/NetSnmpClientPrivate.h \
    src/RequestHandler.h \
    src/processors/GetValueProcessor.h \
    src/processors/HandshakeProcessor.h \
    src/processors/PingProcessor.h \
    src/processors/SetValueProcessor.h

include(../share/interaction/interaction.pri)
include(../share/localsocket/localsocket.pri)
include(../share/pinger/pinger.pri)
include(../share/requesthandler/requesthandler.pri)

include(../../common/enumstreaming/enumstreaming.pri)

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEF3A2C78
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = netSnmpClient.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

!exists(\"$$BUILD_DIR/lib\") : system(mkdir \"$$BUILD_DIR/lib\")

QMAKE_POST_LINK = $(COPY_FILE) "$$DESTDIR/*" "$$BUILD_DIR/lib"
