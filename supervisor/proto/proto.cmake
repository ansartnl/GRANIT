get_filename_component(spproto_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${spproto_CURRENTDIR}")

list(APPEND SOURCES 
	"${spproto_CURRENTDIR}/SpProto.cpp"
	)

list(APPEND HEADERS 
	"${spproto_CURRENTDIR}/SpCommands.h"
	"${spproto_CURRENTDIR}/SpCommon.h"
	"${spproto_CURRENTDIR}/SpProto.h"
	"${spproto_CURRENTDIR}/SpLog.h"
	"${spproto_CURRENTDIR}/SpRequest.h"
	"${spproto_CURRENTDIR}/SpService.h"
	"${spproto_CURRENTDIR}/SpStatus.h"
	)
