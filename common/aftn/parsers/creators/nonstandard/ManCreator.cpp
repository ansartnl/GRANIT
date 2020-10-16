#include "ManCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ManMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ManCreator::ManCreator()
{
    addType(TelegramType::MAN);
    addType(TelegramType::MAN, TypeKey::kAdexpDisambiguation);
}

AftnMessage * ManCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ManStruct man;
    man.version = aftnParameters->supportedTypes()->version(type);

    man.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), man.version);
    if (man.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    man.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), man.version);
    if (man.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    man.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), man.version);
    if (man.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    man.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), man.version);
    man.departureTime = QDateTime::currentDateTimeUtc();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        man.departureTime.setDate(map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate());

    return new ManMessage(man);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
