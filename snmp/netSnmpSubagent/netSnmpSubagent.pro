include(../share/config.pri)

QT       += core
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

APP_NAME = netSnmpSubagent
TARGET = $$join(APP_NAME,,,$$POSTFIX)
TEMPLATE = app
DESTDIR = $$BUILD_DIR/bin

#-lnetsnmpmibs
LIBS += -lnetsnmpagent -lnetsnmp -lnetsnmphelpers
win32 : {
LIBS += -lws2_32 -liphlpapi -lsnmpapi
}

# main

I_PATH = $$PWD/src

SOURCES += src/main.cpp
HEADERS += src/main.h

# helpers

SOURCES += \
    src/helpers/Config.cpp

HEADERS += \
    src/helpers/Config.h \
    src/helpers/ConfigKeys.h \
    src/helpers/Timeval.h

# mib-objects

I_PATH += $$PWD/src/mib-objects

SOURCES += \
    src/mib-objects/BaseMibEntity.cpp \
    src/mib-objects/BaseMibObject.cpp \
    src/mib-objects/CallbackHandler.cpp \
    src/mib-objects/Counter32MibObject.cpp \
    src/mib-objects/Counter64MibObject.cpp \
    src/mib-objects/DateAndTimeMibObject.cpp \
    src/mib-objects/DateAndTimeWithZoneMibObject.cpp \
    src/mib-objects/IntegerMibObject.cpp \
    src/mib-objects/IpAddressMibObject.cpp \
    src/mib-objects/ProgramStateMibObject.cpp \
    src/mib-objects/ScriptMibObject.cpp \
    src/mib-objects/StringMibObject.cpp \
    src/mib-objects/String2MibObject.cpp \
    src/mib-objects/TrapMibObject.cpp \
    src/mib-objects/ValueMibObject.cpp \
    src/mib-objects/ValueMibObjectFactory.cpp

HEADERS += \
    src/mib-objects/BaseMibEntity.h \
    src/mib-objects/BaseMibObject.h \
    src/mib-objects/CallbackHandler.h \
    src/mib-objects/Counter32MibObject.h \
    src/mib-objects/Counter64MibObject.h \
    src/mib-objects/DateAndTimeMibObject.h \
    src/mib-objects/DateAndTimeWithZoneMibObject.h \
    src/mib-objects/IntegerMibObject.h \
    src/mib-objects/IpAddressMibObject.h \
    src/mib-objects/ProgramStateMibObject.h \
    src/mib-objects/ScriptMibObject.h \
    src/mib-objects/StringMibObject.h \
    src/mib-objects/String2MibObject.h \
    src/mib-objects/TrapMibObject.h \
    src/mib-objects/ValueMibObject.h \
    src/mib-objects/ValueMibObjectFactory.h

# internal request processors

I_PATH += $$PWD/src/processors

SOURCES += \
    src/processors/HandshakeProcessor.cpp \
    src/processors/NotifyProcessor.cpp \
    src/processors/PingProcessor.cpp \
    src/processors/SetValueResultProcessor.cpp \
    src/processors/ValueProcessor.cpp

HEADERS += \
    src/processors/HandshakeProcessor.h \
    src/processors/NotifyProcessor.h \
    src/processors/PingProcessor.h \
    src/processors/SetValueResultProcessor.h \
    src/processors/ValueProcessor.h

# services

SOURCES += \
    src/ClientObject.cpp \
    src/InternalRequestHandler.cpp \
    src/InternalRunnable.cpp \
    src/InternalServer.cpp \
    src/MainService.cpp \
    src/Service.cpp \
    src/ServiceStarter.cpp \
    src/Subagent.cpp \
    src/SubagentLoop.cpp \
    src/SubagentObjects.cpp

HEADERS += \
    src/ClientObject.h \
    src/InternalRequestHandler.h \
    src/InternalRunnable.h \
    src/InternalServer.h \
    src/MainService.h \
    src/Service.h \
    src/ServiceStarter.h \
    src/Subagent.h \
    src/SubagentLoop.h \
    src/SubagentObjects.h

INCLUDEPATH += $$I_PATH
DEPENDPATH  += $$I_PATH

include(../share/interaction/interaction.pri)
include(../share/localserver/localserver.pri)
include(../share/localsocket/localsocket.pri)
include(../share/pinger/pinger.pri)
include(../share/requesthandler/requesthandler.pri)

include(../../common/enumstreaming/enumstreaming.pri)
include(../../common/initmain/initmain.pri)
include(../../common/log/log.pri)
include(../../common/log4qt/src/Log4Qt.pri)
include(../../common/qtservice/qtservice.pri)
include(../../common/scriptrunner/scriptrunner.pri)
include(../../common/version/version.pri)

nita_xml : {
    include(../../common/nitaxmlsettings/nitaxmlsettings.pri)
    CONFIG_NAME = $$join(APP_NAME,,,_nita)
    CONF_MASK = conf_*.xml
} else {
    include(../../common/qtxmlsettings/qtxmlsettings.pri)
    CONFIG_NAME = $$APP_NAME
    CONF_MASK = *.conf
}

QMAKE_POST_LINK = \
    $(COPY_FILE) "$$PWD/etc/$$CONF_MASK" "$$BUILD_DIR/etc"$$escape_expand(\\n\\t) \
    $(COPY_FILE) "$$PWD/etc/$$CONFIG_NAME'.'xml" "$$BUILD_DIR/etc/$$TARGET'.'xml"
