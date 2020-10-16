get_filename_component(interaction_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${interaction_CURRENTDIR}")

if(NOT COMMAND export_group)
    list(APPEND HEADERS 
        "${interaction_CURRENTDIR}/GetValue.h"
        "${interaction_CURRENTDIR}/Handshake.h"
        "${interaction_CURRENTDIR}/Interaction.h"
        "${interaction_CURRENTDIR}/Notify.h"
        "${interaction_CURRENTDIR}/PingPong.h"
        "${interaction_CURRENTDIR}/SetValue.h")
    list(APPEND SOURCES "${interaction_CURRENTDIR}/Interaction.cpp")
else()
    export_group("interaction" "interaction" 
        GetValue.h
        Handshake.h
        Interaction.h
        Notify.h
        Interaction.cpp
        PingPong.h
        SetValue.h
    )
endif()
