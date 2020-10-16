#include "RrpCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RrpMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RrpCreator::RrpCreator()
{
    addType(TelegramType::RRP, TypeKey::kNonstandardDisambiguation);
    addType(TelegramType::RRP1, TypeKey::kNonstandardDisambiguation);
}

AftnMessage * RrpCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RrpStruct rrp;
    rrp.version = aftnParameters->supportedTypes()->version(type);

    rrp.type = map.value(AFTN_MESSAGE_TYPE).toString();
    rrp.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    rrp.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rrp.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    rrp.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rrp.version);
    if (rrp.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));
    rrp.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), rrp.version);
    if (rrp.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    rrp.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), rrp.version);
    if (rrp.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    rrp.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rrp.version);

    rrp.changes.changesFlags = 0;

    QVariantMap changesMap = map.value(AFTN_MESSAGE_CHANGED_FIELDS).toMap();

    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_ID))
    {
        rrp.changes.changesFlags |= FplChange::CHANGE_ACID;
        rrp.changes.acid = Transliteration::acidToVersion(changesMap.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DEPARTURE_AERODROME))
    {
        rrp.changes.changesFlags |= FplChange::CHANGE_ADEP;
        rrp.changes.adep = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DESTINATION_AERODROME))
    {
        rrp.changes.changesFlags |= FplChange::CHANGE_ADES;
        rrp.changes.ades = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DEPARTURE_TIME))
    {
        rrp.changes.changesFlags |= FplChange::DEPARTURE_TIME;
        rrp.changes.departureTime = changesMap.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();
        if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
            rrp.changes.departureTime.setDate(changesMap.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate());
    }
    if (changesMap.contains(AFTN_MESSAGE_ELAPSED_TIME))
    {
        rrp.changes.changesFlags |= FplChange::EET;
        rrp.changes.eet = changesMap.value(AFTN_MESSAGE_ELAPSED_TIME).toString();

    }
    if (changesMap.contains(AFTN_MESSAGE_FLIGHT_RULES))
    {
        rrp.changes.changesFlags |= FplChange::FLIGHT_RULES;
        rrp.changes.flightRules = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_FLIGHT_RULES).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_FLIGHT_TYPE))
    {
        rrp.changes.changesFlags |= FplChange::TYPE_OF_FLIGHT;
        rrp.changes.typeOfFlight = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_FLIGHT_TYPE).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_COUNT))
    {
        rrp.changes.changesFlags |= FplChange::NUMBER_OF_AIRCRAFTS;
        rrp.changes.numberOfAircrafts = changesMap.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    }
    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_TYPE))
    {
        rrp.changes.changesFlags |= FplChange::AIRCRAFT_TYPE;
        rrp.changes.aircraftType = Transliteration::aircraftTypeToVersion(changesMap.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_TURBULENCE))
    {
        rrp.changes.changesFlags |= FplChange::TURBULENCE;
        rrp.changes.turbulence = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_TURBULENCE).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_EQUIPMENT))
    {
        rrp.changes.changesFlags |= FplChange::EQUIPMENT;
        rrp.changes.equipment = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_EQUIPMENT).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER))
    {
        rrp.changes.changesFlags |= FplChange::SPEED;
        rrp.changes.speed = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL))
    {
        rrp.changes.changesFlags |= FplChange::FLIGHT_LEVEL;
        rrp.changes.flightLevel = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_ROUTE))
    {
        rrp.changes.changesFlags |= FplChange::ROUTE;
        rrp.changes.route = Transliteration::routeToVersion(changesMap.value(AFTN_MESSAGE_ROUTE).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_OTHER_INFO))
    {
        rrp.changes.changesFlags |= FplChange::OTHER;
        rrp.changes.other = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_OTHER_INFO).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD))
    {
        rrp.changes.changesFlags |= FplChange::ALT1;
        rrp.changes.alt1 = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD_2))
    {
        rrp.changes.changesFlags |= FplChange::ALT2;
        rrp.changes.alt2 = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString(), rrp.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_OTHER_ALTERNATIVE_AERODROMES) && aftnParameters->nonstandardAlternativeAerodromesEnabled())
    {
        rrp.changes.changesFlags |= FplChange::OTHER_ALTERNATIVE;
        QStringList alt = map.value(AFTN_MESSAGE_OTHER_ALTERNATIVE_AERODROMES).toStringList();
        foreach (const QString &a, alt)
            rrp.changes.otherAlternative << Transliteration::toVersion(a, rrp.version);
    }

    if (rrp.changes.changesFlags == FplChange::NO_CHANGE)
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidField21Format));

    return new RrpMessage(rrp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
