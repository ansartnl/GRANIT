INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS       += $$PWD/SitaConnection.h

SOURCES       += $$PWD/SitaConnection.cpp

include(matip/matip.pri)
include(batap/batap.pri)
include(arinc/arinc.pri)
include(afnparser/afnparser.pri)
include(cpdlc/cpdlc.pri)