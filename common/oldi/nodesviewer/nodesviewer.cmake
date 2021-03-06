get_filename_component(NODESVIEWERCURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${NODESVIEWERCURRENTDIR}")
    
list(APPEND Q_HEADERS "${NODESVIEWERCURRENTDIR}/NodesViewer.h")

list(APPEND SOURCES "${NODESVIEWERCURRENTDIR}/NodesViewer.cpp")

list(APPEND FORMS "${NODESVIEWERCURRENTDIR}/NodesViewer.ui")

message("OLDI nodes viewer.")