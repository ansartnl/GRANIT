get_filename_component(localsocket_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${localsocket_CURRENTDIR}" "${localsocket_CURRENTDIR}/private")

if(NOT COMMAND export_group)
    list(APPEND HEADERS 
        "${localsocket_CURRENTDIR}/LocalSocket.h"
        "${localsocket_CURRENTDIR}/private/IncomingDataHandler.h")
    list(APPEND SOURCES 
        "${localsocket_CURRENTDIR}/LocalSocket.cpp"
        "${localsocket_CURRENTDIR}/private/IncomingDataHandler.cpp")
else()
    export_group("localsocket" "localsocket" 
        LocalSocket.h
        LocalSocket.cpp
    )
    export_group("localsocket_private" "localsocket/private" 
        private/IncomingDataHandler.h
        private/IncomingDataHandler.cpp
    )
endif()
