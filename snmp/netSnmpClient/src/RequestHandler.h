#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "IRequestProcessor.h"

#include <QtCore/QObject>

namespace request_handler
{

class RequestProcessorFactory;

} // namespace request_handler

namespace snmp_private
{

class RequestHandler;

RequestHandler * requestHandler(QObject *parent = 0);

/*!
 * \class RequestHandler
 */

class RequestHandler : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QObject *parent = 0);

    const request_handler::PRequestProcessor createProcessor(const QVariant &data);

signals:
    void info(const QString &object, const QString &text);
    void error(const QString &object, const QString &text);

private:
    request_handler::RequestProcessorFactory *mFactory;
};

} // namespace snmp_private

#endif // REQUESTHANDLER_H
