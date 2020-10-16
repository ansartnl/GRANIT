#ifndef ACKMESSAGE_H
#define ACKMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AckStruct
 * \brief Структура сообщения ACK
 */

struct AckStruct : public AtsStruct
{
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const AckStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AckStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.other;
    return stream;
}

/*!
 * \class AckMessage
 * \brief Класс ACK сообщения
 */

class AckMessage : public AtsMessage
{
public:
    AckMessage();
    AckMessage(const AckStruct &s);
    virtual ~AckMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    AckStruct mAck;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ACKMESSAGE_H
