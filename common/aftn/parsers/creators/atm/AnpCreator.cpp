#include "AnpCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AnpMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AnpCreator::AnpCreator()
{
    addType(TelegramType::ANP);
    addType(TelegramType::ANP1);
}

AftnMessage * AnpCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AnpStruct anp;
    anp.version = aftnParameters->supportedTypes()->version(type);

    anp.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), anp.version);
    anp.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), anp.version);
    anp.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), anp.version);
    anp.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        anp.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    anp.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), anp.version);
    anp.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    anp.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString()
                                                              , anp.version);

    return new AnpMessage(anp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
