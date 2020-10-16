get_filename_component(AFTNPARSER_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

# add_definitions(-DAFTN_NAMESPACE)

include_directories("${AFTNPARSER_CURRENTDIR}/share")
include_directories("${AFTNPARSER_CURRENTDIR}/parsers")
include_directories("${AFTNPARSER_CURRENTDIR}/telegrams")
    
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/ParseErrors.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/ParseResult.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/ParseVersion.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/TelegramTypes.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/Share.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/Transliteration.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/share/Transliteration.cpp")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/telegrams/Telegram.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/telegrams/ChTelegram.h")
list(APPEND HEADERS "${AFTNPARSER_CURRENTDIR}/telegrams/AftnTelegram.h")

list(APPEND Q_HEADERS "${AFTNPARSER_CURRENTDIR}/parsers/TelegramParser.h")

list(APPEND SOURCES "${AFTNPARSER_CURRENTDIR}/telegrams/Telegram.cpp")
list(APPEND SOURCES "${AFTNPARSER_CURRENTDIR}/telegrams/ChTelegram.cpp")
list(APPEND SOURCES "${AFTNPARSER_CURRENTDIR}/telegrams/AftnTelegram.cpp")
list(APPEND SOURCES "${AFTNPARSER_CURRENTDIR}/parsers/TelegramParser.cpp")

include("${AFTNPARSER_CURRENTDIR}/../enumstreaming/enumstreaming.cmake")
