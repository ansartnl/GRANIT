#ifndef CNLRPLMESSAGE_H
#define CNLRPLMESSAGE_H

#include "FplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct CnlRplStruct
 * \brief Структура сообщения отмены RPL
 */

struct CnlRplStruct : FplStruct
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

    CnlRplStruct() :
        days(NONE)
    {}
    CnlRplStruct(const FplStruct &s) :
        FplStruct(s), days(NONE)
    {}
};
QT_DECLARE_ENUM_STREAMING(CnlRplStruct::Days)

inline QDataStream & operator<< (QDataStream &stream, const CnlRplStruct &msg)
{
    stream << (const FplStruct &)msg;
    stream << msg.validFrom;
    stream << msg.validTill;
    stream << msg.days;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, CnlRplStruct &msg)
{
    stream >> (FplStruct &)msg;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    stream >> msg.days;
    return stream;
}

/*!
 * \class CnlRplMessage
 * \brief Класс CNL RPL сообщения
 */

class CnlRplMessage : public AtsMessage
{
public:
    CnlRplMessage();
    CnlRplMessage(const CnlRplStruct &s);
    virtual ~CnlRplMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    CnlRplStruct mRpl;

    inline friend QDataStream & operator<< (QDataStream &stream, const CnlRplMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mRpl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, CnlRplMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mRpl;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CNLRPLMESSAGE_H
