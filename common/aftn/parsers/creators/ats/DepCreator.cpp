#include "DepCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "DepMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

DepCreator::DepCreator()
{
    addType(TelegramType::DEP);
    addType(TelegramType::DEP1);
    addType(TelegramType::DEP, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::DEP1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * DepCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    DepStruct dep;
    dep.version = aftnParameters->supportedTypes()->version(type);

    dep.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), dep.version);
    if (dep.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    dep.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), dep.version);
    if (dep.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    dep.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    dep.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), dep.version);
    if (dep.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    dep.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), dep.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        dep.departureTime.setDate(dof);
    }
    dep.boardNumber = map.value(AFTN_MESSAGE_OTHER_BOARD_NUMBER).toString();

    return new DepMessage(dep);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
