#ifndef GETVALUE_H
#define GETVALUE_H

#include <QtCore/QVariant>

namespace snmp_interaction
{

/*!
 * \struct GetValue
 * \brief Запрос на получение значения по ключу
 */

struct GetValue
{
    QString key;

    GetValue()
    {}
    GetValue(const QString &key_) :
        key(key_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const GetValue &msg)
{
    stream << msg.key;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, GetValue &msg)
{
    stream >> msg.key;

    return stream;
}

/*!
 * \struct Value
 * \brief Значение, возвреащенное по запросу
 *
 * Если ключ не поддерживается данным клиентом, \a value содержит \a QVariant::Invalid.
 */

struct Value
{
    QString key;
    QVariant value;

    Value()
    {}
    Value(const QString &key_, const QVariant &value_) :
        key(key_), value(value_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Value &msg)
{
    stream << msg.key;
    stream << msg.value;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Value &msg)
{
    stream >> msg.key;
    stream >> msg.value;

    return stream;
}

} // namespace snmp_interaction

Q_DECLARE_METATYPE(snmp_interaction::GetValue)
Q_DECLARE_METATYPE(snmp_interaction::Value)

#endif // GETVALUE_H
