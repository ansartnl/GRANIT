#-------------------------------------------------
#
# Project created by QtCreator 2011-01-24T15:31:25
#
#-------------------------------------------------
TARGET = SupervisorServer
CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

# CONFIG += ss_no_gui

DESTDIR = $$PWD/bin

TEMPLATE = app

QT       += gui core network sql
ss_no_gui {
QT       -= gui
DEFINES += SS_NO_GUI
}

CONFIG   += thread

greaterThan(QT_MAJOR_VERSION, 4) {
    QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += src src/helpers src/controllers

SOURCES += \
        src/ClientConnection.cpp \
        src/common.cpp \
        src/init.cpp \
        src/main.cpp \
        src/SupervisorService.cpp \
        src/RestartInterval.cpp \
        src/controllers/Controller.cpp \
        src/controllers/FolderController.cpp \
        src/controllers/NetController.cpp \
        src/controllers/ProcessController.cpp \
        src/controllers/ScriptController.cpp \
        src/controllers/ServiceController.cpp \
        src/controllers/SnmpController.cpp \
        src/helpers/ComputerHelper.cpp \
        src/helpers/GroupsHelper.cpp \
        src/helpers/DbConnThread.cpp \
        src/helpers/DatabaseController.cpp \
        src/helpers/ApplicationController.cpp \
        src/helpers/CWPController.cpp \
        src/helpers/FDPControllers.cpp \
        src/helpers/FMTPControllers.cpp \
        src/helpers/ADPControllers.cpp \
        src/helpers/RequestThread.cpp \
        src/helpers/LogController.cpp

HEADERS += \
        src/ClientConnection.h \
        src/common.h \
        src/init.h \
        src/SupervisorService.h \
        src/RestartInterval.h \
        src/controllers/Controller.h \
        src/controllers/FolderController.h \
        src/controllers/NetController.h \
        src/controllers/ProcessController.h \
        src/controllers/ScriptController.h \
        src/controllers/ServiceController.h \
        src/controllers/SnmpController.h \
        src/helpers/ComputerHelper.h \
        src/helpers/GroupsHelper.h \
        src/helpers/DbConnThread.h \
        src/helpers/DatabaseController.h \
        src/helpers/ApplicationController.h \
        src/helpers/CWPController.h \
        src/helpers/FDPControllers.h \
        src/helpers/FMTPControllers.h \
        src/helpers/ADPControllers.h \
        src/helpers/RequestThread.h \
        src/helpers/LogController.h

include(../proto/proto.pri)
include(../../3rdparty/qtservice/src/qtservice.pri)
include(../../common/version/version.pri)
include(../../common/enumstreaming/enumstreaming.pri)
include(../../common/initmain/initmain.pri)
include(../../common/log4qt/src/Log4Qt.pri)
include(../../common/log/log.pri)
include(../../common/pulsar/pulsar.pri)
ss_no_gui {
include(../../3rdparty/qtsingleapplication/src/qtsinglecoreapplication.pri)
} else {
include(../../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../../3rdparty/qtsingleapplication/src/qtsingleapplication.pri)
}
include(../../common/qttcpconnectionchecker/qttcpconnectionchecker.pri)
include(../../common/qtvariantconnection/qtvariantconnection.pri)
include(../../common/qtxmlsettings/qtxmlsettings.pri)
include(../../common/scriptrunner/scriptrunner.pri)

include(../../snmp/netSnmpManager/netSnmpManager.pri)

TRANSLATIONS = share/translations/supervisorserver_ru.ts

# Installation procedure

FILE_EXTENSION =
win32:FILE_EXTENSION = .exe

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

package.path = /package/bin
package.files = $${PWD}/bin/$${TARGET}$${FILE_EXTENSION}

translations.path = /package/share/translations
translations.files = $${PWD}/share/translations/*_ru.qm

config.path = /package/etc
config.files = $${PWD}/etc/config.xml

INSTALLS += package translations config
