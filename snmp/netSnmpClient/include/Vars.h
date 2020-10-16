#ifndef VARS_H
#define VARS_H

#include <QtCore/QVariant>

namespace snmp
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

} // namespace snmp

#endif // VARS_H
