#include "RcfCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RcfMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RcfCreator::RcfCreator()
{
    addType(TelegramType::RCF);
    addType(TelegramType::RCF1);
}

AftnMessage * RcfCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RcfStruct rcf;
    rcf.version = aftnParameters->supportedTypes()->version(type);

    rcf.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rcf.version);
    if (rcf.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    rcf.lastContactTime = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME).toString(), rcf.version);
    rcf.lastContactFrequency = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_LAST_FREQUENCY).toString(), rcf.version);
    rcf.lastReportedPosition = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_LAST_LOCATION).toString(), rcf.version);
    rcf.lastReportedPositionTime = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME_LOCATION).toString(), rcf.version);
    rcf.remainingEquipment = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_POSSIBLE_LINK).toString(), rcf.version);
    rcf.remarks = Transliteration::rfiToVersion(
                map.value(AFTN_MESSAGE_REFUSAL_RADIOLINK_REMARKS).toString(), rcf.version);

    return new RcfMessage(rcf);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
