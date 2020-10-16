#ifndef IREQUESTPROCESSOR_H
#define IREQUESTPROCESSOR_H

#include "RequestStructs.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

namespace request_handler
{

/*!
 * \class IRequestProcessor
 */

class IRequestProcessor : public QObject
{
    Q_OBJECT
public:
    explicit IRequestProcessor(QObject *parent = 0);
    virtual ~IRequestProcessor();

    virtual const Reply execute(const Request &request) = 0;

signals:
    void info(const QString &senderName, const QString &text) const;
    void error(const QString &senderName, const QString &text) const;

protected:
    void info(const QString &text) const;
    void error(const QString &text) const;

};
typedef QSharedPointer < IRequestProcessor > PRequestProcessor;

/*!
 * \class RequestProcessorCreator
 */

class RequestProcessorCreator
{
public:
    RequestProcessorCreator();
    virtual ~RequestProcessorCreator();

    virtual IRequestProcessor * createProcessor(QObject *parent = 0) = 0;

protected:
    static void connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method);
};
typedef QSharedPointer < RequestProcessorCreator > PRequestProcessorCreator;

} // namespace request_handler

#endif // IREQUESTPROCESSOR_H
