#ifndef ATSMESSAGE_H
#define ATSMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct AtsStruct
 * \brief Структура ОВД сообщения
 *
 * Данная структура содержит минимальный набор полей, необходимый для идентификации плана.
 * Данные поля являются обязательными для всех ОВД сообщений (для сообщения ARR, а так же
 * для сообщений CHG, CNL, DEP, DLA до поправки №1 поле \a dof == QDateTime::currentDateTimeUtc())
 */

struct AtsStruct
{
    QString acid;
    QString adep;
    QString ades;
    QDate dof;
    QString boardNumber;
    QString iFplId;

    ParseVersion::Version version;

    AtsStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const AtsStruct &msg)
{
    stream << msg.acid;
    stream << msg.adep;
    stream << msg.ades;
    stream << msg.dof;
    stream << msg.boardNumber;
    stream << msg.iFplId;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, AtsStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.adep;
    stream >> msg.ades;
    stream >> msg.dof;
    stream >> msg.boardNumber;
    stream >> msg.iFplId;
    stream >> msg.version;
    return stream;
}

/*!
 * \struct FplFields
 * \brief Структура полей плана
 */

struct FplFields
{
    // 7
    QString acid;
    // 8
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
    QStringList otherAlternative;

    QDateTime arrivalTime;
    // 18
    QString other;

    // 19
    QString additionalInfo;

    //STS
    QString letter;
    QString boardNumber;

    QString iFplId;

    enum ExFplType {
        DEFAULT = 0
        , DEPARTURED = 1
        , ARRIVED = 2
        , DELAYED = 3
        , CANCELLED = 4
        , ALTERNATIVE = 5
    } exFpl;

    int enabled;

    ParseVersion::Version version;

    FplFields() :
        numberOfAircrafts(0), exFpl(DEFAULT), enabled(0), version(ParseVersion::PV_UNKNOWN)
    {}
};
QT_DECLARE_ENUM_STREAMING(FplFields::ExFplType)

inline QDataStream & operator<< (QDataStream &stream, const FplFields &msg)
{
    stream << msg.acid;
    stream << msg.flightRules;
    stream << msg.typeOfFlight;
    stream << msg.numberOfAircrafts;
    stream << msg.aircraftType;
    stream << msg.turbulence;
    stream << msg.equipment;
    stream << msg.adep;
    stream << msg.departureTime;
    stream << msg.speed;
    stream << msg.flightLevel;
    stream << msg.route;
    stream << msg.ades;
    stream << msg.eet;
    stream << msg.alt1;
    stream << msg.alt2;
    stream << msg.otherAlternative;
    stream << msg.arrivalTime;
    stream << msg.other;
    stream << msg.letter;
    stream << msg.boardNumber;
    stream << msg.exFpl;
    stream << msg.enabled;
    stream << msg.version;
    stream << msg.iFplId;
    stream << msg.additionalInfo;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FplFields &msg)
{
    stream >> msg.acid;
    stream >> msg.flightRules;
    stream >> msg.typeOfFlight;
    stream >> msg.numberOfAircrafts;
    stream >> msg.aircraftType;
    stream >> msg.turbulence;
    stream >> msg.equipment;
    stream >> msg.adep;
    stream >> msg.departureTime;
    stream >> msg.speed;
    stream >> msg.flightLevel;
    stream >> msg.route;
    stream >> msg.ades;
    stream >> msg.eet;
    stream >> msg.alt1;
    stream >> msg.alt2;
    stream >> msg.otherAlternative;
    stream >> msg.arrivalTime;
    stream >> msg.other;
    stream >> msg.letter;
    stream >> msg.boardNumber;
    stream >> msg.exFpl;
    stream >> msg.enabled;
    stream >> msg.version;
    stream >> msg.iFplId;
    stream >> msg.additionalInfo;
    return stream;
}

inline bool operator ==(const FplFields &s1, const FplFields &s2)
{
    if (s1.acid != s2.acid)
        return false;
    if (s1.flightRules != s2.flightRules)
        return false;
    if (s1.typeOfFlight != s2.typeOfFlight)
        return false;
    if (s1.numberOfAircrafts != s2.numberOfAircrafts)
        return false;
    if (s1.aircraftType != s2.aircraftType)
        return false;
    if (s1.turbulence != s2.turbulence)
        return false;
    if (s1.equipment != s2.equipment)
        return false;
    if (s1.adep != s2.adep)
        return false;
    if (s1.departureTime != s2.departureTime)
        return false;
    if (s1.speed != s2.speed)
        return false;
    if (s1.flightLevel != s2.flightLevel)
        return false;
    if (s1.route != s2.route)
        return false;
    if (s1.ades != s2.ades)
        return false;
    if (s1.eet != s2.eet)
        return false;
    if (s1.alt1 != s2.alt1)
        return false;
    if (s1.alt2 != s2.alt2)
        return false;
    if (s1.otherAlternative != s2.otherAlternative)
        return false;
    if (s1.arrivalTime != s2.arrivalTime)
        return false;
    if (s1.other != s2.other)
        return false;
    if (s1.letter != s2.letter)
        return false;
    if (s1.boardNumber != s2.boardNumber)
        return false;
    if (s1.iFplId != s2.iFplId)
        return false;
    if (s1.exFpl != s2.exFpl)
        return false;
    if (s1.enabled != s2.enabled)
        return false;
    if (s1.version != s2.version)
        return false;
    if (s1.additionalInfo != s2.additionalInfo)
        return false;
    return true;
}

