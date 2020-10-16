get_filename_component(localserver_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${localserver_CURRENTDIR}")

if(NOT COMMAND export_group)
    list(APPEND HEADERS 
        "${localserver_CURRENTDIR}/LocalServer.h")
    list(APPEND SOURCES 
        "${localserver_CURRENTDIR}/LocalServer.cpp")
else()
    export_group("localserver" "localserver" 
        LocalServer.h
        LocalServer.cpp
    )
endif()
