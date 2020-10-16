INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/Batap.h \
                 $$PWD/BatapMessageProducer.h \
                 $$PWD/BatapMessageStorage.h

SOURCES       += $$PWD/Batap.cpp \
                 $$PWD/BatapMessageProducer.cpp

include(redis/redis.pri)
