#include "ApzCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ApzMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ApzCreator::ApzCreator()
{
    addType(TelegramType::APZ);
    addType(TelegramType::APZ1);
}

AftnMessage * ApzCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ApzStruct apz;
    apz.version = aftnParameters->supportedTypes()->version(type);

    apz.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), apz.version);
    apz.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), apz.version);
    apz.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        apz.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    apz.airport = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO_AIRPORT).toString(), apz.version);
    apz.validFrom = map.value(AFTN_MESSAGE_OTHER_INFO_START_TIME).toDateTime();
    apz.validTill = map.value(AFTN_MESSAGE_OTHER_INFO_FINISH_TIME).toDateTime();
    apz.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), apz.version);

    return new ApzMessage(apz);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
