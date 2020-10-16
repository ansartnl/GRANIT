include_directories(
    ../include
    processors
)

####################################################
###  SECTION Sources
###################################################

add_group("" ""
    INetSnmpDataSource.cpp
    NetSnmpClient.cpp
    NetSnmpClientPrivate.cpp
    NetSnmpClientPrivate.h
    RequestHandler.h
    RequestHandler.cpp
    ../include/INetSnmpDataSource.h
    ../include/NetSnmpClient.h
    ../include/netSnmpClient_global.h
    ../include/Vars.h
)

add_group("processors" "processors"
    processors/GetValueProcessor.h
    processors/GetValueProcessor.cpp
    processors/HandshakeProcessor.h
    processors/HandshakeProcessor.cpp
    processors/PingProcessor.h
    processors/PingProcessor.cpp
    processors/SetValueProcessor.h
    processors/SetValueProcessor.cpp
)

############
# External
############

if(NOT NETSNMPCLIENT_SUBPROJECT)
    set(IMPORT_PREFIX "../share")
    import_groups(../../../common/enumstreaming/enumstreaming.cmake "common")
else(NOT NETSNMPCLIENT_SUBPROJECT)
    set(IMPORT_PREFIX ${SHARE_SOURCE_DIR})
    import_groups(${SHARE_SOURCE_DIR}/common/enumstreaming/enumstreaming.cmake "common")
endif(NOT NETSNMPCLIENT_SUBPROJECT)

import_groups(${IMPORT_PREFIX}/interaction/interaction.cmake "common")
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

add_library(netSnmpClient SHARED ${BUILD_COMPONENTS})

set_property(TARGET netSnmpClient PROPERTY FOLDER "${PROJECT_FOLDER}")

link_qt(netSnmpClient)

target_link_libraries(netSnmpClient
    snmp
)

configure_build(netSnmpClient)

set_output_directories(netSnmpClient)

###################################################
###  END OF SECTION Build
###################################################


###################################################
###  SECTION Post-Build
###################################################

copy_files(netSnmpClient ${INCLUDE_INSTALLATION_PATH}/netSnmpClient ${CMAKE_CURRENT_SOURCE_DIR}/../include/ NetSnmpClient.h netSnmpClient_global.h INetSnmpDataSource.h Vars.h)

###################################################
###  END OF SECTION Post-Build
###################################################

