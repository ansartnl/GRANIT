#ifndef PLRMESSAGE_H
#define PLRMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct PlrPoint
 * \brief Структура сообщения PLR
 */

struct PlrPoint
{
    QString point;
    QTime time;
    QString speed;
    QString flightLevel;
};

inline QDataStream & operator<< (QDataStream &stream, const PlrPoint &msg)
{
    stream << msg.point;
    stream << msg.time;
    stream << msg.speed;
    stream << msg.flightLevel;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, PlrPoint &msg)
{
    stream >> msg.point;
    stream >> msg.time;
    stream >> msg.speed;
    stream >> msg.flightLevel;
    return stream;
}

/*!
 * \struct PlrStruct
 * \brief Структура сообщения PLR
 */

struct PlrStruct : AtsStruct
{
    QDateTime departureTime;
    QString other;

    const QList < PlrPoint > flightPoints() const;
};

inline QDataStream & operator<< (QDataStream &stream, const PlrStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.departureTime;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, PlrStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.departureTime;
    stream >> msg.other;
    return stream;
}

/*!
 * \class PlrMessage
 * \brief Класс PLR сообщения
 */

class PlrMessage : public AtsMessage
{
public:
    PlrMessage();
    PlrMessage(const PlrStruct &s);
    virtual ~PlrMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    PlrStruct mPlr;

    inline friend QDataStream & operator<< (QDataStream &stream, const PlrMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mPlr;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, PlrMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mPlr;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PLRMESSAGE_H
