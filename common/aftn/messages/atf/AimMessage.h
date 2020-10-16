#ifndef AIMMESSAGE_H
#define AIMMESSAGE_H

#include "AtfMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AimStruct
 * \brief Структура сообщения AIM
 */

struct AimStruct : AtfStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const AimStruct &msg)
{
    stream << (const AtfStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AimStruct &msg)
{
    stream >> (AtfStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class AimMessage
 * \brief Класс сообщения AIM
 */

class AimMessage : public AtfMessage
{
public:
    AimMessage();
    AimMessage(const AimStruct &s);
    virtual ~AimMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtfStruct & atf();
    const AtfStruct & atfConst() const;

    AimStruct mAim;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AimStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AimStruct)
#endif

#endif // AIMMESSAGE_H
