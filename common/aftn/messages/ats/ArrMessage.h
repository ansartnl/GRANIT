#ifndef ARRMESSAGE_H
#define ARRMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ArrStruct
 * \brief Структура сообщения ARR
 */

struct ArrStruct : AtsStruct
{
    QString arrivalAirport;
    QString arrivalAirportName;
    QDateTime arrivalTime;
    QDateTime departureTime;
#if defined(BAN_BUILD)
    QString other; // optional
#endif
};

inline QDataStream & operator<< (QDataStream &stream, const ArrStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.arrivalAirport;
    stream << msg.arrivalAirportName;
    stream << msg.arrivalTime;
    stream << msg.departureTime;
#if defined(BAN_BUILD)
    stream << msg.other;
#endif
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ArrStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.arrivalAirport;
    stream >> msg.arrivalAirportName;
    stream >> msg.arrivalTime;
    stream >> msg.departureTime;
#if defined(BAN_BUILD)
    stream >> msg.other;
#endif
    return stream;
}

/*!
 * \class ArrMessage
 * \brief Класс ARR сообщения
 */

class ArrMessage : public AtsMessage
{
public:
    ArrMessage();
    ArrMessage(const ArrStruct &s);
    virtual ~ArrMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    ArrStruct mArr;

    inline friend QDataStream & operator<< (QDataStream &stream, const ArrMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mArr;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, ArrMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mArr;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::ArrStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(ArrStruct)
#endif

#endif // ARRMESSAGE_H
