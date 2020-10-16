#include "RplCreator.h"

#include "TelegramTypes.h"

#include "RplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RplCreator::RplCreator()
{
    addType(TelegramType::RPL);
    addType(TelegramType::RPL1);
}

AftnMessage * RplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RplStruct rpl(createFplStruct(type, map));

    rpl.validFrom = map.value(AFTN_MESSAGE_RPL_VALID_FROM).toDate();
    rpl.validTill = map.value(AFTN_MESSAGE_RPL_VALID_TILL).toDate();

    if (map.contains(AFTN_MESSAGE_RPL_MONDAY))
        rpl.days |= RplStruct::MONDAY;
    if (map.contains(AFTN_MESSAGE_RPL_TUESDAY))
        rpl.days |= RplStruct::TUESDAY;
    if (map.contains(AFTN_MESSAGE_RPL_WEDNESDAY))
        rpl.days |= RplStruct::WEDNESDAY;
    if (map.contains(AFTN_MESSAGE_RPL_THURSDAY))
        rpl.days |= RplStruct::THURSDAY;
    if (map.contains(AFTN_MESSAGE_RPL_FRIDAY))
        rpl.days |= RplStruct::FRIDAY;
    if (map.contains(AFTN_MESSAGE_RPL_SATURDAY))
        rpl.days |= RplStruct::SATURDAY;
    if (map.contains(AFTN_MESSAGE_RPL_SUNDAY))
        rpl.days |= RplStruct::SUNDAY;

    return new RplMessage(rpl);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
