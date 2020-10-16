get_filename_component(trap_interaction_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${trap_interaction_CURRENTDIR}")

if(NOT COMMAND export_group)
    list(APPEND HEADERS 
        "${trap_interaction_CURRENTDIR}/TrapInteraction.h")
    list(APPEND SOURCES 
        "${trap_interaction_CURRENTDIR}/TrapInteraction.cpp")
else()
    export_group("trap_interaction" "trap_interaction" 
        TrapInteraction.h
        TrapInteraction.cpp
    )
endif()
