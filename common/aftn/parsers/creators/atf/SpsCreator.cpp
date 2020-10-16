#include "SpsCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "SpsMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SpsCreator::SpsCreator()
{
    addType(TelegramType::SPS);
    addType(TelegramType::SPS1);
}

AftnMessage * SpsCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    SpsStruct sps;
    sps.version = aftnParameters->supportedTypes()->version(type);

    sps.type = map.value(AFTN_MESSAGE_TYPE).toString();
    sps.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    sps.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        sps.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    sps.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), sps.version);

    return new SpsMessage(sps);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
