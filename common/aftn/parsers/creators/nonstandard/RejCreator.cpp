#include "RejCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RejMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RejCreator::RejCreator()
{
    addType(TelegramType::REJ);
    addType(TelegramType::REJ1);
    addType(TelegramType::REJ, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::REJ1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * RejCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    RejStruct rej;

    rej.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rej.version);
    if (rej.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    rej.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), rej.version);
    if (rej.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    rej.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), rej.version);
    if (rej.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    rej.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rej.version);
    rej.departureTime = QDateTime::currentDateTimeUtc();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rej.departureTime.setDate(map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate());

    rej.version = ParseVersion::textVersion(rej.adep);

    return new RejMessage(rej);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
