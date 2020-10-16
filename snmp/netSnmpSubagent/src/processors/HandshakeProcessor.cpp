#include "HandshakeProcessor.h"

#include "InternalServer.h"

#include "Handshake.h"
#include "LocalSocket.h"

HandshakeProcessor::HandshakeProcessor(QObject *parent) :
    request_handler::IRequestProcessor(parent)
{
    setObjectName("handshake processor");
}

const request_handler::Reply HandshakeProcessor::execute(const request_handler::Request &request)
{
    local_connection::LocalSocket *socket = static_cast < local_connection::LocalSocket * >(request.source);
    if (!socket) {
        error(tr("Bad source"));
        return request_handler::Reply();
    }
    if (!request.data.canConvert < snmp_interaction::Handshake >()) {
        error(tr("Bad data"));
        return request_handler::Reply();
    }
    const snmp_interaction::Handshake msg = request.data.value < snmp_interaction::Handshake >();
    if (msg.name.isEmpty()) {
        error(tr("Empty name"));
        return request_handler::Reply();
    }
    socket->setObjectName(msg.name);

    emit handshakeCame(socket);

    return request_handler::Reply();
}


HandshakeProcessorCreator::HandshakeProcessorCreator()
{
}

request_handler::IRequestProcessor * HandshakeProcessorCreator::createProcessor(QObject *parent)
{
    HandshakeProcessor *processor = new HandshakeProcessor;
    connect(processor, SIGNAL(handshakeCame(local_connection::LocalSocket*))
            , parent, SLOT(onHandshakeCame(local_connection::LocalSocket*)));
    return processor;
}
