#include "NetSnmpClient.h"

#include "NetSnmpClientPrivate.h"

namespace snmp
{

NetSnmpClient::NetSnmpClient()
{
    snmp_private::privateClient();
}

NetSnmpClient::~NetSnmpClient()
{
}

NetSnmpClient * NetSnmpClient::instance()
{
    static NetSnmpClient *mNetSnmpManager = new NetSnmpClient();
    return mNetSnmpManager;
}

void NetSnmpClient::setDataSource(INetSnmpDataSource *dataSource)
{
    snmp_private::privateClient()->setDataSource(dataSource);
}

} // namespace snmp
