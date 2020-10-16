get_filename_component(afnparser_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${afnparser_CURRENTDIR}")
    
if(NOT COMMAND export_group)
	list(APPEND HEADERS 
		"${afnparser_CURRENTDIR}/AckMessage.h"
		"${afnparser_CURRENTDIR}/CompleteMessage.h"
		"${afnparser_CURRENTDIR}/ContactAdvisoryMessage.h"
		"${afnparser_CURRENTDIR}/ContactMessage.h"
		"${afnparser_CURRENTDIR}/Header.h"
		"${afnparser_CURRENTDIR}/MessageTypeIdentification.h"
		"${afnparser_CURRENTDIR}/ResponseMessage.h")
	list(APPEND SOURCES 
		"${afnparser_CURRENTDIR}/AckMessage.cpp"
		"${afnparser_CURRENTDIR}/CompleteMessage.cpp"
		"${afnparser_CURRENTDIR}/ContactAdvisoryMessage.cpp"
		"${afnparser_CURRENTDIR}/ContactMessage.cpp"
		"${afnparser_CURRENTDIR}/Header.cpp"
		"${afnparser_CURRENTDIR}/ResponseMessage.cpp")
else()
	export_group("afnparser" "afnparser" 
		AckMessage.h
		AckMessage.cpp
		CompleteMessage.h
		CompleteMessage.cpp
		ContactAdvisoryMessage.h
		ContactAdvisoryMessage.cpp
		ContactMessage.cpp
		ContactMessage.h
		Header.h
		Header.cpp
		MessageTypeIdentification.h
		ResponseMessage.h
		ResponseMessage.cpp
	)
endif()