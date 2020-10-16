#-------------------------------------------------
#
# Project created by QtCreator 2012-11-16T11:34:31
#
#-------------------------------------------------

QT       += core gui
DESTDIR = $$PWD/bin
TARGET = aftncatalogorganizer
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/catalogorganizer.cpp

HEADERS  += src/mainwindow.h \
            src/catalogorganizer.h

FORMS    += src/mainwindow.ui

include(../qtxmlsettings/qtxmlsettings.pri)
include(../cmdline/cmdline.pri)
