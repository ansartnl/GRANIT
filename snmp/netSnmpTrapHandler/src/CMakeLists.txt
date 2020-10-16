
####################################################
###  SECTION Sources
###################################################

add_group("" ""
        main.cpp
)

############
# External
############

if(NOT NETSNMPTRAPHANDLER_SUBPROJECT)
        set(IMPORT_PREFIX "../../share")
        set(LOG4QT_BUILD_STATIC TRUE)
        set(LOG4QT_DONT_INSTALL TRUE)
        add_subdirectory(../../../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
        include_directories(../../../common/log4qt/src)

        import_groups(../../../common/cmdline/cmdline.cmake "common")
        import_groups(../../../common/initmain/initmain.cmake "common")
        import_groups(../../../common/log/log.cmake "common")
        remove_definitions(-DLOG4QT_LINKED_JUDICIOUSLY)

else(NOT NETSNMPTRAPHANDLER_SUBPROJECT)
        set(IMPORT_PREFIX ${SHARE_SOURCE_DIR})
        import_groups(${SHARE_SOURCE_DIR}/common/cmdline/cmdline.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/initmain/initmain.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/log/log.cmake "common")
endif(NOT NETSNMPTRAPHANDLER_SUBPROJECT)

import_groups(${IMPORT_PREFIX}/trap_interaction/trap_interaction.cmake "common")

###################################################
###  END OF SECTION Sources
###################################################


###################################################
###  SECTION Build
###################################################

prepare_build_components()

add_executable(netSnmpTrapHandler ${BUILD_COMPONENTS})

set_property(TARGET netSnmpTrapHandler PROPERTY FOLDER "${PROJECT_FOLDER}")

link_qt(netSnmpTrapHandler)

if(NETSNMPTRAPHANDLER_SUBPROJECT)
    link_libraries_common(netSnmpTrapHandler
        Log4Qt
    )
else(NETSNMPTRAPHANDLER_SUBPROJECT)
    target_link_libraries(netSnmpTrapHandler
        log4qt
    )
endif(NETSNMPTRAPHANDLER_SUBPROJECT)

configure_build(netSnmpTrapHandler)

set_output_directories(netSnmpTrapHandler)

###################################################
###  END OF SECTION Build
###################################################


###################################################
###  SECTION Post-Build
###################################################

#install_translations(netSnmpTrapHandler ${CMAKE_CURRENT_SOURCE_DIR}/../share/translations)

###################################################
###  END OF SECTION Post-Build
###################################################

