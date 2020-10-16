INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

CONFIG(debug, debug|release) {
    DEFINES += MATIP_DEBUG
}

HEADERS       += $$PWD/MatipHeader.h \
                 $$PWD/Matip.h

SOURCES       += $$PWD/MatipHeader.cpp \
                 $$PWD/Matip.cpp

