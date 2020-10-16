include(../share/config.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testManager
TARGET = $$join(TARGET,,,$$POSTFIX)
TEMPLATE = app
DESTDIR = $$BUILD_DIR/bin

SOURCES += src/main.cpp\
    src/ManagerWindow.cpp

HEADERS += \
    src/ManagerWindow.h

FORMS += \
    src/ManagerWindow.ui

include(../netSnmpManager/netSnmpManager.pri)

include(../../common/enumstreaming/enumstreaming.pri)
