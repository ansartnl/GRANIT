#ifndef ATFFPLMESSAGE_H
#define ATFFPLMESSAGE_H

#include "AtfMessage.h"
#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AtfFplStruct
 * \brief Структура сообщений ОПВД, связанных с планом
 *
 * Данная структура содержит общие поля телеграмм RRP/РРП, SAM/САМ, SLC/СЛЦ, SLT/СЛТ, CSL.
 */

struct AtfFplStruct : AtfStruct
{
    QString acid;
    QString adep;
    QString ades;
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const AtfFplStruct &msg)
{
    stream << (const AtfStruct &)msg;
    stream << msg.acid;
    stream << msg.adep;
    stream << msg.ades;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtfFplStruct &msg)
{
    stream >> (AtfStruct &)msg;
    stream >> msg.acid;
    stream >> msg.adep;
    stream >> msg.ades;
    stream >> msg.other;
    return stream;
}

/*!
 * \class AtfFplMessage
 * \brief Класс ОПВД сообщения, связанного с планом
 */

class AtfFplMessage : public AtfMessage
{
public:
    AtfFplMessage();
    AtfFplMessage(const AtfFplStruct &s);
    virtual ~AtfFplMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    virtual AtsStruct ats() const;

    AtfFplStruct mAtfFpl;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AtfFplStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AtfFplStruct)
#endif

#endif // ATFFPLMESSAGE_H
