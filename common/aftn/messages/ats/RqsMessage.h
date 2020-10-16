#ifndef RQSMESSAGE_H
#define RQSMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RqsStruct
 * \brief Структура сообщения RQS
 */

struct RqsStruct : AtsStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const RqsStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RqsStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class RqsMessage
 * \brief Класс RQS сообщения
 */

class RqsMessage : public AtsMessage
{
public:
    RqsMessage();
    RqsMessage(const RqsStruct &s);
    virtual ~RqsMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    RqsStruct mRqs;

    inline friend QDataStream & operator<< (QDataStream &stream, const RqsMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mRqs;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, RqsMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mRqs;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RqsStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RqsStruct)
#endif

#endif // RQSMESSAGE_H
