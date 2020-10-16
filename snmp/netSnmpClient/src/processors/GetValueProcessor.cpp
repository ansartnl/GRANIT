#include "GetValueProcessor.h"

#include "INetSnmpDataSource.h"
#include "NetSnmpClientPrivate.h"

#include "GetValue.h"
#include "LocalSocket.h"

namespace snmp_private
{

GetValueProcessor::GetValueProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("getvalue processor");
}

const request_handler::Reply GetValueProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::GetValue >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::GetValue msg = request.data.value < snmp_interaction::GetValue >();

    NetSnmpClientPrivate *manager = qobject_cast < NetSnmpClientPrivate * >(parent());

    return request_handler::Reply(QVariantList() << QVariant::fromValue(
                                      snmp_interaction::Value(msg.key, manager->dataSource()->value(msg.key))
                                      ));
}


GetValueProcessorCreator::GetValueProcessorCreator()
{
}

request_handler::IRequestProcessor * GetValueProcessorCreator::createProcessor(QObject *parent)
{
    GetValueProcessor *processor = new GetValueProcessor(parent);
    return processor;
}

} // namespace snmp_private
