#include "RtcCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RtcMessage.h"

#include "AtsNonStandardMessagesParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RtcCreator::RtcCreator()
{
    addType(TelegramType::RTC);
}

AftnMessage * RtcCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    RtcStruct rtc;

    rtc.time = map.value(RTC_RES_DATE_TIME).toDateTime();

    return new RtcMessage(rtc);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
