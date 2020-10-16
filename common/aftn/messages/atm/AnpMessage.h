#ifndef ANPMESSAGE_H
#define ANPMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AnpStruct
 * \brief Структура сообщения АНП
 */

struct AnpStruct : AtmStruct
{
    QString acid;

    int numberOfAircrafts;
    QString aircraftType;
};

inline QDataStream & operator<< (QDataStream &stream, const AnpStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.acid;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AnpStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.acid;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    return stream;
}

/*!
 * \class AnpMessage
 * \brief Класс АНП сообщения
 */

class AnpMessage : public AtmMessage
{
public:
    AnpMessage();
    AnpMessage(const AnpStruct &s);
    virtual ~AnpMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    AnpStruct mAnp;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AnpStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AnpStruct)
#endif

#endif // ANPMESSAGE_H