/*!
 * \struct FplChange
 * \brief Структура изменения полей плана
 */

struct FplChange : FplFields
{
    enum Change {
        NO_CHANGE = 0x00000000
        , FLIGHT_RULES = 0x00000001
        , TYPE_OF_FLIGHT = 0x00000002
        , NUMBER_OF_AIRCRAFTS = 0x00000004
        , AIRCRAFT_TYPE = 0x00000008
        , TURBULENCE = 0x00000010
        , EQUIPMENT = 0x00000020
        , SPEED = 0x00000040
        , FLIGHT_LEVEL = 0x00000080
        , ROUTE = 0x00000100
        , OTHER = 0x00000200
        , DEPARTURE_TIME = 0x00000400
        , ARRIVAL_TIME = 0x00000800
        , ALT1 = 0x00001000
        , ALT2 = 0x00002000
        , EET = 0x00004000
        , EX_FPL = 0x00008000
        , OTHER_STS = 0x00100000

        , CHANGE_ACID = 0x00020000
        , CHANGE_ADEP = 0x00040000
        , CHANGE_ADES = 0x00080000

        , ACTUAL_DEPARTURE_TIME = 0x00100000

        , OTHER_ALTERNATIVE = 0x00200000

        , ADDITIONAL_INFO = 0x00400000

        , BOARD_NUMBER = 0x00800000

        , ALL = FLIGHT_RULES | TYPE_OF_FLIGHT | NUMBER_OF_AIRCRAFTS | AIRCRAFT_TYPE | TURBULENCE
        | EQUIPMENT | SPEED | FLIGHT_LEVEL | ROUTE | OTHER | DEPARTURE_TIME | ARRIVAL_TIME
        | ALT1 | ALT2 | EET | EX_FPL | OTHER_STS | CHANGE_ACID | CHANGE_ADEP | CHANGE_ADES
        | ACTUAL_DEPARTURE_TIME | OTHER_ALTERNATIVE | ADDITIONAL_INFO | BOARD_NUMBER
    };
    typedef QFlags < Change > Changes;
    Changes changesFlags;

    QDateTime actualDepartureTime;

    FplChange() :
        changesFlags(NO_CHANGE)
    {}
    FplChange(const FplFields &f) :
        FplFields(f), changesFlags(NO_CHANGE)
    {}
};
QT_DECLARE_ENUM_STREAMING(FplChange::Changes)

inline QDataStream & operator<< (QDataStream &stream, const FplChange &msg)
{
    stream << (const FplFields &)msg;
    stream << msg.changesFlags;
    stream << msg.actualDepartureTime;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FplChange &msg)
{
    stream >> (FplFields &)msg;
    stream >> msg.changesFlags;
    stream >> msg.actualDepartureTime;
    return stream;
}

inline bool operator ==(const FplChange &s1, const FplChange &s2)
{
    if (!operator ==(static_cast<FplFields>(s1), static_cast<FplFields>(s2)))
        return false;
    if (s1.changesFlags != s2.changesFlags)
        return false;
    return true;
}

/*!
 * \class AtsMessage
 * \brief Класс ОВД сообщения
 *
 * Базовый класс для всех ОВД сообщений.
 */

class AtsMessage : public AftnMessage
{
public:
    AtsMessage();
    virtual ~AtsMessage();

    ParseVersion::Version version() const;
//    void setVersion(ParseVersion::Version v);

    virtual FplChange changes() const = 0;
    virtual AtsStruct ats() const = 0;
    QString otherToAdexp(const QString &other) const;

    inline friend QDataStream & operator<< (QDataStream &stream, const AtsMessage &msg)
    {
        stream << (const AftnMessage &)msg;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, AtsMessage &msg)
    {
        stream >> (AftnMessage &)msg;
        return stream;
    }
};

QString removeColonFromTime(const QString &time);

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ATSMESSAGE_H
