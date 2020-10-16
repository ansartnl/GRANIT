#ifndef NETSNMPCLIENT_H
#define NETSNMPCLIENT_H

#include "netSnmpClient_global.h"

namespace snmp
{

class INetSnmpDataSource;

/*!
 * \class NetSnmpClient
 * \brief Менеджер соединения клиента с субагентом net-snmp.
 */

class NETSNMPCLIENTSHARED_EXPORT NetSnmpClient
{
private:
    NetSnmpClient();
    ~NetSnmpClient();

public:
    static NetSnmpClient * instance();

    void setDataSource(INetSnmpDataSource *dataSource);
};

} // namespace snmp

#define netSnmpClient snmp::NetSnmpClient::instance()

#endif // NETSNMPCLIENT_H
