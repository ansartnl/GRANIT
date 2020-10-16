#include "InternalRequestHandler.h"

#include "HandshakeProcessor.h"
#include "NotifyProcessor.h"
#include "PingProcessor.h"
#include "SetValueResultProcessor.h"
#include "ValueProcessor.h"

#include "Interaction.h"

#include "RequestProcessorFactory.h"

InternalRequestHandler * requestHandler(QObject *parent)
{
    static InternalRequestHandler *mInternalRequestHandler = new InternalRequestHandler(parent);
    return mInternalRequestHandler;
}

InternalRequestHandler::InternalRequestHandler(QObject *parent) :
    QObject(parent), mFactory(new request_handler::RequestProcessorFactory(this))
{
    connect(mFactory, SIGNAL(info(QString,QString)), SIGNAL(info(QString,QString)));
    connect(mFactory, SIGNAL(error(QString,QString)), SIGNAL(error(QString,QString)));

    mFactory->registerCreator < snmp_interaction::Handshake >(new HandshakeProcessorCreator);
    mFactory->registerCreator < snmp_interaction::Notify >(new NotifyProcessorCreator);
    mFactory->registerCreator < snmp_interaction::Ping >(new PingProcessorCreator);
    mFactory->registerCreator < snmp_interaction::SetValueResult >(new SetValueResultProcessorCreator);
    mFactory->registerCreator < snmp_interaction::Value >(new ValueProcessorCreator);
}

const request_handler::PRequestProcessor InternalRequestHandler::createProcessor(const QVariant &data)
{
    return mFactory->createProcessor(data, parent());
}
