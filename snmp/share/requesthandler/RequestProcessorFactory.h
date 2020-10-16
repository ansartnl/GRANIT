#ifndef REQUESTPROCESSORFACTORY_H
#define REQUESTPROCESSORFACTORY_H

#include "IRequestProcessor.h"

namespace request_handler
{

class RequestProcessorFactory : public QObject
{
    Q_OBJECT
public:
    explicit RequestProcessorFactory(QObject *parent = 0);

    template < typename T >
    void registerCreator(RequestProcessorCreator *creator)
    {
        mCreators.insert(hashCode(QVariant::fromValue(T())), PRequestProcessorCreator(creator));
    }

    const PRequestProcessor createProcessor(const QVariant &data, QObject *parent = 0);

signals:
    void info(const QString &senderName, const QString &text);
    void error(const QString &senderName, const QString &text);

private:
    const QString hashCode(const QVariant &data) const;

private:
    QHash < QString, PRequestProcessorCreator > mCreators;
    PRequestProcessorCreator mDefaultCreator;
};

} // namespace request_handler

#endif // REQUESTPROCESSORFACTORY_H
