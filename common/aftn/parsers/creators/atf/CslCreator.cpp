#include "CslCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "CslMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CslCreator::CslCreator()
{
    addType(TelegramType::CSL);
    addType(TelegramType::CSL1);
}

AftnMessage * CslCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AtfFplStruct atf;
    atf.version = aftnParameters->supportedTypes()->version(type);

    atf.type = map.value(AFTN_MESSAGE_TYPE).toString();
    atf.text = map.value(AFTN_SOURCE_MESSAGE).toString();
    atf.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        atf.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    atf.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), atf.version);
    if (atf.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));
    atf.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), atf.version);
    if (atf.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    atf.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(),atf.version);
    if (atf.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));
    atf.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), atf.version);

    return new CslMessage(atf);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
