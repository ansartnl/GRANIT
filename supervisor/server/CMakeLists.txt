cmake_minimum_required(VERSION 2.6)

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------

# Declare project
project(SupervisorServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

find_package(Qt4 REQUIRED)
# Setup Qt
#set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTSQL TRUE)
set(QT_USE_QTNETWORK TRUE)
# Create libraries list
include(${QT_USE_FILE})

# -----------------------------------------------------
# 3rd party
# -----------------------------------------------------
add_subdirectory(../../common/qtservice ${CMAKE_CURRENT_BINARY_DIR}/qtservice)
include_directories(../../common/qtservice)

add_subdirectory(../../common/qtxmlsettings ${CMAKE_CURRENT_BINARY_DIR}/qtxmlsettings)
include_directories(../../common/qtxmlsettings)

add_subdirectory(../../common/qtlanguagemenu ${CMAKE_CURRENT_BINARY_DIR}/qtlanguagemenu)
include_directories(../../common/qtlanguagemenu)

set(LOG4QT_BUILD_STATIC TRUE)
set(LOG4QT_DONT_INSTALL TRUE)
add_subdirectory(../../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
include_directories(../../common/log4qt/src)

include(../proto/proto.cmake)
include(../../common/initmain/initmain.cmake)
include(../../common/log/log.cmake)
include(../../common/pulsar/pulsar.cmake)
include(../../common/qttcpconnectionchecker/qttcpconnectionchecker.cmake)
include(../../common/qtvariantconnection/qtvariantconnection.cmake)
include(../../common/scriptrunner/scriptrunner.cmake)

# -----------------------------------------------------

message("${CMAKE_CURRENT_YEAR}")

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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_VERBOSE_MAKEFILE TRUE)
  add_definitions(-DQTSERVICE_DEBUG)
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

set(CMAKE_INCLUDE_CURRENT_DIR true)

# SOURCE FILES AND DIRECTORIES

# project version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_RELEASE 0)
set (VERSION_BUILD 0)
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

add_definitions(-DVERSION_STR="${VERSION}")

set(COMPANY_NAME "Peleng Ltd.")
set(FILE_DESCRIPTION "${PROJECT_NAME}")
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}.${VERSION_BUILD}")
set(LEGAL_COPYRIGHT "All rights reserved")
set(PRODUCT_NAME "${PROJECT_NAME}")
set(OUTPUT_NAME "${PROJECT_NAME}.exe")

# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------

# Get project sources
list(APPEND SOURCES
        src/main.cpp
        src/common.cpp
        src/SupervisorService.cpp
        src/init.cpp
        src/ClientConnection.cpp
        src/controllers/Controller.cpp
        src/controllers/NetController.cpp
        src/controllers/ProcessController.cpp
        src/controllers/ServiceController.cpp
        src/controllers/ScriptController.cpp
        src/helpers/DbConnThread.cpp
        src/helpers/AppController.cpp
        src/helpers/ComputerHelper.cpp
        src/helpers/MonitoredControllers.cpp
        src/helpers/RequestThread.cpp
        )

list(APPEND Q_HEADERS
        src/SupervisorService.h
        src/ClientConnection.h
        src/init.h
        src/controllers/Controller.h
        src/controllers/NetController.h
        src/controllers/ProcessController.h
        src/controllers/ServiceController.h
        src/controllers/ScriptController.h
        src/helpers/DbConnThread.h
        src/helpers/AppController.h
        src/helpers/MonitoredControllers.h
        src/helpers/RequestThread.h
        )

list(APPEND HEADERS
        src/common.h
        src/helpers/ComputerHelper.h
        )

# Walk throw file list to include all sources directories
foreach(HEADER_FILE ${HEADERS} ${Q_HEADERS})
  get_filename_component(HEADER_PATH ${HEADER_FILE} PATH)
  include_directories(${HEADER_PATH})
endforeach(HEADER_FILE)

# if you use Q_OBJECT
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})
qt4_automoc(${Q_SOURCES})
qt4_wrap_ui(UI_HEADERS ${FORMS})
qt4_add_resources(QRC_SOURCES ${RESOURCES})

#create app icon
set(WIN_RESOURCES)
if(MINGW AND WIN32)
  # resource for mingw win32
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/share/SupervisorServer.rc.in ${CMAKE_CURRENT_BINARY_DIR}/share/SupervisorServer.rc)
  list(APPEND WIN_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/share/SupervisorServer.rc)
endif(MINGW AND WIN32)


# COMPILATION
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")


add_executable(${PROJECT_NAME}  #WIN32
  ${SOURCES}
  ${Q_SOURCES}
  ${MOC_SOURCES}
  ${QRC_SOURCES}
  ${HEADERS}
  ${Q_HEADERS}
  ${P_HEADERS}
  ${UI_HEADERS}
  ${QS_SOURCES}
  ${WIN_RESOURCES})

# LINKING
# rename debug target
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug" VERSION ${VERSION})

# main
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} 
	qtservice 
	qtxmlsettings 
	qtlanguagemenu
    log4qt
    )
