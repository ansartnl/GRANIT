#ifndef TRAPINTERACTION_H
#define TRAPINTERACTION_H

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

namespace trap_interaction
{

/*!
 * \struct Trap
 * \brief Ловушка получена
 *
 * Структура, которую отправляет обработчик ловушек, при получении очередной.
 */

struct Trap
{
    QString ip;
    QString hostname;
    QString name;
    QVariantMap variables;

    Trap()
    {}
    Trap(const QString &ip_, const QString &hostname_, const QString &name_, const QVariantMap &variables_ = QVariantMap()) :
        ip(ip_), hostname(hostname_), name(name_), variables(variables_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const Trap &msg)
{
    stream << msg.ip;
    stream << msg.hostname;
    stream << msg.name;
    stream << msg.variables;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, Trap &msg)
{
    stream >> msg.ip;
    stream >> msg.hostname;
    stream >> msg.name;
    stream >> msg.variables;

    return stream;
}

} // namespace trap_interaction

Q_DECLARE_METATYPE(trap_interaction::Trap)

#endif // TRAPINTERACTION_H
