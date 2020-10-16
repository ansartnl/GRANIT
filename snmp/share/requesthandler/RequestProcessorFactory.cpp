#include "RequestProcessorFactory.h"

#include "DefaultRequestProcessor.h"

namespace request_handler
{

RequestProcessorFactory::RequestProcessorFactory(QObject *parent) :
    QObject(parent), mDefaultCreator(new DefaultRequestProcessorCreator)
{
    setObjectName("request processor factory");
}

const PRequestProcessor RequestProcessorFactory::createProcessor(const QVariant &data, QObject *parent)
{
    PRequestProcessorCreator creator = mCreators[hashCode(data)];
    if (creator.isNull())
        creator = mDefaultCreator;

    IRequestProcessor *command = creator->createProcessor(parent);
    connect(command, SIGNAL(info(QString,QString)), SIGNAL(info(QString,QString)));
    connect(command, SIGNAL(error(QString,QString)), SIGNAL(error(QString,QString)));
    return PRequestProcessor(command);
}

const QString RequestProcessorFactory::hashCode(const QVariant &data) const
{
    return QString::fromLatin1(data.typeName());
}

} // namespace request_handler
