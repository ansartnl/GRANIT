#ifndef DEFAULTREQUESTPROCESSOR_H
#define DEFAULTREQUESTPROCESSOR_H

#include "IRequestProcessor.h"

namespace request_handler
{

/*!
 * \class DefaultRequestProcessor
 * \brief Обработчик запросов по умолчанию
 *
 * Предназначен для запросов, для которых не найдено другого обработчика.
 * Генерирует ошибку и возвращает \a QVariant::Invalid.
 */

class DefaultRequestProcessor : public IRequestProcessor
{
    Q_OBJECT
public:
    DefaultRequestProcessor(QObject *parent = 0);

    const Reply execute(const Request &request);
};

/*!
 * \class DefaultRequestProcessorCreator
 */

class DefaultRequestProcessorCreator : public RequestProcessorCreator
{
public:
    DefaultRequestProcessorCreator();

private:
    IRequestProcessor * createProcessor(QObject *parent = 0);
};

} // namespace request_handler

#endif // DEFAULTREQUESTPROCESSOR_H
