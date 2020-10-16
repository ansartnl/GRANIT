#ifndef ATMMESSAGE_H
#define ATMMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDate>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AtmStruct
 * \brief Структура сообщений ИВП
 *
 * Данная структура содержит общие поля телеграмм АНП, РЖМ, РЖИ, АПЗ, АПВ, ALR.
 */

struct AtmStruct
{
    QString type;
    QString text;
    QString other;
    QDate dof;

    ParseVersion::Version version;
};
Q_DECLARE_METATYPE(AtmStruct)

inline QDataStream & operator<< (QDataStream &stream, const AtmStruct &msg)
{
    stream << msg.type;
    stream << msg.text;
    stream << msg.other;
    stream << msg.dof;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtmStruct &msg)
{
    stream >> msg.type;
    stream >> msg.text;
    stream >> msg.other;
    stream >> msg.dof;
    stream >> msg.version;
    return stream;
}

/*!
 * \class AtmMessage
 * \brief Класс ИВП сообщения
 */

class AtmMessage : public AftnMessage
{
public:
    AtmMessage();
    AtmMessage(const AtmStruct &s);
    virtual ~AtmMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    virtual AtmStruct & atm();
    virtual const AtmStruct & atmConst() const;

private:
    AtmStruct mAtm;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ATMMESSAGE_H
