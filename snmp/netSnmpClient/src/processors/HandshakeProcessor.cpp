#include "HandshakeProcessor.h"

#include "INetSnmpDataSource.h"
#include "NetSnmpClientPrivate.h"

#include "Handshake.h"
#include "LocalSocket.h"

namespace snmp_private
{

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
    socket->setObjectName(msg.name);
    info(tr("Handshake came from %1").arg(msg.name));

    emit handshakeCame();

    NetSnmpClientPrivate *manager = qobject_cast < NetSnmpClientPrivate * >(parent());

    return request_handler::Reply(QVariantList() << QVariant::fromValue(
                                      snmp_interaction::Handshake(manager->dataSource()->identifier())
                                      ));
}


HandshakeProcessorCreator::HandshakeProcessorCreator()
{
}

request_handler::IRequestProcessor * HandshakeProcessorCreator::createProcessor(QObject *parent)
{
    HandshakeProcessor *processor = new HandshakeProcessor(parent);
    connect(processor, SIGNAL(handshakeCame()), parent, SLOT(onHandshakeCame()));
    return processor;
}

} // namespace snmp_private
