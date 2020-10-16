QT += network

I_PATH = $$PWD

INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

HEADERS += \
    $$PWD/LocalSocket.h \
    $$PWD/private/IncomingDataHandler.h

SOURCES += \
    $$PWD/LocalSocket.cpp \
    $$PWD/private/IncomingDataHandler.cpp
