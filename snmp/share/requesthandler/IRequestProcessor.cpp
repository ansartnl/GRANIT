#include "IRequestProcessor.h"

namespace request_handler
{

/*!
 * \class IRequestProcessor
 */

IRequestProcessor::IRequestProcessor(QObject *parent) :
    QObject(parent)
{
    setObjectName("unknown request processor");
}

IRequestProcessor::~IRequestProcessor()
{
}

void IRequestProcessor::info(const QString &text) const
{
    emit info(objectName(), text);
}

void IRequestProcessor::error(const QString &text) const
{
    emit error(objectName(), text);
}

/*!
 * \class RequestProcessorCreator
 */

RequestProcessorCreator::RequestProcessorCreator()
{
}

RequestProcessorCreator::~RequestProcessorCreator()
{
}

void RequestProcessorCreator::connect(const QObject *sender, const char *signal
                                      , const QObject *receiver, const char *method)
{
    QObject::connect(sender, signal, receiver, method, Qt::QueuedConnection);
}

} // namespace request_handler
