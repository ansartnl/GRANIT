#include "SplCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "SplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SplCreator::SplCreator()
{
    addType(TelegramType::SPL);
}

AftnMessage * SplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    SplStruct spl;
    spl.version = aftnParameters->supportedTypes()->version(type);

    spl.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), spl.version);
    if (spl.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    spl.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), spl.version);
    if (spl.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    spl.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    spl.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), spl.version);
    if (spl.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    spl.eet = map.value(AFTN_MESSAGE_ELAPSED_TIME).toString();

    spl.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), spl.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        spl.departureTime.setDate(dof);
    }

    QRegExp eetRegExp("^(\\d{2}):?(\\d{2})$");
    if (eetRegExp.exactMatch(spl.eet)) {
        spl.arrivalTime = spl.departureTime.addSecs(eetRegExp.cap(1).toUInt() * 3600
                                                    + eetRegExp.cap(2).toUInt() * 60);
    }

    spl.additional = Transliteration::toVersion(map.value(AFTN_MESSAGE_ADDITIONAL_INFO).toString(), spl.version);

    return new SplMessage(spl);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
