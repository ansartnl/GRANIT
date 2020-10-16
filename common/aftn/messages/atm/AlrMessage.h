#ifndef ALRMESSAGE_H
#define ALRMESSAGE_H

#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AlrStruct
 * \brief Структура сообщения АЛР/ALR по ТС2013
 */

struct AlrStruct : AtmStruct
{
    //5
    QString emergencyState;
    QString emergencySender;
    QString emergencyMessage;
    //7
    QString acid;
    //8
    QString flightRules;
    QString typeOfFlight;
    // 9
    int numberOfAircrafts;
    QString aircraftType;
    QString turbulence;
    // 10
    QString equipment;
    // 13
    QString adep;
    QDateTime departureTime;
    // 15
    QString speed;
    QString flightLevel;
    QString route;
    // 16
    QString ades;
    QString eet;
    QString alt1;
    QString alt2;

    QDateTime arrivalTime;
    // 19
    QString additionalInfo;
    //20
    QString rescueInfo;
};

inline QDataStream & operator<< (QDataStream &stream, const AlrStruct &msg)
{
    stream << (const AtmStruct &)msg;
    stream << msg.emergencyState;
    stream << msg.emergencySender;
    stream << msg.emergencyMessage;
    stream << msg.acid;
    stream << msg.flightRules;
    stream << msg.typeOfFlight;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    stream << msg.turbulence;
    stream << msg.equipment;
    stream << msg.departureTime;
    stream << msg.speed;
    stream << msg.flightLevel;
    stream << msg.route;
    stream << msg.eet;
    stream << msg.alt1;
    stream << msg.alt2;
    stream << msg.arrivalTime;
    stream << msg.additionalInfo;
    stream << msg.rescueInfo;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AlrStruct &msg)
{
    stream >> (AtmStruct &)msg;
    stream >> msg.emergencyState;
    stream >> msg.emergencySender;
    stream >> msg.emergencyMessage;
    stream >> msg.acid;
    stream >> msg.flightRules;
    stream >> msg.typeOfFlight;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    stream >> msg.turbulence;
    stream >> msg.equipment;
    stream >> msg.departureTime;
    stream >> msg.speed;
    stream >> msg.flightLevel;
    stream >> msg.route;
    stream >> msg.eet;
    stream >> msg.alt1;
    stream >> msg.alt2;
    stream >> msg.arrivalTime;
    stream >> msg.additionalInfo;
    stream >> msg.rescueInfo;
    return stream;
}

/*!
 * \class AlrMessageShort
 * \brief Класс АЛР/ALR сообщений по ТС2013
 */

class AlrMessage : public AtmMessage
{
public:
    AlrMessage();
    AlrMessage(const AlrStruct &s);
    virtual ~AlrMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    AtmStruct & atm();
    const AtmStruct & atmConst() const;

    AlrStruct mAlr;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::AlrStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(AlrStruct)
#endif

#endif // ALRMESSAGE_H
