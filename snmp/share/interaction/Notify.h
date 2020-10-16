#ifndef NOTIFY_H
#define NOTIFY_H

#include <QtCore/QVariant>

namespace snmp_interaction
{

/*!
 * \struct Var
 * \brief Одно из значений, передающихся вместе с нотификацией
 */

struct Var
{
    QString key;
    QVariant value;
    Var()
    {}
    Var(const QString &key_, const QVariant &value_) :
        key(key_), value(value_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Var &msg)
{
    stream << msg.key;
    stream << msg.value;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Var &msg)
{
    stream >> msg.key;
    stream >> msg.value;

    return stream;
}

/*!
 * \class Vars
 * \brief Список значений, передающихся вместе с нотификацией
 */

typedef QList < Var > Vars;

/*!
 * \struct Notify
 * \brief Нотификация агента о событии
 */

struct Notify
{
    QString key;
    Vars vars;

    Notify()
    {}
    Notify(const QString &key_, const Vars &vars_) :
        key(key_), vars(vars_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Notify &msg)
{
    stream << msg.key;
    stream << msg.vars;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Notify &msg)
{
    stream >> msg.key;
    stream >> msg.vars;

    return stream;
}

} // namespace snmp_interaction

Q_DECLARE_METATYPE(snmp_interaction::Var)
Q_DECLARE_METATYPE(snmp_interaction::Vars)
Q_DECLARE_METATYPE(snmp_interaction::Notify)

#endif // NOTIFY_H
