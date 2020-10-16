include(../share/config.pri)

DEFINES += NETSNMPMANAGER_LIBRARY

QT       += network
QT       -= gui

TARGET = netSnmpManager
TEMPLATE = lib
DESTDIR = $$PWD/lib
#DESTDIR = $$join(DESTDIR,,,$$POSTFIX)

greaterThan(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}

LIBS += -lnetsnmp

I_PATH = $$PWD/include $$PWD/src
INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

SOURCES += \
    src/HostWatcher.cpp \
    src/NetSnmpManager.cpp \
    src/NetSnmpManagerPrivate.cpp \
    src/SnmpHelper.cpp

HEADERS += \
    include/NetSnmpManager.h \
    include/netSnmpManager_global.h \
    include/SnmpVariable.h \
    src/HostWatcher.h \
    src/NetSnmpManagerPrivate.h \
    src/SnmpHelper.h

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

include(../share/trap_interaction/trap_interaction.pri)

include(../../common/enumstreaming/enumstreaming.pri)

!exists(\"$$BUILD_DIR/lib\") : system(mkdir \"$$BUILD_DIR/lib\")

QMAKE_POST_LINK = $(COPY_FILE) "$$DESTDIR/*" "$$BUILD_DIR/lib"
