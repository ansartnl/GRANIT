#include "AlrCreatorShort.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AlrMessageShort.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AlrCreatorShort::AlrCreatorShort()
{
    addType(TelegramType::ALR, TypeKey::kNonstandardDisambiguation);
    addType(TelegramType::ALR1, TypeKey::kNonstandardDisambiguation);
}

AftnMessage * AlrCreatorShort::pCreate(const QString &type, const QVariantMap &map) const
{
    AlrStructShort alr;
    alr.version = aftnParameters->supportedTypes()->version(type);

    alr.type = Transliteration::toVersion(map.value(AFTN_MESSAGE_TYPE).toString(), alr.version);
    alr.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), alr.version);
    alr.text = Transliteration::toVersion(map.value(AFTN_SOURCE_MESSAGE).toString(), alr.version);
    alr.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        alr.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    alr.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), alr.version);
    alr.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    alr.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString()
                                                              , alr.version);

    return new AlrMessageShort(alr);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
