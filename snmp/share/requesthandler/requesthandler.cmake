get_filename_component(requesthandler_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${requesthandler_CURRENTDIR}")

if(NOT COMMAND export_group)
    list(APPEND HEADERS 
        "${requesthandler_CURRENTDIR}/DefaultRequestProcessor.h"
        "${requesthandler_CURRENTDIR}/IRequestProcessor.h"
        "${requesthandler_CURRENTDIR}/RequestProcessorFactory.h"
        "${requesthandler_CURRENTDIR}/RequestStructs.h")
    list(APPEND SOURCES 
        "${requesthandler_CURRENTDIR}/DefaultRequestProcessor.cpp"
        "${requesthandler_CURRENTDIR}/IRequestProcessor.cpp"
        "${requesthandler_CURRENTDIR}/RequestProcessorFactory.cpp"
        "${requesthandler_CURRENTDIR}/RequestStructs.cpp")
else()
    export_group("requesthandler" "requesthandler" 
        DefaultRequestProcessor.h
        DefaultRequestProcessor.cpp
        IRequestProcessor.h
        IRequestProcessor.cpp
        RequestProcessorFactory.h
        RequestProcessorFactory.cpp
        RequestStructs.h
        RequestStructs.cpp
    )
endif()
