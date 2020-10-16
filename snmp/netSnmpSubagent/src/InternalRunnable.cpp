#include "InternalRunnable.h"

#include "InternalRequestHandler.h"

InternalRunnable::InternalRunnable(local_connection::LocalSocket *socket, const QVariant &request) :
    QObject(0), mSocket(socket), mRequest(request)
{
}

void InternalRunnable::run()
{
    request_handler::PRequestProcessor command = requestHandler()->createProcessor(mRequest);

    const request_handler::Reply reply = command->execute(request_handler::Request(mRequest, mSocket));
    foreach (const QVariant &d, reply.data) {
        emit send(mSocket, d);
    }
}
