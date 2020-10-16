#ifndef GETVALUEPROCESSOR_H
#define GETVALUEPROCESSOR_H

#include "IRequestProcessor.h"

namespace snmp_private
{

class GetValueProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit GetValueProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);
};

/*!
 * \class GetValueProcessorCreator
 */

class GetValueProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    GetValueProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

} // namespace snmp_private

#endif // GETVALUEPROCESSOR_H
