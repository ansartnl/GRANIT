#ifndef INTERNALREQUESTHANDLER_H
#define INTERNALREQUESTHANDLER_H

#include "IRequestProcessor.h"

#include <QtCore/QObject>

namespace request_handler
{

class RequestProcessorFactory;

} // namespace request_handler

class InternalRequestHandler;

InternalRequestHandler * requestHandler(QObject *parent = 0);

/*!
 * \class InternalRequestHandler
 */

class InternalRequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit InternalRequestHandler(QObject *parent = 0);

    const request_handler::PRequestProcessor createProcessor(const QVariant &data);

signals:
    void info(const QString &object, const QString &text);
    void error(const QString &object, const QString &text);

private:
    request_handler::RequestProcessorFactory *mFactory;
};

#endif // INTERNALREQUESTHANDLER_H
