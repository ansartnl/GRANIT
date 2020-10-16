#include "ResCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ResMessage.h"

#include "AtsNonStandardMessagesParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ResCreator::ResCreator()
{
    addType(TelegramType::RES);
}

AftnMessage * ResCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    ResStruct res;

    res.time = map.value(RTC_RES_DATE_TIME).toDateTime();

    return new ResMessage(res);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
