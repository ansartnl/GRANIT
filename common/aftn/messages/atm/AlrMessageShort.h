#ifndef ALRMESSAGESHORT_H
#define ALRMESSAGESHORT_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AlrStructShort
 * \brief Структура сообщения АЛР/ALR по ТС95
 */

struct AlrStructShort : AtmStruct
{
    QString acid;

    int numberOfAircrafts;
    QString aircraftType;
};

inline QDataStream & operator<< (QDataStream &stream, const AlrStructShort &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.acid;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AlrStructShort &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.acid;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    return stream;
}

/*!
 * \class AlrMessageShort
 * \brief Класс АЛР/ALR сообщений по ТС95
 */

class AlrMessageShort : public AtmMessage
{
public:
    AlrMessageShort();
    AlrMessageShort(const AlrStructShort &s);
    virtual ~AlrMessageShort();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    AlrStructShort mAlr;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AlrStructShort)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AlrStructShort)
#endif

#endif // ALRMESSAGESHORT_H
