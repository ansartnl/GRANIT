#ifndef HANDSHAKEPROCESSOR_H
#define HANDSHAKEPROCESSOR_H

#include "IRequestProcessor.h"

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class HandshakeProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit HandshakeProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);

signals:
    void handshakeCame(local_connection::LocalSocket *socket);
};

/*!
 * \class HandshakeProcessorCreator
 */

class HandshakeProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    HandshakeProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

#endif // HANDSHAKEPROCESSOR_H
