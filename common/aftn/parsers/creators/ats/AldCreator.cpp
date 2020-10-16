#include "AldCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AldMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AldCreator::AldCreator()
{
    addType(TelegramType::ALD);
    addType(TelegramType::ALD1);
}

AftnMessage * AldCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AldStruct ald;
    ald.version = aftnParameters->supportedTypes()->version(type);

    ald.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), ald.version);
    if (ald.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    ald.numberOfAircrafts = map.value(AFTN_MESSAGE_AIRCRAFT_COUNT).toInt();
    ald.aircraftType = Transliteration::aircraftTypeToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_TYPE).toString(), ald.version);
    if (ald.aircraftType.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidAircraftType));
    ald.turbulence = Transliteration::toVersion(map.value(AFTN_MESSAGE_TURBULENCE).toString(), ald.version);

    ald.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), ald.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
    {
        ald.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
    } else
    {
        ald.dof = QDateTime::currentDateTimeUtc().date();
    }

    return new AldMessage(ald);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
