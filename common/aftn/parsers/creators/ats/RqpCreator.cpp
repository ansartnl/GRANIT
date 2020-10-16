#include "RqpCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RqpMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqpCreator::RqpCreator()
{
    addType(TelegramType::RQP);
}

AftnMessage * RqpCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RqpStruct rqp;
    rqp.version = aftnParameters->supportedTypes()->version(type);

    rqp.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), rqp.version);
    if (rqp.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    rqp.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), rqp.version);
    if (rqp.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    rqp.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), rqp.version);
    if (rqp.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    rqp.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rqp.version);
    rqp.dof = QDateTime::currentDateTimeUtc().date();

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rqp.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new RqpMessage(rqp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
