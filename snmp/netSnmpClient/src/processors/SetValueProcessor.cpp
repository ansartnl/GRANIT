#include "SetValueProcessor.h"

#include "INetSnmpDataSource.h"
#include "NetSnmpClientPrivate.h"

#include "SetValue.h"
#include "LocalSocket.h"

namespace snmp_private
{

snmp_interaction::SetValueResult::SetResult resultEnum(snmp::INetSnmpDataSource::SetResult sr)
{
    switch (sr) {
    case snmp::INetSnmpDataSource::SR_OK:
        return snmp_interaction::SetValueResult::SR_OK;
    case snmp::INetSnmpDataSource::SR_WRONG_KEY:
        return snmp_interaction::SetValueResult::SR_WRONG_KEY;
    case snmp::INetSnmpDataSource::SR_READ_ONLY:
        return snmp_interaction::SetValueResult::SR_READ_ONLY;
    case snmp::INetSnmpDataSource::SR_WRONG_TYPE:
        return snmp_interaction::SetValueResult::SR_WRONG_TYPE;
    case snmp::INetSnmpDataSource::SR_WRONG_VALUE:
        return snmp_interaction::SetValueResult::SR_WRONG_VALUE;
    }
    return snmp_interaction::SetValueResult::SR_UNKNOWN;
}

SetValueProcessor::SetValueProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("getvalue processor");
}

const request_handler::Reply SetValueProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::SetValue >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::SetValue msg = request.data.value < snmp_interaction::SetValue >();

    NetSnmpClientPrivate *manager = qobject_cast < NetSnmpClientPrivate * >(parent());
    snmp::INetSnmpDataSource::SetResult res = manager->dataSource()->setValue(msg.key, msg.value);

    return request_handler::Reply(QVariantList() << QVariant::fromValue(
                                      snmp_interaction::SetValueResult(msg.key, resultEnum(res))
                                      ));
}


SetValueProcessorCreator::SetValueProcessorCreator()
{
}

request_handler::IRequestProcessor * SetValueProcessorCreator::createProcessor(QObject *parent)
{
    SetValueProcessor *processor = new SetValueProcessor(parent);
    return processor;
}

} // namespace snmp_private
