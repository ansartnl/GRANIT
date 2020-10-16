#include "CnlRplCreator.h"

#include "TelegramTypes.h"

#include "CnlRplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CnlRplCreator::CnlRplCreator()
{
    addType(TelegramType::CNL_RPL1);
}

AftnMessage * CnlRplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    CnlRplStruct cnlRpl(createFplStruct(type, map));

    cnlRpl.validFrom = map.value(AFTN_MESSAGE_RPL_VALID_FROM).toDate();
    cnlRpl.validTill = map.value(AFTN_MESSAGE_RPL_VALID_TILL).toDate();

    if (map.contains(AFTN_MESSAGE_RPL_MONDAY))
        cnlRpl.days |= CnlRplStruct::MONDAY;
    if (map.contains(AFTN_MESSAGE_RPL_TUESDAY))
        cnlRpl.days |= CnlRplStruct::TUESDAY;
    if (map.contains(AFTN_MESSAGE_RPL_WEDNESDAY))
        cnlRpl.days |= CnlRplStruct::WEDNESDAY;
    if (map.contains(AFTN_MESSAGE_RPL_THURSDAY))
        cnlRpl.days |= CnlRplStruct::THURSDAY;
    if (map.contains(AFTN_MESSAGE_RPL_FRIDAY))
        cnlRpl.days |= CnlRplStruct::FRIDAY;
    if (map.contains(AFTN_MESSAGE_RPL_SATURDAY))
        cnlRpl.days |= CnlRplStruct::SATURDAY;
    if (map.contains(AFTN_MESSAGE_RPL_SUNDAY))
        cnlRpl.days |= CnlRplStruct::SUNDAY;

    return new CnlRplMessage(cnlRpl);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
