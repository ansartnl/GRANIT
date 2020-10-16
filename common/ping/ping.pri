INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/Ping.h \
    $$PWD/PingPrivate.h \
    $$PWD/IcmpSocket.h \
    $$PWD/WinStructs.h

SOURCES += \
    $$PWD/Ping.cpp \
    $$PWD/PingPrivate.cpp \
    $$PWD/IcmpSocket.cpp

DEFINES += MAX_PING_DATA_SIZE=1024

win32:LIBS += -lws2_32
