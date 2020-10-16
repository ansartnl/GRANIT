#include "ApwCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ApwMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ApwCreator::ApwCreator()
{
    addType(TelegramType::APW);
    addType(TelegramType::APW1);
}

AftnMessage * ApwCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ApwStruct apw;
    apw.version = aftnParameters->supportedTypes()->version(type);

    apw.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), apw.version);
    apw.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), apw.version);
    apw.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        apw.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    apw.airport = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO_AIRPORT).toString(), apw.version);
    apw.validFrom = map.value(AFTN_MESSAGE_OTHER_INFO_START_TIME).toDateTime();
    apw.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), apw.version);

    return new ApwMessage(apw);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
