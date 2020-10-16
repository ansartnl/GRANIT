#include "SetValueResultProcessor.h"

#include "InternalServer.h"

#include "LocalSocket.h"

SetValueResultProcessor::SetValueResultProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("setvalueresult processor");
}

const request_handler::Reply SetValueResultProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::SetValueResult >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::SetValueResult msg = request.data.value < snmp_interaction::SetValueResult >();

    emit setValueResultCame(socket, msg.key, msg.result);

    return request_handler::Reply();
}


SetValueResultProcessorCreator::SetValueResultProcessorCreator()
{
}

request_handler::IRequestProcessor * SetValueResultProcessorCreator::createProcessor(QObject *parent)
{
    SetValueResultProcessor *processor = new SetValueResultProcessor;
    connect(processor, SIGNAL(setValueResultCame(local_connection::LocalSocket*,QString
                                                 ,snmp_interaction::SetValueResult::SetResult))
            , parent, SLOT(onSetValueResultCame(local_connection::LocalSocket*,QString
                                                ,snmp_interaction::SetValueResult::SetResult)));
    return processor;
}
