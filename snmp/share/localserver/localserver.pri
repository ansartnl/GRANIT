QT += network

I_PATH = $$PWD

INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

HEADERS += $$PWD/LocalServer.h

SOURCES += $$PWD/LocalServer.cpp
