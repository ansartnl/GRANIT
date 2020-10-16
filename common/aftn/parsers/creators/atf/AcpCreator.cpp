#include "AcpCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "AcpMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AcpCreator::AcpCreator()
{
    addType(TelegramType::ACP);
    addType(TelegramType::ACP1);
}

AftnMessage * AcpCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    AcpStruct acp;
    acp.version = aftnParameters->supportedTypes()->version(type);

    acp.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), acp.version);
    if (acp.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    acp.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), acp.version);
    if (acp.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    acp.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), acp.version);
    if (acp.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    return new AcpMessage(acp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
