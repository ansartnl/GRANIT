#include "INetSnmpDataSource.h"

#include "NetSnmpClientPrivate.h"

namespace snmp
{

void INetSnmpDataSource::notify(const QString &key, const Vars &vars)
{
    snmp_private::privateClient()->notify(key, vars);
}

} // namespace snmp
