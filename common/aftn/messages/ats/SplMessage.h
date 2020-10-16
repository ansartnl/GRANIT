#ifndef SPLMESSAGE_H
#define SPLMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SplStruct
 * \brief Структура сообщения SPL
 */

struct SplStruct
{
    QString acid;

    QString adep;
    QDateTime departureTime;

    QString ades;
    QString eet;
    QDateTime arrivalTime;

    QString other;
    QString additional;

    ParseVersion::Version version;
};

inline QDataStream & operator<< (QDataStream &stream, const SplStruct &msg)
{
    stream << msg.acid;
    stream << msg.adep;
    stream << msg.departureTime;
    stream << msg.ades;
    stream << msg.eet;
    stream << msg.arrivalTime;
    stream << msg.other;
    stream << msg.additional;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, SplStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.adep;
    stream >> msg.departureTime;
    stream >> msg.ades;
    stream >> msg.eet;
    stream >> msg.arrivalTime;
    stream >> msg.other;
    stream >> msg.additional;
    return stream;
}

/*!
 * \class SplMessage
 * \brief Класс SPL сообщения
 */

class SplMessage : public AtsMessage
{
public:
    SplMessage();
    SplMessage(const SplStruct &s);
    virtual ~SplMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    SplStruct mSpl;

    inline friend QDataStream & operator<< (QDataStream &stream, const SplMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mSpl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, SplMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mSpl;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::SplStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(SplStruct)
#endif

#endif // SPLMESSAGE_H
