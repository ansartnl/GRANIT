#ifndef SETVALUERESULTPROCESSOR_H
#define SETVALUERESULTPROCESSOR_H

#include "IRequestProcessor.h"

#include "SetValue.h"

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class SetValueResultProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit SetValueResultProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);

signals:
    void setValueResultCame(local_connection::LocalSocket *socket, const QString &key
                            , snmp_interaction::SetValueResult::SetResult result);
};

/*!
 * \class SetValueResultProcessorCreator
 */

class SetValueResultProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    SetValueResultProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

#endif // SETVALUERESULTPROCESSOR_H
