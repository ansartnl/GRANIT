cmake_minimum_required(VERSION 2.8.9)
include(cmake/compiler-flags.cmake)
include(cmake/copy-files.cmake)
include(cmake/set-version.cmake)
include(cmake/set-executable-specifics.cmake)
include(cmake/sources-management.cmake)
include(cmake/output-directories.cmake)
include(cmake/using-preset-defaults.cmake)
include(cmake/translation-management.cmake)
include(cmake/version-dep-commands.cmake)

if(NOT NETSNMPTRAPHANDLER_SUBPROJECT)
        declare_output_directories()
        set_multithreaded_build()
        set_default_definitions()
        set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        set_debug_postfix("d")
endif(NOT NETSNMPTRAPHANDLER_SUBPROJECT)

###################################################
###  SECTION Project Settings
###################################################

project(netSnmpTrapHandler)
message("Building netSnmpTrapHandler")
set_version(netSnmpTrapHandler 1 1 0)
add_definitions(
        -DLOCAL_SERVER_NAME="net-snmp-subagent-local-server"
        -DTRAP_UDP_PORT=5055
)
set(nita_xml ON)
if(nita_xml)
    add_definitions(-DNITA_XML)
endif(nita_xml)

###################################################
###  END OF SECTION Project Settings
###################################################


###################################################
###  SECTION Frameworks and Libraries
###################################################

######
# Qt
######

find_qt(Core Network Sql)

###################################################
###  END OF SECTION Frameworks and Libraries
###################################################

add_subdirectory(src)

