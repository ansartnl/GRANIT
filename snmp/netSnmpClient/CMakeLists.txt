cmake_minimum_required(VERSION 2.8.9)
include(cmake/compiler-flags.cmake)
include(cmake/copy-files.cmake)
include(cmake/set-version.cmake)
include(cmake/set-executable-specifics.cmake)
include(cmake/sources-management.cmake)
include(cmake/output-directories.cmake)
include(cmake/using-preset-defaults.cmake)
include(cmake/version-dep-commands.cmake)

if(NOT NETSNMPCLIENT_SUBPROJECT)
	declare_output_directories()
	set_multithreaded_build()
	set_default_definitions()
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_debug_postfix("d")
endif(NOT NETSNMPCLIENT_SUBPROJECT)

###################################################
###  SECTION Project Settings
###################################################

project(netSnmpClient)
message("Building netSnmpClient")
set_version(netSnmpClient 1 1 0)
add_definitions(
	-DNETSNMPCLIENT_LIBRARY
	-DLOCAL_SERVER_NAME="net-snmp-subagent-local-server"
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

find_qt(Core Network)

###################################################
###  END OF SECTION Frameworks and Libraries
###################################################

add_subdirectory(src)

