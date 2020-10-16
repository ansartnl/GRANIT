#include "AimCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AimMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AimCreator::AimCreator()
{
    addType(TelegramType::AIM);
}

AftnMessage * AimCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AimStruct aim;
    aim.version = aftnParameters->supportedTypes()->version(type);

    aim.type = map.value(AFTN_MESSAGE_TYPE).toString();
    aim.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    aim.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        aim.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    aim.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), aim.version);

    return new AimMessage(aim);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
