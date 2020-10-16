#ifndef RJMMESSAGE_H
#define RJMMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RvmStruct
 * \brief Структура сообщения РЖМ
 */

struct RvmStruct : AtmStruct
{
    QString number;
};

inline QDataStream & operator<< (QDataStream &stream, const RvmStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.number;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RvmStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.number;
    return stream;
}

/*!
 * \class RvmMessage
 * \brief Класс РЖМ сообщения
 */

class RvmMessage : public AtmMessage
{
public:
    RvmMessage();
    RvmMessage(const RvmStruct &s);
    virtual ~RvmMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    RvmStruct mRvm;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RvmStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RvmStruct)
#endif

#endif // RJMMESSAGE_H
