#include "ValueProcessor.h"

#include "InternalServer.h"

#include "GetValue.h"
#include "LocalSocket.h"

ValueProcessor::ValueProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("value processor");
}

const request_handler::Reply ValueProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::Value >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::Value msg = request.data.value < snmp_interaction::Value >();

    emit valueCame(socket, msg.key, msg.value);

    return request_handler::Reply();
}


ValueProcessorCreator::ValueProcessorCreator()
{
}

request_handler::IRequestProcessor * ValueProcessorCreator::createProcessor(QObject *parent)
{
    ValueProcessor *processor = new ValueProcessor;
    connect(processor, SIGNAL(valueCame(local_connection::LocalSocket*,QString,QVariant))
            , parent, SLOT(onValueCame(local_connection::LocalSocket*,QString,QVariant)));
    return processor;
}
