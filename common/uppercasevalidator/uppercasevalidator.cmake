get_filename_component(UPPERCASEVALIDATOR_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${UPPERCASEVALIDATOR_CURRENTDIR}")

list(APPEND Q_HEADERS "${UPPERCASEVALIDATOR_CURRENTDIR}/uppercasevalidator.h")

list(APPEND SOURCES "${UPPERCASEVALIDATOR_CURRENTDIR}/uppercasevalidator.cpp")

