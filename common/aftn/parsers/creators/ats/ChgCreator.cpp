#include "ChgCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ChgMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ChgCreator::ChgCreator()
{
    addType(TelegramType::CHG);
    addType(TelegramType::CHG1);
    addType(TelegramType::CHG, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::CHG1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * ChgCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ChgStruct chg;
    chg.version = aftnParameters->supportedTypes()->version(type);

    chg.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), chg.version);
    if (chg.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    chg.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), chg.version);
    if (chg.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    chg.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), chg.version);
    if (chg.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    chg.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), chg.version);
    chg.dof = QDateTime::currentDateTimeUtc().date();

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        chg.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    chg.changes.changesFlags = 0;

    QVariantMap changesMap = map.value(AFTN_MESSAGE_CHANGED_FIELDS).toMap();

    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_ID))
    {
        chg.changes.changesFlags |= FplChange::CHANGE_ACID;
        chg.changes.acid = Transliteration::acidToVersion(changesMap.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DEPARTURE_AERODROME))
    {
        chg.changes.changesFlags |= FplChange::CHANGE_ADEP;
        chg.changes.adep = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DESTINATION_AERODROME))
    {
        chg.changes.changesFlags |= FplChange::CHANGE_ADES;
        chg.changes.ades = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DEPARTURE_TIME))
    {
        chg.changes.changesFlags |= FplChange::DEPARTURE_TIME;
        chg.changes.departureTime = changesMap.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();
        chg.changes.departureTime.setDate(chg.dof);
        if (changesMap.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
            chg.changes.departureTime.setDate(changesMap.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate());
    }
    if (changesMap.contains(AFTN_MESSAGE_ELAPSED_TIME))
    {
        chg.changes.changesFlags |= FplChange::EET;
        chg.changes.eet = changesMap.value(AFTN_MESSAGE_ELAPSED_TIME).toString();

    }
    if (changesMap.contains(AFTN_MESSAGE_FLIGHT_RULES))
    {
        chg.changes.changesFlags |= FplChange::FLIGHT_RULES;
        chg.changes.flightRules = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_FLIGHT_RULES).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_FLIGHT_TYPE))
    {
        chg.changes.changesFlags |= FplChange::TYPE_OF_FLIGHT;
        chg.changes.typeOfFlight = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_FLIGHT_TYPE).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_COUNT))
    {
        chg.changes.changesFlags |= FplChange::NUMBER_OF_AIRCRAFTS;
        chg.changes.numberOfAircrafts = changesMap.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    }
    if (changesMap.contains(AFTN_MESSAGE_AIRCRAFT_TYPE))
    {
        chg.changes.changesFlags |= FplChange::AIRCRAFT_TYPE;
        chg.changes.aircraftType = Transliteration::aircraftTypeToVersion(changesMap.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_TURBULENCE))
    {
        chg.changes.changesFlags |= FplChange::TURBULENCE;
        chg.changes.turbulence = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_TURBULENCE).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_EQUIPMENT))
    {
        chg.changes.changesFlags |= FplChange::EQUIPMENT;
        chg.changes.equipment = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_EQUIPMENT).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER))
    {
        chg.changes.changesFlags |= FplChange::SPEED;
        chg.changes.speed = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL))
    {
        chg.changes.changesFlags |= FplChange::FLIGHT_LEVEL;
        chg.changes.flightLevel = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_REQUESTED_CRUISING_LEVEL).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_ROUTE))
    {
        chg.changes.changesFlags |= FplChange::ROUTE;
        chg.changes.route = Transliteration::routeToVersion(changesMap.value(AFTN_MESSAGE_ROUTE).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_OTHER_INFO))
    {
        chg.changes.changesFlags |= FplChange::OTHER;
        chg.changes.other = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_OTHER_INFO).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD))
    {
        chg.changes.changesFlags |= FplChange::ALT1;
        chg.changes.alt1 = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_DISPERSAL_FIELD_2))
    {
        chg.changes.changesFlags |= FplChange::ALT2;
        chg.changes.alt2 = Transliteration::toVersion(changesMap.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString(), chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_OTHER_ALTERNATIVE_AERODROMES) && aftnParameters->nonstandardAlternativeAerodromesEnabled())
    {
        chg.changes.changesFlags |= FplChange::OTHER_ALTERNATIVE;
        QStringList alt = map.value(AFTN_MESSAGE_OTHER_ALTERNATIVE_AERODROMES).toStringList();
        foreach (const QString &a, alt)
            chg.changes.otherAlternative << Transliteration::toVersion(a, chg.version);
    }
    if (changesMap.contains(AFTN_MESSAGE_OTHER_BOARD_NUMBER))
    {
        chg.changes.changesFlags |= FplChange::BOARD_NUMBER;
        chg.changes.boardNumber = changesMap.value(AFTN_MESSAGE_OTHER_BOARD_NUMBER).toString();
    }

    return new ChgMessage(chg);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
