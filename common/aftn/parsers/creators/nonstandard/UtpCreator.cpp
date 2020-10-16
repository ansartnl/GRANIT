#include "UtpCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "UtpMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

UtpCreator::UtpCreator()
{
    addType(TelegramType::UTP1);
}

AftnMessage * UtpCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    UtpStruct utp;
    utp.version = aftnParameters->supportedTypes()->version(type);

    utp.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), utp.version);
    if (utp.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    utp.aircraft = map.value(AFTN_MESSAGE_AIRCRAFT).toStringList();
    utp.adep = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), utp.version);
    if (utp.adep.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidADEP));
    utp.departureTime = map.value(AFTN_MESSAGE_DEPARTURE_TIME).toDateTime();
    utp.endExploration = map.value(AFTN_MESSAGE_END_EXPLORATION).toDateTime();
    utp.beginFlights = map.value(AFTN_MESSAGE_BEGIN_FLIGHTS).toDateTime();
    utp.endFlights = map.value(AFTN_MESSAGE_END_FLIGHTS).toDateTime();

    utp.route = Transliteration::routeToVersion(map.value(AFTN_MESSAGE_ROUTE).toString(), utp.version);

    utp.ades = Transliteration::toVersion(map.value(AFTN_MESSAGE_DEPARTURE_AERODROME).toString(), utp.version);
    utp.alt1 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD).toString(), utp.version);
    utp.alt2 = Transliteration::toVersion(map.value(AFTN_MESSAGE_DISPERSAL_FIELD_2).toString(), utp.version);

    utp.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), utp.version);

    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF)) {
        const QDate dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();
        utp.departureTime.setDate(dof);
    }

    utp.iFplId = map.value(AFTN_MESSAGE_FPL_ID).toString();

    return new UtpMessage(utp);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
