#ifndef RJIMESSAGE_H
#define RJIMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RviStruct
 * \brief Структура сообщения РЖИ
 */

struct RviStruct : AtmStruct
{
    QString number;
};

inline QDataStream & operator<< (QDataStream &stream, const RviStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.number;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RviStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.number;
    return stream;
}

/*!
 * \class RviMessage
 * \brief Класс РЖИ сообщения
 */

class RviMessage : public AtmMessage
{
public:
    RviMessage();
    RviMessage(const RviStruct &s);
    virtual ~RviMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    RviStruct mRvi;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RviStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RviStruct)
#endif

#endif // RJIMESSAGE_H
