DEFINES += AFTN_NAMESPACE

INCLUDEPATH += \
    $$PWD/share \
    $$PWD/parsers \
    $$PWD/telegrams

HEADERS += \
    $$PWD/share/ParseErrors.h \
    $$PWD/share/ParseResult.h \
    $$PWD/share/ParseVersion.h \
    $$PWD/share/TelegramTypes.h \
    $$PWD/share/Share.h \
    $$PWD/telegrams/Telegram.h \
    $$PWD/telegrams/ChTelegram.h \
    $$PWD/telegrams/AftnTelegram.h \
    $$PWD/telegrams/ResTelegram.h \
    $$PWD/telegrams/RtcTelegram.h \
    $$PWD/parsers/TelegramParser.h 

SOURCES += \
    $$PWD/telegrams/Telegram.cpp \
    $$PWD/telegrams/ChTelegram.cpp \
    $$PWD/telegrams/AftnTelegram.cpp \
    $$PWD/telegrams/ResTelegram.cpp \
    $$PWD/telegrams/RtcTelegram.cpp \
    $$PWD/parsers/TelegramParser.cpp 
	
include(../enumstreaming/enumstreaming.pri)
