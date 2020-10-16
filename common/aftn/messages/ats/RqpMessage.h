#ifndef RQPMESSAGE_H
#define RQPMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RqpStruct
 * \brief Структура сообщения RQP
 */

struct RqpStruct : AtsStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const RqpStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RqpStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class RqpMessage
 * \brief Класс RQP сообщения
 */

class RqpMessage : public AtsMessage
{
public:
    RqpMessage();
    RqpMessage(const RqpStruct &s);
    virtual ~RqpMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    RqpStruct mRqp;

    inline friend QDataStream & operator<< (QDataStream &stream, const RqpMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mRqp;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, RqpMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mRqp;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RqpStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RqpStruct)
#endif

#endif // RQPMESSAGE_H
