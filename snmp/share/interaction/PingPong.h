#ifndef PINGPONG_H
#define PINGPONG_H

#include <QtCore/QVariant>

namespace snmp_interaction
{

/*!
 * \struct Ping
 * \brief Пинг
 *
 * Структура для проверки соединения.
 */

struct Ping
{
    Ping()
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Ping &)
{
    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Ping &)
{
    return stream;
}

} // namespace snmp_interaction

Q_DECLARE_METATYPE(snmp_interaction::Ping)

#endif // PINGPONG_H
