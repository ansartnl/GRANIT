#ifndef INETSNMPDATASOURCE_H
#define INETSNMPDATASOURCE_H

#include "Vars.h"

namespace snmp
{

/*!
 * \class INetSnmpDataSource
 * \brief Интерфейс для клиента snmp
 *
 * Клиентская программа должна реализовать данный интерфейс и передать указатель на экземпляр реализации в
 * \a NetSnmpClient.
 */

class INetSnmpDataSource
{
public:
    INetSnmpDataSource() {}
    virtual ~INetSnmpDataSource() {}

    /*!
     * Уникальный, в пределах субагента, идентификатор процесса. Должен быть известен до запуска программы.
     * Для программы, несколько экземпляров которой могут быть запущены на одном компьютере,
     * идентификаторы должен отличаться.
     */
    virtual const QString identifier() const = 0;

    /*!
     * Реазизация данной функции должна возвращать значение по строковому ключу,
     * либо \a QVariant::Invalid, если такой ключ не поддерживается.
     */
    virtual const QVariant value(const QString &key) const = 0;

    enum SetResult {
        SR_OK
        , SR_WRONG_KEY
        , SR_READ_ONLY
        , SR_WRONG_TYPE
        , SR_WRONG_VALUE
    };

    /*!
     * Реазизация данной функции должна изменять значение по ключу \a key на то, что содержится в \a value.
     * \return SR_OK, если изменение прошло успешно;
     *         SR_WRONG_KEY, если ключ не поддерживается;
     *         SR_READ_ONLY, если произошла попытка изменить значение, доступное только для чтения;
     *         SR_WRONG_TYPE, если \a value содержит переменную не того типа, который соответствует ключу;
     *         SR_WRONG_VALUE, если \a value содержит значение, выходящее за пределы допустимых.
     */
    virtual SetResult setValue(const QString &key, const QVariant &value) = 0;

    /*!
     * Данная функция вызывается для записи в лог информационного сообщения.
     */
    virtual void writeLogInfo(const QString &text) = 0;

    /*!
     * Данная функция вызывается для записи в лог сообщения об ошибке.
     */
    virtual void writeLogError(const QString &text) = 0;

protected:
    /*!
     * Данную функцию следует вызывать для отправки нотификации (NOTIFICATION).
     */
    void notify(const QString &key, const Vars &vars);
};

} // namespace snmp

#endif // INETSNMPDATASOURCE_H
