include(../share/config.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testClient
TARGET = $$join(TARGET,,,$$POSTFIX)
TEMPLATE = app
DESTDIR = $$BUILD_DIR/bin

SOURCES += src/main.cpp\
    src/ClientWindow.cpp \
    src/TestNetSnmpDataSource.cpp

HEADERS += \
    src/ClientWindow.h \
    src/TestNetSnmpDataSource.h

FORMS += \
    src/ClientWindow.ui

include(../netSnmpClient/netSnmpClient.pri)
