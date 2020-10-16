cmake_minimum_required(VERSION 2.8)

set(CMAKE_DEBUG_POSTFIX _debug)

project(qtautotransaction)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set(qtautotransaction_SOURCES qtautotransaction.cpp)

set(qtautotransaction_Q_HEADERS)
set(qtautotransaction_HEADERS qtautotransaction.h)

qt4_wrap_cpp(qtautotransaction_MOC_SOURCES ${qtautotransaction_Q_HEADERS})

qt4_wrap_ui(qtautotransaction_UI_HEADERS ${qtautotransaction_FORMS})

add_library(${PROJECT_NAME} STATIC ${qtautotransaction_SOURCES} ${qtautotransaction_HEADERS} ${qtautotransaction_Q_HEADERS} ${qtautotransaction_MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} )
