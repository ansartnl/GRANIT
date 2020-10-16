#include "FplStructCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "FplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

FplStructCreator::FplStructCreator()
{
}

const FplStruct FplStructCreator::createFplStruct(const QString &type, const QVariantMap &map) const
{
    FplStruct fpl;
    fpl.version = aftnParameters->supportedTypes()->version(type);

    fpl.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), fpl.version);
    if (fpl.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    fpl.flightRules = Transliteration::toVersion(map.value(AFTN_MESSAGE_FLIGHT_RULES).toString()
                                                 , fpl.version);
    fpl.typeOfFlight = Transliteration::toVersion(map.value(AFTN_MESSAGE_FLIGHT_TYPE).toString()
                                                  , fpl.version);

    fpl.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    fpl.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString()
                                                              , fpl.version);
    if (fpl.aircraftType.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidAircraftType));
    fpl.turbulence = Transliteration::toVersion(map.value(AFTN_MESSAGE_TURBULENCE).toString(), fpl.version);

    fpl.equipment = Transliteration::toVersion(map.value(AFTN_MESSAGE_EQUIPMENT).toString().trimmed()
                                               , fpl.version);

    fpl.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString()
                                          , fpl.version);
    if (fpl.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    fpl.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    fpl.speed = Transliteration::toVersion(map.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString()
                                           , fpl.version);
    fpl.flightLevel = Transliteration::toVersion(map.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString()
                                                 , fpl.version);
    fpl.route = Transliteration::routeToVersion(map.value(AFTN_MESSAGE_ROUTE).toString(), fpl.version);

    fpl.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString()
                                          , fpl.version);
    if (fpl.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    fpl.eet = map.value(AFTN_MESSAGE_ELAPSED_TIME).toString();
    fpl.alt1 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString(), fpl.version);
    fpl.alt2 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString(), fpl.version);
    if (aftnParameters->nonstandardAlternativeAerodromesEnabled())
    {
        QStringList alt = map.value(AFTN_MESSAGE_OTHER_ALTERNATIVE_AERODROMES).toStringList();
        foreach (const QString &a, alt)
            fpl.otherAlternative << Transliteration::toVersion(a, fpl.version);
    }

    fpl.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), fpl.version);
    fpl.letter = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO_STS).toString(), fpl.version);

    fpl.exFpl = FplStruct::ExFplType(0);
    fpl.enabled = 0;

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        fpl.departureTime.setDate(dof);
    }

    QRegExp eetRegExp("^(\\d{2}):?(\\d{2})$");
    if (eetRegExp.exactMatch(fpl.eet)) {
        fpl.arrivalTime = fpl.departureTime.addSecs(eetRegExp.cap(1).toUInt() * 3600
                                                    + eetRegExp.cap(2).toUInt() * 60);
    }

    fpl.iFplId = map.value(AFTN_MESSAGE_FPL_ID).toString();
    fpl.boardNumber = map.value(AFTN_MESSAGE_OTHER_BOARD_NUMBER).toString();

    fpl.additionalInfo = map.value(AFTN_MESSAGE_ADDITIONAL_INFO).toString();

    return fpl;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
