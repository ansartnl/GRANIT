#ifndef SETVALUE_H
#define SETVALUE_H

#include "enumstreaming.h"

namespace snmp_interaction
{

/*!
 * \struct SetValue
 * \brief Запрос на изменение значения по ключу
 */

struct SetValue
{
    QString key;
    QVariant value;

    SetValue()
    {}
    SetValue(const QString &key_, const QVariant &value_) :
        key(key_), value(value_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const SetValue &msg)
{
    stream << msg.key;
    stream << msg.value;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SetValue &msg)
{
    stream >> msg.key;
    stream >> msg.value;

    return stream;
}

/*!
 * \struct SetValueResult
 * \brief Результат попытки изменения значения переменной
 */

struct SetValueResult
{
    QString key;

    enum SetResult {
        SR_OK
        , SR_WRONG_KEY
        , SR_READ_ONLY
        , SR_WRONG_TYPE
        , SR_WRONG_VALUE

        , SR_UNKNOWN
    } result;

    SetValueResult() :
        result(SR_UNKNOWN)
    {}
    SetValueResult(const QString &key_, SetResult result_) :
        key(key_), result(result_)
    {}

    static inline const QString toString(SetValueResult::SetResult sr)
    {
        switch (sr) {
        case SR_OK:
            return QObject::tr("Ok");
        case SR_WRONG_KEY:
            return QObject::tr("The key is not supported");
        case SR_READ_ONLY:
            return QObject::tr("Attempted to change read-only value");
        case SR_WRONG_TYPE:
            return QObject::tr("Attempted to change value of different type");
        case SR_WRONG_VALUE:
            return QObject::tr("Attempted to set wrong value");
        case SR_UNKNOWN:
            return QObject::tr("Unknown error");
        }
        return QObject::tr("Unhandled");
    }
};
QT_DECLARE_ENUM_STREAMING(snmp_interaction::SetValueResult::SetResult)

inline QDataStream & operator <<(QDataStream &stream, const SetValueResult &msg)
{
    stream << msg.key;
    stream << msg.result;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SetValueResult &msg)
{
    stream >> msg.key;
    stream >> msg.result;

    return stream;
}

} // namespace snmp_interaction

Q_DECLARE_METATYPE(snmp_interaction::SetValue)
Q_DECLARE_METATYPE(snmp_interaction::SetValueResult)
Q_DECLARE_METATYPE(snmp_interaction::SetValueResult::SetResult)

#endif // SETVALUE_H
