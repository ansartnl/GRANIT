#include "RqsCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RqsMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqsCreator::RqsCreator()
{
    addType(TelegramType::RQS);
}

AftnMessage * RqsCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RqsStruct rqs;
    rqs.version = aftnParameters->supportedTypes()->version(type);

    rqs.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rqs.version);
    if (rqs.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    rqs.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), rqs.version);
    if (rqs.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    rqs.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), rqs.version);
    if (rqs.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    rqs.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rqs.version);
    rqs.dof = QDateTime::currentDateTimeUtc().date();

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rqs.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new RqsMessage(rqs);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
