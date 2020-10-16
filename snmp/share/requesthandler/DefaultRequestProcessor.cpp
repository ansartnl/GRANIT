#include "DefaultRequestProcessor.h"

namespace request_handler
{

/*!
 * \class DefaultRequestProcessor
 */

DefaultRequestProcessor::DefaultRequestProcessor(QObject *parent) :
    IRequestProcessor(parent)
{
    setObjectName("default processor");
}

const Reply DefaultRequestProcessor::execute(const Request &request)
{
    emit error(tr("Could not find processor for type '%1'").arg(request.data.typeName()));

    return Reply(QVariantList());
}

/*!
 * \class DefaultRequestProcessorCreator
 */

DefaultRequestProcessorCreator::DefaultRequestProcessorCreator()
{
}

IRequestProcessor * DefaultRequestProcessorCreator::createProcessor(QObject *parent)
{
    Q_UNUSED(parent)
    return new DefaultRequestProcessor;
}

} // namespace request_handler
