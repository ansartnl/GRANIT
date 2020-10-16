#!/bin/bash

SCRIPT=$( readlink -f $0 )
SCRIPT_DIR=$( dirname $SCRIPT )

MIB_NAME="TEST-NET-SNMP-CLIENT-MIB"

# snmptt = 'SNMP trap handler for use with snmptrapd'. Не уверен, нужен ли он
sudo apt-get install snmp snmpd snmptt snmp-mibs-downloader libsnmp-dev

# Локальные настройки
mkdir -p ~/.snmp
mkdir -p ~/.snmp/mibs

MIB_IN_CONF=`cat ~/.snmp/snmp.conf | grep $MIB_NAME`
if [ -z "$MIB_IN_CONF" ]; then
    echo "mibs +$MIB_NAME" >> ~/.snmp/snmp.conf
fi
echo "agentXSocket    tcp:localhost:705" > ~/.snmp/netSnmpSubagent.conf

# На всякий случай сносим индексы
rm -f ~/.snmp/mibs/.index

cp $SCRIPT_DIR/*-MIB.txt ~/.snmp/mibs/

# Настройка агента snmpd
sudo cp -f $SCRIPT_DIR/snmpd.conf /etc/snmp/
sudo chmod 600 /etc/snmp/snmpd.conf
sudo chown root:root /etc/snmp/snmpd.conf

sudo service snmpd restart

