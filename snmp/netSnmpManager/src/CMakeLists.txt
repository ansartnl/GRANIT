include_directories(../include)

####################################################
###  SECTION Sources
###################################################

add_group("" ""
        HostWatcher.cpp
        HostWatcher.h
        NetSnmpManager.cpp
        NetSnmpManagerPrivate.cpp
        NetSnmpManagerPrivate.h
        SnmpHelper.cpp
        SnmpHelper.h
        ../include/NetSnmpManager.h
        ../include/netSnmpManager_global.h
        ../include/SnmpVariable.h
)

############
# External
############

if(NOT NETSNMPMANAGER_SUBPROJECT)
        set(IMPORT_PREFIX "../../share")
        import_groups(../../../common/enumstreaming/enumstreaming.cmake "common")
else(NOT NETSNMPMANAGER_SUBPROJECT)
        set(IMPORT_PREFIX ${SHARE_SOURCE_DIR})
        import_groups(${SHARE_SOURCE_DIR}/common/enumstreaming/enumstreaming.cmake "share/common")
endif(NOT NETSNMPMANAGER_SUBPROJECT)

import_groups(${IMPORT_PREFIX}/trap_interaction/trap_interaction.cmake "common")

###################################################
###  END OF SECTION Sources
###################################################


###################################################
###  SECTION Build
###################################################

prepare_build_components()

add_library(netSnmpManager SHARED ${BUILD_COMPONENTS})

set_property(TARGET netSnmpManager PROPERTY FOLDER ${PROJECT_FOLDER})

link_qt(netSnmpManager)

target_link_libraries(netSnmpManager
        snmp
)

configure_build(netSnmpManager)

set_output_directories(netSnmpManager)

###################################################
###  END OF SECTION Build
###################################################


###################################################
###  SECTION Post-Build
###################################################

copy_files(netSnmpManager ${INCLUDE_INSTALLATION_PATH}/netSnmpManager ${CMAKE_CURRENT_SOURCE_DIR}/../include/ NetSnmpManager.h netSnmpManager_global.h SnmpVariable.h)

###################################################
###  END OF SECTION Post-Build
###################################################

