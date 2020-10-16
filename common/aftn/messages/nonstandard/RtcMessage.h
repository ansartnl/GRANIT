#ifndef RTCMESSAGE_H
#define RTCMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RtcStruct
 * \brief Структура сообщения RTC
 */

struct RtcStruct
{
    QDateTime time;

    RtcStruct() :
        time(QDateTime::currentDateTimeUtc())
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const RtcStruct &msg)
{
    stream << msg.time;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RtcStruct &msg)
{
    stream >> msg.time;
    return stream;
}

/*!
 * \class RtcMessage
 * \brief Класс RTC сообщения
 */

class RtcMessage : public AftnMessage
{
public:
    RtcMessage();
    RtcMessage(const RtcStruct &s);
    virtual ~RtcMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    RtcStruct mRtc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RTCMESSAGE_H
