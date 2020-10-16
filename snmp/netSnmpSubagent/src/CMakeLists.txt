include_directories(
        helpers
        mib-objects
        processors
)

####################################################
###  SECTION Sources
###################################################

add_group("" ""
        ClientObject.cpp
        ClientObject.h
        InternalRequestHandler.cpp
        InternalRequestHandler.h
        InternalRunnable.cpp
        InternalRunnable.h
        InternalServer.cpp
        InternalServer.h
        main.cpp
        main.h
        MainService.cpp
        MainService.h
        Service.cpp
        Service.h
        ServiceStarter.cpp
        ServiceStarter.h
        Subagent.cpp
        Subagent.h
        SubagentLoop.cpp
        SubagentLoop.h
        SubagentObjects.cpp
        SubagentObjects.h
)

add_group("helpers" "helpers"
        helpers/Config.h
        helpers/Config.cpp
        helpers/ConfigKeys.h
        helpers/Timeval.h
)

add_group("mib-objects" "mib-objects"
        mib-objects/BaseMibEntity.h
        mib-objects/BaseMibEntity.cpp
        mib-objects/BaseMibObject.h
        mib-objects/BaseMibObject.cpp
        mib-objects/CallbackHandler.h
        mib-objects/CallbackHandler.cpp
        mib-objects/Counter32MibObject.h
        mib-objects/Counter32MibObject.cpp
        mib-objects/Counter64MibObject.h
        mib-objects/Counter64MibObject.cpp
        mib-objects/DateAndTimeMibObject.h
        mib-objects/DateAndTimeMibObject.cpp
        mib-objects/DateAndTimeWithZoneMibObject.h
        mib-objects/DateAndTimeWithZoneMibObject.cpp
        mib-objects/IntegerMibObject.h
        mib-objects/IntegerMibObject.cpp
        mib-objects/IpAddressMibObject.h
        mib-objects/IpAddressMibObject.cpp
        mib-objects/ProgramStateMibObject.h
        mib-objects/ProgramStateMibObject.cpp
        mib-objects/ScriptMibObject.h
        mib-objects/ScriptMibObject.cpp
        mib-objects/StringMibObject.h
        mib-objects/StringMibObject.cpp
        mib-objects/String2MibObject.h
        mib-objects/String2MibObject.cpp
        mib-objects/TrapMibObject.h
        mib-objects/TrapMibObject.cpp
        mib-objects/ValueMibObject.h
        mib-objects/ValueMibObject.cpp
        mib-objects/ValueMibObjectFactory.h
        mib-objects/ValueMibObjectFactory.cpp
)

add_group("processors" "processors"
        processors/HandshakeProcessor.h
        processors/HandshakeProcessor.cpp
        processors/NotifyProcessor.h
        processors/NotifyProcessor.cpp
        processors/PingProcessor.h
        processors/PingProcessor.cpp
        processors/SetValueResultProcessor.h
        processors/SetValueResultProcessor.cpp
        processors/ValueProcessor.h
        processors/ValueProcessor.cpp
)

############
# External
############

if(NOT NETSNMPSUBAGENT_SUBPROJECT)
        set(IMPORT_PREFIX "../../share")

        add_subdirectory(../../../common/qtservice ${CMAKE_CURRENT_BINARY_DIR}/qtservice)
        include_directories(../../../common/qtservice)

        set(LOG4QT_BUILD_STATIC TRUE)
        set(LOG4QT_DONT_INSTALL TRUE)
        add_subdirectory(../../../common/log4qt ${CMAKE_CURRENT_BINARY_DIR}/log4qt)
        include_directories(../../../common/log4qt/src)

        import_groups(../../../3rdparty/qcorecmdlineargs/qcorecmdlineargs.cmake "3rdparty")
        import_groups(../../../common/enumstreaming/enumstreaming.cmake "common")
        import_groups(../../../common/cmdline/cmdline.cmake "common")
        import_groups(../../../common/initmain/initmain.cmake "common")
        import_groups(../../../common/log/log.cmake "common")
        remove_definitions(-DLOG4QT_LINKED_JUDICIOUSLY)
    if(nita_xml)
        import_groups(../../../common/nitaxmlsettings/nitaxmlsettings.cmake "common")
    else(nita_xml)
        import_groups(../../../common/qtxmlsettings/qtxmlsettings.cmake "common")
    endif(nita_xml)
        import_groups(../../../common/scriptrunner/scriptrunner.cmake "common")
        import_groups(../../../common/version/version.cmake "common")

