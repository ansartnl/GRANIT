#-------------------------------------------------
#
# Project created by QtCreator 2014-06-03T10:51:54
#
#-------------------------------------------------

QT       += core gui script sql

equals(QT_MAJOR_VERSION, 4) {
    QT += phonon
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += dbus printsupport widgets
    QMAKE_CXXFLAGS += -std=c++11
    LIBS += -lphonon4qt5
}

DESTDIR = $$PWD/bin
TARGET = SupervisorMonitor
TEMPLATE = app

INCLUDEPATH += ../../common/managers/printactionsmanager

SOURCES += src/main.cpp\
        src/SupervisorView.cpp \
    src/SupervisorWidget.cpp \
    src/SupervisorMonitor.cpp \
    src/datasources/NetworkDataSource.cpp \
    src/connections/supervisorServer/MonitoringSocket.cpp \
    src/datasources/GroupsDataSource.cpp \
    src/datasources/HostDataSource.cpp \
    src/dialogs/ScriptProcessDialog.cpp \
    src/ClickableLabel.cpp \
    src/models/MonitoringModel.cpp \
    src/dialogs/LogDialog.cpp \
    ../../common/managers/printactionsmanager/printactionsmanager.cpp \
    ../../common/managers/printactionsmanager/printactionsmanagerhelper.cpp \
    src/LabelAction.cpp \
    src/dialogs/LogCollectorDialog.cpp \
    src/dialogs/EventLogDialog.cpp \
    src/models/EventLogModel.cpp \
    src/models/EventLogFilterModel.cpp \
    src/AlarmPlayer.cpp \
    src/connections/asterix253/AsterixListener253.cpp \
    src/datasources/Asterix253MORLIDataSource.cpp \
    src/datasources/Asterix253RLKDataSource.cpp \
    src/SQLiteManager.cpp \
    src/SMCLogManager.cpp \
    src/datasources/FolderDataSource.cpp \
    src/datasources/SnmpDataSource.cpp

HEADERS  += src/SupervisorView.h \
    src/SupervisorWidget.h \
    src/SupervisorMonitor.h \
    src/main.h \
    src/datasources/NetworkDataSource.h \
    src/connections/supervisorServer/MonitoringSocket.h \
    src/datasources/IDatasource.h \
    src/datasources/GroupsDataSource.h \
    src/datasources/HostDataSource.h \
    src/dialogs/ScriptProcessDialog.h \
    src/ClickableLabel.h \
    src/models/MonitoringModel.h \
    src/dialogs/LogDialog.h \
    ../../common/managers/printactionsmanager/printactionsmanager.h \
    ../../common/managers/printactionsmanager/printactionsmanagerhelper.h \
    ../../common/managers/printactionsmanager/private/printactionsmanager_p.h \
    src/LabelAction.h \
    src/dialogs/LogCollectorDialog.h \
    src/dialogs/EventLogDialog.h \
    src/models/EventLogModel.h \
    src/models/EventLogFilterModel.h \
    src/AlarmPlayer.h \
    src/connections/asterix253/Asterix253Struct.h \
    src/connections/asterix253/AsterixListener253.h \
    src/connections/IConnection.h \
    src/datasources/Asterix253MORLIDataSource.h \
    src/datasources/Asterix253RLKDataSource.h \
    src/SQLiteManager.h \
    src/SMCLogManager.h \
    src/datasources/FolderDataSource.h \
    src/datasources/SnmpDataSource.h

include(../../common/asterix/asterix.pri)
include(../../common/initmain/initmain.pri)
include(../../common/log4qt/src/Log4Qt.pri)
include(../../common/log/log.pri)
include(../../common/qtlanguagemenu/qtlanguagemenu.pri)
include(../../common/qtsocketautoconnector/qtsocketautoconnector.pri)
include(../../common/qttcpconnectionchecker/qttcpconnectionchecker.pri)
include(../../common/qtvariantconnection/qtvariantconnection.pri)
include(../../common/usersystem/client/UserClient.pri)
include(../../common/usersystem/proto/UserProto.pri)
include(../../common/usersystem/server/UserServer.pri)
include(../../common/utils/utils.pri)
include(../../common/qtxmlsettings/qtxmlsettings.pri)
include(../proto/proto.pri)

FORMS += \
    src/SupervisorMonitor.ui \
    src/dialogs/ScriptProcessDialog.ui \
    src/dialogs/LogDialog.ui \
    src/dialogs/LogCollectorDialog.ui \
    src/dialogs/EventLogDialog.ui
