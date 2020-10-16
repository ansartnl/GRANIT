cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(FILES_TO_TRANSLATE)

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------

# Declare project
project(SupervisorMonitor)

set(CMAKE_INCLUDE_CURRENT_DIR true)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

find_package(Qt4 REQUIRED)
# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------
# Setup Qt
set(QT_USE_QTNETWORK TRUE)
# Create libraries list
include(${QT_USE_FILE})

# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------

# all 3rd party subdirectories ---------------------------------------------------------

add_subdirectory(../../common/qtlanguagemenu ${CMAKE_CURRENT_BINARY_DIR}/qtlanguagemenu)
include_directories(../../common/qtlanguagemenu)

add_subdirectory(../../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../../common/qtxmlsettings)

set(LOG4QT_BUILD_STATIC TRUE)
set(LOG4QT_DONT_INSTALL TRUE)
add_subdirectory(../../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
include_directories(../../common/log4qt/src)

include(../proto/proto.cmake)
include(../../common/qtsocketautoconnector/qtsocketautoconnector.cmake)
include(../../common/qttcpconnectionchecker/qttcpconnectionchecker.cmake)
include(../../common/qtvariantconnection/qtvariantconnection.cmake)
include(../../common/initmain/initmain.cmake)
include(../../common/log/log.cmake)
include(../../common/usersystem/usersystem.cmake)

# -----------------------------------------------------

message("Building project ${PROJECT_NAME}")
message("
Configuration
-------------
CMAKE_BUILD_TYPE        = ${CMAKE_BUILD_TYPE}
CXXFLAGS                = $ENV{CXXFLAGS}
CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}
LDFLAGS                 = $ENV{LDFLAGS}
CMAKE_EXE_LINKER_FLAGS  = ${CMAKE_EXE_LINKER_FLAGS}
CMAKE_INSTALL_PREFIX    = ${CMAKE_INSTALL_PREFIX}
")

if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_VERBOSE_MAKEFILE TRUE)
endif (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Release")

set(CMAKE_INCLUDE_CURRENT_DIR true)

# project version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_RELEASE 0)
set (VERSION_BUILD 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

add_definitions(-DVERSION_STR="${VERSION}")
add_definitions(-DAPP_VERSION="${VERSION}")

set(COMPANY_NAME "Itos Ltd.")
set(FILE_DESCRIPTION "${PROJECT_NAME}")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD}")
set(LEGAL_COPYRIGHT "All rights reserved")
set(PRODUCT_NAME "Supervisor Monitor")

# SOURCE FILES AND DIRECTORIES

# Get project sources
list(APPEND SOURCES
        src/init.cpp
        src/main.cpp
        src/MainWindow.cpp
        src/MonitoringSocket.cpp
        src/models/HostsModel.cpp
        src/models/MonitoringModel.cpp
        src/LogDialog.cpp
        src/GroupConfigDialog.cpp
        ../../common/managers/printactionsmanager/printactionsmanager.cpp
        ../../common/managers/printactionsmanager/printactionsmanagerhelper.cpp
  )

list(APPEND Q_HEADERS
        src/MainWindow.h
        src/MonitoringSocket.h
        src/models/HostsModel.h
        src/models/MonitoringModel.h
        src/LogDialog.h
        src/GroupConfigDialog.h
        ../../common/managers/printactionsmanager/printactionsmanager.h
        ../../common/managers/printactionsmanager/printactionsmanagerhelper.h
        ../../common/managers/printactionsmanager/private/printactionsmanager_p.h
  )

list(APPEND HEADERS
        src/init.h
        src/main.h
  )

list(APPEND FORMS
		src/MainWindow.ui
		src/LogDialog.ui
		src/GroupConfigDialog.ui		
  )

list(APPEND RESOURCES
    share/Resource.qrc
    )

# Walk throw file list to include all sources directories
foreach(HEADER_FILE ${Q_HEADERS} ${P_HEADERS} ${HEADERS} ${SOURCES} PARENT_SCOPE)
  get_filename_component(HEADER_PATH ${HEADER_FILE} PATH)
  include_directories(${HEADER_PATH})
endforeach(HEADER_FILE)
include_directories(src/ src/models ../../common/managers/printactionsmanager)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
#qt4_automoc(${Q_SOURCES})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

#create app icon
set(WIN_RESOURCES)
if(MINGW AND WIN32)
  # resource for mingw win32
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/share/SupervisorGui.rc.in ${CMAKE_CURRENT_BINARY_DIR}/share/SupervisorGui.rc)
  list(APPEND WIN_RESOURCES
    share/Resource.rc
    ${CMAKE_CURRENT_BINARY_DIR}/share/SupervisorGui.rc
    )
endif(MINGW AND WIN32)

# COMPILATION
add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
  ${Q_SOURCES}
  ${MOC_SOURCES}
  ${QRC_SOURCES}
  ${Q_HEADERS}
  ${HEADERS}
  ${UI_HEADERS}
  ${QS_SOURCES}
  ${WIN_RESOURCES})

################################################
# LINKING
################################################
#rename target
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug" VERSION ${VERSION})

# main library
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}
  qtlanguagemenu
  qtxmlsettings
  log4qt
  )
################################################

set(FILES_TO_TRANSLATE ${Q_HEADERS} ${P_HEADERS} ${HEADERS} ${SOURCES} ${FORMS})
file(GLOB TRANSLATIONS_FILES share/translations*.ts)
qt4_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TRANSLATIONS_FILES})
#qt4_add_translation(QM_FILES ${TRANSLATIONS_FILES})
