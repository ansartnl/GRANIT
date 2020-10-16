CONFIG(debug, debug|release) : POSTFIX = _debug

CONFIG += nita_xml

nita_xml : {
    DEFINES += NITA_XML
}

deploy : {
    BUILD_DIR = $$PWD/../NetSnmpProject
} else {
    CONFIG(release, debug|release) : BUILD_DIR = $$PWD/../release
    CONFIG(debug, debug|release) : BUILD_DIR = $$PWD/../debug
}

!exists(\"$$BUILD_DIR\") : system(mkdir \"$$BUILD_DIR\")
!exists(\"$$BUILD_DIR/etc\") : system(mkdir \"$$BUILD_DIR/etc\")
!exists(\"$$BUILD_DIR/log\") : system(mkdir \"$$BUILD_DIR/log\")

deploy : {
    # TODO: copy config to etc
}

VERSION = 1.1

DEFINES += ITOS_NET_SNMP_VERSION=0x0101

DEFINES += LOCAL_SERVER_NAME='\\"net-snmp-subagent-local-server\\"'
DEFINES += TRAP_UDP_PORT=5055

DEFINES += BASE_MIB='\\"COMMON-MIB\\"'
DEFINES += MIB_PING_VARIABLE='\\"ping\\"'
