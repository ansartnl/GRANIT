#include "AnmCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AnmMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AnmCreator::AnmCreator()
{
    addType(TelegramType::ANM);
    addType(TelegramType::ANM1);
}

AftnMessage * AnmCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AnmStruct anm;
    anm.version = aftnParameters->supportedTypes()->version(type);

    anm.type = map.value(AFTN_MESSAGE_TYPE).toString();
    anm.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    anm.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        anm.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    anm.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), anm.version);

    return new AnmMessage(anm);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
