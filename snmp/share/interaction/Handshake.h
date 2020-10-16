#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include <QtCore/QVariant>

namespace snmp_interaction
{

/*!
 * \struct Handshake
 * \brief Рукопожатие клиента и сервера
 *
 * Стартовая структура, которой обмениваются клиент и сервер при подключении.
 */

struct Handshake
{
    QString name;

    Handshake()
    {}
    Handshake(const QString &name_) :
        name(name_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Handshake &msg)
{
    stream << msg.name;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Handshake &msg)
{
    stream >> msg.name;

    return stream;
}

} // namespace snmp_interaction

Q_DECLARE_METATYPE(snmp_interaction::Handshake)

#endif // HANDSHAKE_H
