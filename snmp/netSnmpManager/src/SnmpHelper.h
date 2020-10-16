#ifndef SNMPHELPER_H
#define SNMPHELPER_H

#include "SnmpVariable.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

namespace snmp_helper
{

const snmp::BaseSnmpVariable getSnmpInfo(const QString &host, const oid *objid, size_t objidSize);
bool isWritable(const oid *objid, size_t objidSize);

typedef QPair < QVariant, snmp::SnmpVariable::Type > Value;
const Value valueByType(const QString &value, int type);

} // namespace snmp_helper

#endif // SNMPHELPER_H
