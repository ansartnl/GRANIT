cmake_minimum_required(VERSION 2.8.9)
include(cmake/compiler-flags.cmake)
include(cmake/copy-files.cmake)
include(cmake/set-version.cmake)
include(cmake/set-executable-specifics.cmake)
include(cmake/sources-management.cmake)
include(cmake/output-directories.cmake)
include(cmake/using-preset-defaults.cmake)
include(cmake/version-dep-commands.cmake)

if(NOT NETSNMPMANAGER_SUBPROJECT)
        declare_output_directories()
        set_multithreaded_build()
        #set_cxx11_build()
        #set_warnings_as_errors_build()
        set_default_definitions()
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        set_debug_postfix("d")
endif(NOT NETSNMPMANAGER_SUBPROJECT)

###################################################
###  SECTION Project Settings
###################################################

project(netSnmpManager)
message("Building netSnmpManager")
set_version(netSnmpManager 1 1 0)
add_definitions(
        -DNETSNMPMANAGER_LIBRARY
        -DBASE_MIB="COMMON-MIB"
        -DMIB_PING_VARIABLE="ping"
        -DTRAP_UDP_PORT=5055
)

###################################################
###  END OF SECTION Project Settings
###################################################


###################################################
###  SECTION Frameworks and Libraries
###################################################

######
# Qt
######

find_qt(Core)

###################################################
###  END OF SECTION Frameworks and Libraries
###################################################

add_subdirectory(src)

