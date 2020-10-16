INCLUDEPATH   += $$PWD
DEPENDPATH    += $$PWD

HEADERS += 	$$PWD/AckMessage.h \
            $$PWD/CompleteMessage.h \
            $$PWD/ContactAdvisoryMessage.h \
            $$PWD/ContactMessage.h \
            $$PWD/Header.h \
            $$PWD/MessageTypeIdentification.h \
            $$PWD/ResponseMessage.h

SOURCES +=  $$PWD/AckMessage.cpp \
            $$PWD/CompleteMessage.cpp \
            $$PWD/ContactAdvisoryMessage.cpp \
            $$PWD/ContactMessage.cpp \
            $$PWD/Header.cpp \
            $$PWD/ResponseMessage.cpp
