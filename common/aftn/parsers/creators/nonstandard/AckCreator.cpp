#include "AckCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AckMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AckCreator::AckCreator()
{
    addType(TelegramType::ACK);
    addType(TelegramType::ACK1);
    addType(TelegramType::ACK, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::ACK1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * AckCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    AckStruct ack;

    ack.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), ack.version);
    if (ack.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    ack.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), ack.version);
    if (ack.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    ack.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), ack.version);
    if (ack.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    ack.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), ack.version);
    ack.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        ack.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    ack.version = ParseVersion::textVersion(ack.adep);

    return new AckMessage(ack);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
