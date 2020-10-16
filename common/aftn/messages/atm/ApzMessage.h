#ifndef APZMESSAGE_H
#define APZMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ApzStruct
 * \brief Структура сообщения АПЗ
 */

struct ApzStruct : AtmStruct
{
    QString airport;
    QDateTime validFrom;
    QDateTime validTill;
};

inline QDataStream & operator<< (QDataStream &stream, const ApzStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.airport;
    stream << msg.validFrom;
    stream << msg.validTill;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ApzStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.airport;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    return stream;
}

/*!
 * \class ApzMessage
 * \brief Класс АПЗ сообщения
 */

class ApzMessage : public AtmMessage
{
public:
    ApzMessage();
    ApzMessage(const ApzStruct &s);
    virtual ~ApzMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    ApzStruct mApz;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::ApzStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(ApzStruct)
#endif

#endif // APZMESSAGE_H
