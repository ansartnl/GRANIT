#include "PlrCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "PlrMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

PlrCreator::PlrCreator()
{
    addType(TelegramType::PLR);
}

AftnMessage * PlrCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    PlrStruct plr;
    plr.version = aftnParameters->supportedTypes()->version(type);

    plr.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), plr.version);
    if (plr.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    plr.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), plr.version);
    if (plr.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));

    plr.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DESTINATION_AERODROME).toString(), plr.version);
    if (plr.ades.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADES));

    plr.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();

    plr.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), plr.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        plr.departureTime.setDate(dof);
    }

    return new PlrMessage(plr);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
