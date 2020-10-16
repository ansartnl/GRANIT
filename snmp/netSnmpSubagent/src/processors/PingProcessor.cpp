#include "PingProcessor.h"

#include "InternalServer.h"

#include "PingPong.h"
#include "LocalSocket.h"

PingProcessor::PingProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("ping processor");
}

const request_handler::Reply PingProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (socket->objectName().isEmpty()) {
        error(tr("Socket name is empty"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::Ping >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }

    emit pingCame(socket->objectName());

    return request_handler::Reply();
}


PingProcessorCreator::PingProcessorCreator()
{
}

request_handler::IRequestProcessor * PingProcessorCreator::createProcessor(QObject *parent)
{
    PingProcessor *processor = new PingProcessor;
    connect(processor, SIGNAL(pingCame(QString)), parent, SIGNAL(pingCame(QString)));
    return processor;
}
