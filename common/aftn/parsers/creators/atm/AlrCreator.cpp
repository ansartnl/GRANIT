#include "AlrCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AlrMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AlrCreator::AlrCreator()
{
    addType(TelegramType::ALR);
    addType(TelegramType::ALR1);
}

AftnMessage * AlrCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AlrStruct alr;

    alr.version = aftnParameters->supportedTypes()->version(type);

    alr.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), alr.version);

    alr.emergencyState = Transliteration::toVersion(map.value(AFTN_MESSAGE_EMERGENCY_STAGE).toString()
                                                    , alr.version);
    alr.emergencySender = Transliteration::toVersion(map.value(AFTN_MESSAGE_EMERGENCY_SENDER).toString()
                                                     , alr.version);
    alr.emergencyMessage = Transliteration::toVersion(map.value(AFTN_MESSAGE_EMERGENCY_MESSAGE).toString()
                                                      , alr.version);

    alr.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), alr.version);
    if (alr.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    alr.flightRules = Transliteration::toVersion(map.value(AFTN_MESSAGE_FLIGHT_RULES).toString()
                                                 , alr.version);
    alr.typeOfFlight = Transliteration::toVersion(map.value(AFTN_MESSAGE_FLIGHT_TYPE).toString()
                                                  , alr.version);

    alr.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    alr.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString()
                                                              , alr.version);
    if (alr.aircraftType.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidAircraftType));
    alr.turbulence = Transliteration::toVersion(map.value(AFTN_MESSAGE_TURBULENCE).toString(), alr.version);

    alr.equipment = Transliteration::toVersion(map.value(AFTN_MESSAGE_EQUIPMENT).toString().trimmed()
                                               , alr.version);

    alr.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString()
                                          , alr.version);
    if (alr.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    alr.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    alr.speed = Transliteration::toVersion(map.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString()
                                           , alr.version);
    alr.flightLevel = Transliteration::toVersion(map.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString()
                                                 , alr.version);
    alr.route = Transliteration::routeToVersion(map.value(AFTN_MESSAGE_ROUTE).toString(), alr.version);

    alr.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString()
                                          , alr.version);
    if (alr.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    alr.eet = map.value(AFTN_MESSAGE_ELAPSED_TIME).toString();
    alr.alt1 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString(), alr.version);
    alr.alt2 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString(), alr.version);

    alr.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), alr.version);

    alr.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), alr.version);

    alr.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
    {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        alr.departureTime.setDate(dof);
    }

    QRegExp eetRegExp("^(\\d{2}):?(\\d{2})$");
    if (eetRegExp.exactMatch(alr.eet)) {
        alr.arrivalTime = alr.departureTime.addSecs(eetRegExp.cap(1).toUInt() * 3600
                                                    + eetRegExp.cap(2).toUInt() * 60);
    }

    alr.additionalInfo = Transliteration::toVersion(map.value(AFTN_MESSAGE_ADDITIONAL_INFO).toString()
                                                              , alr.version);
    alr.rescueInfo = Transliteration::toVersion(map.value(AFTN_MESSAGE_RESCUE_INFO).toString()
                                                              , alr.version);
    return new AlrMessage(alr);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
