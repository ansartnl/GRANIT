I_PATH = $$PWD

INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

HEADERS += \
    $$PWD/GetValue.h \
    $$PWD/Handshake.h \
    $$PWD/Interaction.h \
    $$PWD/Notify.h \
    $$PWD/PingPong.h \
    $$PWD/SetValue.h

SOURCES += \
    $$PWD/Interaction.cpp
