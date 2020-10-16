#include "SltCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "SltMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SltCreator::SltCreator()
{
    addType(TelegramType::SLT);
    addType(TelegramType::SLT1);
}

AftnMessage * SltCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    SltStruct slt;
    slt.version = aftnParameters->supportedTypes()->version(type);

    slt.type = map.value(AFTN_MESSAGE_TYPE).toString();
    slt.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    slt.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        slt.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    slt.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), slt.version);
    if (slt.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));
    slt.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString()
                                          , slt.version);
    if (slt.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    slt.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();
    slt.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString()
                                          ,slt.version);
    if (slt.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    slt.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), slt.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        slt.departureTime.setDate(dof);
    }

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_ETA))
        slt.slotTime = QDateTime(slt.dof, map.value(AFTN_MESSAGE_OTHER_INFO_ETA).toTime(), Qt::UTC);

    return new SltMessage(slt);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
