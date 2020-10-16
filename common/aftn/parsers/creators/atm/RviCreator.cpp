#include "RviCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RviMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RviCreator::RviCreator()
{
    addType(TelegramType::RVI);
    addType(TelegramType::RVI1);
}

AftnMessage * RviCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RviStruct rvi;
    rvi.version = aftnParameters->supportedTypes()->version(type);

    rvi.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), rvi.version);
    rvi.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), rvi.version);
    rvi.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rvi.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    rvi.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rvi.version);

    return new RviMessage(rvi);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
