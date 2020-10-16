get_filename_component(arinc_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${arinc_CURRENTDIR}")

if(NOT COMMAND export_group)
	list(APPEND HEADERS 
		"${cmdline_CURRENTDIR}/A620Message.h"
		"${cmdline_CURRENTDIR}/A622Message.h"
		"${cmdline_CURRENTDIR}/TypeBMessage.h"
		)
	list(APPEND SOURCES 
		"${cmdline_CURRENTDIR}/A620Message.cpp"
		"${cmdline_CURRENTDIR}/A622Message.cpp"
		"${cmdline_CURRENTDIR}/TypeBMessage.cpp"
)
else()
    export_group("arinc" "arinc"
		A620Message.h
		A620Message.cpp
		A622Message.h
		A622Message.cpp
		TypeBMessage.h
		TypeBMessage.cpp
	)
endif()
