#include "UlsCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "UlsMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

UlsCreator::UlsCreator()
{
    addType(TelegramType::ULS);
    addType(TelegramType::ULS1);
}

AftnMessage * UlsCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    UlsStruct uls;
    uls.version = aftnParameters->supportedTypes()->version(type);

    uls.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), uls.version);
    if (uls.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));
#ifdef PATCH1
    if (map.contains(AFTN_MESSAGE_AIRCRAFT_COUNT)) {
        uls.hasField9 = true;
        uls.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
        uls.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString(), uls.version);
        if (uls.aircraftType.isEmpty())
            addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidAircraftType));
        uls.turbulence = Transliteration::toVersion(map.value(AFTN_MESSAGE_TURBULENCE).toString(), uls.version);
    } else {
        uls.hasField9 = false;
        uls.numberOfAircrafts = 1;
    }
#endif
    uls.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), uls.version);
    if (uls.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    uls.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), uls.version);
    if (uls.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    uls.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), uls.version);
    uls.letter = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO_STS).toString(), uls.version);
    uls.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        uls.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new UlsMessage(uls);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
