#include "CnlCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "CnlMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CnlCreator::CnlCreator()
{
    addType(TelegramType::CNL);
    addType(TelegramType::CNL1);
    addType(TelegramType::CNL, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::CNL1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * CnlCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    CnlStruct cnl;
    cnl.version = aftnParameters->supportedTypes()->version(type);

    cnl.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), cnl.version);
    if (cnl.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    cnl.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), cnl.version);
    if (cnl.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    cnl.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    cnl.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), cnl.version);
    if (cnl.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    cnl.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), cnl.version);
    cnl.dof = QDateTime::currentDateTimeUtc().date();

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        cnl.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new CnlMessage(cnl);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
