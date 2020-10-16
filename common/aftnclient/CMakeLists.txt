cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

# ------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------
# Variable for project name, used in many places
set (PROJECT aftnclient)

# Declare project
project(${PROJECT})

find_package(Qt4 REQUIRED)

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
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_RELEASE}" )

# SOURCE FILES AND DIRECTORIES
# ------------------------------------------------------------------
# Setup Qt
set(QT_USE_QTNETWORK    TRUE)


# Create libraries list
include(${QT_USE_FILE})

# Get project sources
list(APPEND Q_HEADERS src/main/aftnclient.h)
list(APPEND HEADERS src/main/private/aftnclient_p.h)
list(APPEND HEADERS src/main/aftnproto.h)
list(APPEND SOURCES src/main/aftnclient.cpp)
include_directories(./src ./src/main ./src/main/private)

# ------------------------------------------------------
# 3RDPARTY
# ------------------------------------------------------

# ------------------------------------------------------
# COMPILING
# -----------------------------------------------------
qt4_wrap_cpp(MOC_SOURCES ${Q_HEADERS})

add_library(${PROJECT_NAME} STATIC
  ${SOURCES}
  ${HEADERS}
  ${Q_HEADERS}
  ${MOC_SOURCES})
  
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} qcyrilliccodecs)