#include "FlaCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "FlaMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

FlaCreator::FlaCreator()
{
    addType(TelegramType::FLA);
    addType(TelegramType::FLA1);
}

AftnMessage * FlaCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    FlaStruct fla;
    fla.version = aftnParameters->supportedTypes()->version(type);

    fla.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), fla.version);
    if (fla.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));
    fla.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), fla.version);
    if (fla.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    fla.arrivalAirport = Transliteration::toVersion(map.value(AFTN_MESSAGE_ARRIVAL_AERODROME).toString(), fla.version);
    fla.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), fla.version);
    fla.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        fla.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new FlaMessage(fla);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
