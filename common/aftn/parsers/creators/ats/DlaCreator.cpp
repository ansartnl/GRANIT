#include "DlaCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "DlaMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

DlaCreator::DlaCreator()
{
    addType(TelegramType::DLA);
    addType(TelegramType::DLA1);
    addType(TelegramType::DLA, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::DLA1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * DlaCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    DlaStruct dla;
    dla.version = aftnParameters->supportedTypes()->version(type);

    dla.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), dla.version);
    if (dla.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    dla.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), dla.version);
    if (dla.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    dla.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), dla.version);
    if (dla.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    dla.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    dla.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), dla.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        dla.departureTime.setDate(dof);
    }
    dla.boardNumber = map.value(AFTN_MESSAGE_OTHER_BOARD_NUMBER).toString();

    return new DlaMessage(dla);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
