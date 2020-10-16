#include "ScnCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ScnMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ScnCreator::ScnCreator()
{
    addType(TelegramType::SCN);
    addType(TelegramType::SCN1);
}

AftnMessage * ScnCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ScnStruct scn;
    scn.version = aftnParameters->supportedTypes()->version(type);

    scn.type = map.value(AFTN_MESSAGE_TYPE).toString();
    scn.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    scn.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        scn.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    scn.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), scn.version);

    return new ScnMessage(scn);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
