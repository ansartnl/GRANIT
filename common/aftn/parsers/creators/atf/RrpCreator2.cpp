#include "RrpCreator2.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RrpMessage2.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RrpCreator2::RrpCreator2()
{
    addType(TelegramType::RRP);
    addType(TelegramType::RRP1);
}

AftnMessage * RrpCreator2::pCreate(const QString &type, const QVariantMap &map) const
{
    RrpStruct2 rrp;
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
    rrp.route = Transliteration::toVersion(map.value(AFTN_MESSAGE_ROUTE).toString(), rrp.version);

    return new RrpMessage2(rrp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
