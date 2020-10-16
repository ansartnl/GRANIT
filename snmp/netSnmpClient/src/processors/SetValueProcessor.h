#ifndef SETVALUEPROCESSOR_H
#define SETVALUEPROCESSOR_H

#include "IRequestProcessor.h"

namespace snmp_private
{

class SetValueProcessor : public request_handler::IRequestProcessor
{
    Q_OBJECT
public:
    explicit SetValueProcessor(QObject *parent = 0);

    const request_handler::Reply execute(const request_handler::Request &request);
};

/*!
 * \class SetValueProcessorCreator
 */

class SetValueProcessorCreator : public request_handler::RequestProcessorCreator
{
public:
    SetValueProcessorCreator();

private:
    request_handler::IRequestProcessor * createProcessor(QObject *parent = 0);
};

} // namespace snmp_private

#endif // SETVALUEPROCESSOR_H
