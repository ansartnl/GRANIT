#ifndef VALUEPROCESSOR_H
#define VALUEPROCESSOR_H

#include "IRequestProcessor.h"

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class ValueProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit ValueProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);

signals:
    void valueCame(local_connection::LocalSocket *socket, const QString &key, const QVariant &value);
};

/*!
 * \class ValueProcessorCreator
 */

class ValueProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    ValueProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

#endif // VALUEPROCESSOR_H
