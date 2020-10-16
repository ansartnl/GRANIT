I_PATH = $$PWD

INCLUDEPATH += $$I_PATH
DEPENDPATH += $$I_PATH

HEADERS += \
    $$PWD/DefaultRequestProcessor.h \
    $$PWD/IRequestProcessor.h \
    $$PWD/RequestProcessorFactory.h \
    $$PWD/RequestStructs.h

SOURCES += \
    $$PWD/DefaultRequestProcessor.cpp \
    $$PWD/IRequestProcessor.cpp \
    $$PWD/RequestProcessorFactory.cpp \
    $$PWD/RequestStructs.cpp
