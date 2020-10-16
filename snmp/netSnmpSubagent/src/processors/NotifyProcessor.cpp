#include "NotifyProcessor.h"

#include "InternalServer.h"

#include "LocalSocket.h"

NotifyProcessor::NotifyProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("Notify processor");
}

const request_handler::Reply NotifyProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::Notify >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::Notify msg = request.data.value < snmp_interaction::Notify >();

    emit notifyCame(socket, msg.key, msg.vars);

    return request_handler::Reply();
}


NotifyProcessorCreator::NotifyProcessorCreator()
{
}

request_handler::IRequestProcessor * NotifyProcessorCreator::createProcessor(QObject *parent)
{
    NotifyProcessor *processor = new NotifyProcessor;
    connect(processor, SIGNAL(notifyCame(local_connection::LocalSocket*,QString,snmp_interaction::Vars))
            , parent, SLOT(onNotifyCame(local_connection::LocalSocket*,QString,snmp_interaction::Vars)));
    return processor;
}
