#ifndef SPSMESSAGE_H
#define SPSMESSAGE_H

#include "AtfMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SpsStruct
 * \brief Структура сообщения SPS/СПС
 */

struct SpsStruct : AtfStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const SpsStruct &msg)
{
    stream << (const AtfStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, SpsStruct &msg)
{
    stream >> (AtfStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class SpsMessage
 * \brief Класс сообщения SPS/СПС
 */

class SpsMessage : public AtfMessage
{
public:
    SpsMessage();
    SpsMessage(const SpsStruct &s);
    virtual ~SpsMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    SpsStruct mSps;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::SpsStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(SpsStruct)
#endif

#endif // SPSMESSAGE_H
