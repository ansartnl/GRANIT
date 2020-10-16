QT       += core

INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/InitMain.h
SOURCES       += $$PWD/InitMain.cpp

do_not_use_cmdline_arguments {
DEFINES += DO_NOT_USE_CMD_LINE_ARGUMENTS
} else {
include(../cmdline/cmdline.pri)
}
