get_filename_component(pinger_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${pinger_CURRENTDIR}")

if(NOT COMMAND export_group)
    list(APPEND HEADERS "${pinger_CURRENTDIR}/Pinger.h")
    list(APPEND SOURCES "${pinger_CURRENTDIR}/Pinger.cpp")
else()
    export_group("pinger" "pinger" 
        Pinger.h
        Pinger.cpp
    )
endif()
