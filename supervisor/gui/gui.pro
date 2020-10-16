#-------------------------------------------------
#
# Project created by QtCreator 2011-04-11T16:39:06
#
#-------------------------------------------------

TARGET = SupervisorMonitor
CONFIG(debug, debug|release) : TARGET = $$join(TARGET,,,_debug)

DESTDIR = $$PWD/bin

QT       += core gui
TEMPLATE = app

INCLUDEPATH += src/ src/models

INCLUDEPATH += ../../common/managers/printactionsmanager

SOURCES += \
        src/main.cpp \
        src/MainWindow.cpp \
        src/MonitoringSocket.cpp \
        src/models/HostsModel.cpp \
        src/models/MonitoringModel.cpp \
        src/models/GroupsModel.cpp \
        src/LogDialog.cpp \
        src/init.cpp \
        ../../common/managers/printactionsmanager/printactionsmanager.cpp \
        ../../common/managers/printactionsmanager/printactionsmanagerhelper.cpp \
    src/RemoteOperationsDialog.cpp \
    src/FontColorsDialog.cpp \
    src/AppSettings.cpp \
    src/models/OperationsModel.cpp \
    src/models/OperationsSortFilterModel.cpp \
    src/LogCollectorDialog.cpp \
    src/GroupConfigDialog.cpp

HEADERS  += \
        src/main.h \
        src/MainWindow.h \
        src/MonitoringSocket.h \
        src/models/HostsModel.h \
        src/models/MonitoringModel.h \
        src/models/GroupsModel.h \
        src/LogDialog.h \
        src/init.h \
        ../../common/managers/printactionsmanager/printactionsmanager.h \
        ../../common/managers/printactionsmanager/printactionsmanagerhelper.h \
        ../../common/managers/printactionsmanager/private/printactionsmanager_p.h \
    src/RemoteOperationsDialog.h \
    src/FontColorsDialog.h \
    src/AppSettings.h \
    src/models/OperationsModel.h \
    src/models/OperationsSortFilterModel.h \
    src/LogCollectorDialog.h \
    src/GroupConfigDialog.h

FORMS    += \
        src/MainWindow.ui \
        src/LogDialog.ui \
    src/RemoteOperationsDialog.ui \
    src/FontColorsDialog.ui \
    src/LogCollectorDialog.ui \
    src/GroupConfigDialog.ui

RESOURCES += \
    share/Resource.qrc
RC_FILE = share/Resource.rc

include(../proto/proto.pri)
include(../../common/version/version.pri)
include(../../common/initmain/initmain.pri)
include(../../common/log4qt/src/Log4Qt.pri)
include(../../common/log/log.pri)
include(../../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../../common/qtsocketautoconnector/qtsocketautoconnector.pri)
include(../../common/qttcpconnectionchecker/qttcpconnectionchecker.pri)
include(../../common/qtvariantconnection/qtvariantconnection.pri)
include(../../common/qtxmlsettings/qtxmlsettings.pri)
include(../../common/usersystem/client/UserClient.pri)
include(../../common/usersystem/proto/UserProto.pri)
include(../../common/usersystem/server/UserServer.pri)
include(../../common/styleEditor/ExtendedStyle.pri)
include(../../common/sound/sound.pri)

TRANSLATIONS = share/translations/SupervisorMonitor_ru.ts

# Installation procedure

FILE_EXTENSION =
win32:FILE_EXTENSION = .exe

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

package.path = /package/bin
package.files = $${OUT_PWD}/$${CONFIGURATION}/$${TARGET}$${FILE_EXTENSION}

translations.path = /package/share/translations
translations.files = $${PWD}/share/translations/*_ru.qm

config.path = /package/etc
config.files = $${PWD}/etc/config.xml

INSTALLS += package translations config





