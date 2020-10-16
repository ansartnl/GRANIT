include(../share/config.pri)

QT       += core network
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

APP_NAME = netSnmpTrapHandler
TARGET = $$join(APP_NAME,,,$$POSTFIX)
TEMPLATE = app
DESTDIR = $$BUILD_DIR/bin

SOURCES += src/main.cpp

include(../share/trap_interaction/trap_interaction.pri)

include(../../common/initmain/initmain.pri)
include(../../common/log/log.pri)
include(../../common/log4qt/src/Log4Qt.pri)
