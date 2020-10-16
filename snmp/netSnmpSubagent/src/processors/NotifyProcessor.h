#ifndef NOTIFYPROCESSOR_H
#define NOTIFYPROCESSOR_H

#include "IRequestProcessor.h"

#include "Notify.h"

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class NotifyProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit NotifyProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);

signals:
    void notifyCame(local_connection::LocalSocket *socket
                    , const QString &key, const snmp_interaction::Vars &vars);
};

/*!
 * \class NotifyProcessorCreator
 */

class NotifyProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    NotifyProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

#endif // NOTIFYPROCESSOR_H
