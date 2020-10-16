#include "RvmCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RvmMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RvmCreator::RvmCreator()
{
    addType(TelegramType::RVM);
    addType(TelegramType::RVM1);
}

AftnMessage * RvmCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    RvmStruct rvm;
    rvm.version = aftnParameters->supportedTypes()->version(type);

    rvm.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), rvm.version);
    rvm.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), rvm.version);
    rvm.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        rvm.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    rvm.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), rvm.version);

    return new RvmMessage(rvm);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
