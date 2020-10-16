#ifndef UTPMESSAGE_H
#define UTPMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct UtpStruct
 * \brief Структура сообщения UTP
 */

struct UtpStruct : AtsStruct
{
    // 7
//    QString acid;
    // 9
    QStringList aircraft;
    // 13
//    QString adep;
    QDateTime departureTime;
    QDateTime endExploration;
    QDateTime beginFlights;
    QDateTime endFlights;
    // 15
    QString route;
    // 16
//    QString ades;
    QString alt1;
    QString alt2;

    QDateTime arrivalTime;
    // 18
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const UtpStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.aircraft;
    stream << msg.departureTime;
    stream << msg.endExploration;
    stream << msg.beginFlights;
    stream << msg.endFlights;
    stream << msg.route;
    stream << msg.alt1;
    stream << msg.alt2;
    stream << msg.arrivalTime;
    stream << msg.other;

    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, UtpStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.aircraft;
    stream >> msg.departureTime;
    stream >> msg.endExploration;
    stream >> msg.beginFlights;
    stream >> msg.endFlights;
    stream >> msg.route;
    stream >> msg.alt1;
    stream >> msg.alt2;
    stream >> msg.arrivalTime;
    stream >> msg.other;

    return stream;
}

/*!
 * \class UtpMessage
 * \brief Класс UTP сообщения
 */

class UtpMessage : public AtsMessage
{
public:
    UtpMessage();
    UtpMessage(const UtpStruct &s);
    virtual ~UtpMessage();

    QString data() const;
    //QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    UtpStruct mUtp;

    inline friend QDataStream & operator<< (QDataStream &stream, const UtpMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mUtp;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, UtpMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mUtp;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::UtpStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(UtpStruct)
#endif


#endif // UTPMESSAGE_H
