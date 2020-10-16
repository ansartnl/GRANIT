#ifndef RPLMESSAGE_H
#define RPLMESSAGE_H

#include "FplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RplStruct
 * \brief Структура сообщения RPL
 */

struct RplStruct : FplStruct
{
    QDate validFrom;
    QDate validTill;
    enum Day {
        NONE        = 0x0000

        , MONDAY    = 0x0001
        , TUESDAY   = 0x0002
        , WEDNESDAY = 0x0004
        , THURSDAY  = 0x0008
        , FRIDAY    = 0x0010
        , SATURDAY  = 0x0020
        , SUNDAY    = 0x0040

        , ALL = MONDAY | TUESDAY | WEDNESDAY | THURSDAY | FRIDAY | SATURDAY | SUNDAY
    };
    typedef QFlags < Day > Days;
    Days days;

    RplStruct() :
        days(NONE)
    {}
    RplStruct(const FplStruct &s) :
        FplStruct(s), days(NONE)
    {}
};
QT_DECLARE_ENUM_STREAMING(RplStruct::Days)

inline QDataStream & operator<< (QDataStream &stream, const RplStruct &msg)
{
    stream << (const FplStruct &)msg;
    stream << msg.validFrom;
    stream << msg.validTill;
    stream << msg.days;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RplStruct &msg)
{
    stream >> (FplStruct &)msg;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    stream >> msg.days;
    return stream;
}

/*!
 * \class RplMessage
 * \brief Класс RPL сообщения
 */

class RplMessage : public AtsMessage
{
public:
    RplMessage();
    RplMessage(const RplStruct &s);
    virtual ~RplMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    RplStruct mRpl;

    inline friend QDataStream & operator<< (QDataStream &stream, const RplMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mRpl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, RplMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mRpl;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RPLMESSAGE_H
