#ifndef APVMESSAGE_H
#define APVMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ApwStruct
 * \brief Структура сообщения АПВ
 */

struct ApwStruct : AtmStruct
{
    QString airport;
    QDateTime validFrom;
};

inline QDataStream & operator<< (QDataStream &stream, const ApwStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.airport;
    stream << msg.validFrom;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ApwStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.airport;
    stream >> msg.validFrom;
    return stream;
}

/*!
 * \class ApwMessage
 * \brief Класс АПВ сообщения
 */

class ApwMessage : public AtmMessage
{
public:
    ApwMessage();
    ApwMessage(const ApwStruct &s);
    virtual ~ApwMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    ApwStruct mApw;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::ApwStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(ApwStruct)
#endif

#endif // APVMESSAGE_H
