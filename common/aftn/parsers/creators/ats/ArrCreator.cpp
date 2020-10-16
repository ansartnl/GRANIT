#include "ArrCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "ArrMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ArrCreator::ArrCreator()
{
    addType(TelegramType::ARR);
    addType(TelegramType::ARR1);
    addType(TelegramType::ARR, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::ARR1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * ArrCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    ArrStruct arr;
    arr.version = aftnParameters->supportedTypes()->version(type);

    arr.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), arr.version);
    if (arr.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    arr.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), arr.version);
    if (arr.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    arr.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();
    arr.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), arr.version);
    arr.arrivalAirport = Transliteration::toVersion(map.value(AFTN_MESSAGE_ARRIVAL_AERODROME).toString(), arr.version);
    arr.arrivalTime = map.value(AFTN_MESSAGE_ARRIVAL_TIME).toDateTime();
    arr.arrivalAirportName = Transliteration::toVersion(map.value(AFTN_MESSAGE_ARRIVAL_AERODROME_NAME).toString(), arr.version);

    if (arr.ades == arr.arrivalAirport)
        arr.ades.clear();

    arr.dof = QDateTime::currentDateTimeUtc().date();
#if defined(BAN_BUILD)
    arr.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), arr.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        arr.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        arr.departureTime.setDate(arr.dof);
    }
#endif
    arr.boardNumber = map.value(AFTN_MESSAGE_OTHER_BOARD_NUMBER).toString();

    return new ArrMessage(arr);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
