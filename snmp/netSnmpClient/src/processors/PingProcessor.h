#ifndef PINGPROCESSOR_H
#define PINGPROCESSOR_H

#include "IRequestProcessor.h"

namespace snmp_private
{

class PingProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit PingProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);

signals:
    void pingCame(const QString &socketName);
};

/*!
 * \class PingProcessorCreator
 */

class PingProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    PingProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

} // namespace snmp_private

#endif // PINGPROCESSOR_H