else(NOT NETSNMPSUBAGENT_SUBPROJECT)
        set(IMPORT_PREFIX ${SHARE_SOURCE_DIR})
        import_groups(${SHARE_SOURCE_DIR}/3rdparty/qcorecmdlineargs/qcorecmdlineargs.cmake "3rdparty")
        import_groups(${SHARE_SOURCE_DIR}/common/enumstreaming/enumstreaming.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/cmdline/cmdline.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/initmain/initmain.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/log/log.cmake "common")
    if(nita_xml)
        import_groups(${SHARE_SOURCE_DIR}/common/nitaxmlsettings/nitaxmlsettings.cmake "common")
    else(nita_xml)
        import_groups(${SHARE_SOURCE_DIR}/common/qtxmlsettings/qtxmlsettings.cmake "common")
    endif(nita_xml)
        import_groups(${SHARE_SOURCE_DIR}/common/scriptrunner/scriptrunner.cmake "common")
        import_groups(${SHARE_SOURCE_DIR}/common/version/version.cmake "common")
endif(NOT NETSNMPSUBAGENT_SUBPROJECT)

import_groups(${IMPORT_PREFIX}/interaction/interaction.cmake "common")
import_groups(${IMPORT_PREFIX}/localserver/localserver.cmake "common")
import_groups(${IMPORT_PREFIX}/localsocket/localsocket.cmake "common")
import_groups(${IMPORT_PREFIX}/pinger/pinger.cmake "common")
import_groups(${IMPORT_PREFIX}/requesthandler/requesthandler.cmake "common")

###################################################
###  END OF SECTION Sources
###################################################


###################################################
###  SECTION Build
###################################################

prepare_build_components()

add_executable(netSnmpSubagent ${BUILD_COMPONENTS})

set_property(TARGET netSnmpSubagent PROPERTY FOLDER "${PROJECT_FOLDER}")

link_qt(netSnmpSubagent)

if(NETSNMPSUBAGENT_SUBPROJECT)
    link_libraries_common(netSnmpSubagent
        Log4Qt
        QtService
    )
else(NETSNMPSUBAGENT_SUBPROJECT)
    target_link_libraries(netSnmpSubagent
        log4qt
        qtservice
    )
endif(NETSNMPSUBAGENT_SUBPROJECT)

target_link_libraries(netSnmpSubagent
    netsnmpagent
    netsnmp
    netsnmphelpers
)

configure_build(netSnmpSubagent)

set_output_directories(netSnmpSubagent)

###################################################
###  END OF SECTION Build
###################################################


###################################################
###  SECTION Post-Build
###################################################

install_translations(netSnmpSubagent ${CMAKE_CURRENT_SOURCE_DIR}/../share/translations)
if(nita_xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ netSnmpSubagent_nita.xml)
    add_custom_command(TARGET netSnmpSubagent POST_BUILD COMMAND ${CMAKE_COMMAND} -E rename
        ${BIN_INSTALLATION_PATH}/../etc/netSnmpSubagent_nita.xml ${BIN_INSTALLATION_PATH}/../etc/netSnmpSubagent.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_admin.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_database.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_datisClient.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_dclClient.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_dvolmetClient.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_fansClient.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ conf_dportalClient.xml)
else(nita_xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ netSnmpSubagent.xml)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ admin.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ database.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ datisClient.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ dclClient.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ dvolmetClient.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ fansClient.conf)
    copy_files(netSnmpSubagent ${BIN_INSTALLATION_PATH}/../etc ${CMAKE_CURRENT_SOURCE_DIR}/../etc/ dportalClient.conf)
endif(nita_xml)

###################################################
###  END OF SECTION Post-Build
###################################################

