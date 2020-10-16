#include "AlrMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AlrMessage::AlrMessage()
{
}

AlrMessage::AlrMessage(const AlrStruct &s) :
    mAlr(s)
{
    setVersion(s.version);
}

AlrMessage::~AlrMessage()
{
}

QString AlrMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAlr.emergencyState;
    ret += "/";
    ret += mAlr.emergencySender;
    ret += "/";
    ret += mAlr.emergencyMessage;
    ret += "\r\n";

    ret += "-";
    ret += mAlr.acid;

    ret += "-";
    ret += mAlr.flightRules + mAlr.typeOfFlight;
    ret += "\r\n";

    ret += "-";
    if (mAlr.numberOfAircrafts == 1)
        ret += QString("%1/%2").arg(mAlr.aircraftType).arg(mAlr.turbulence);
    else
        ret += QString("%1%2/%3").arg(mAlr.numberOfAircrafts).arg(mAlr.aircraftType).arg(mAlr.turbulence);

    ret += "-";
    ret += mAlr.equipment;
    ret += "\r\n";

    ret += "-";
    ret += mAlr.adep + mAlr.departureTime.toString("hhmm");
    ret += "\r\n";

    ret += "-";
    ret += mAlr.speed + mAlr.flightLevel + " " + mAlr.route;
    ret += "\r\n";

    ret += "-";
    ret += mAlr.ades + ((mAlr.eet.size() < 5) ? mAlr.eet : (mAlr.eet.mid(0, 2) + mAlr.eet.mid(3, 2)));
    if (!mAlr.alt1.isEmpty())
        ret += " " + mAlr.alt1;
    if (!mAlr.alt2.isEmpty())
        ret += " " + mAlr.alt2;
    ret += "\r\n";

    ret += "-";
    ret += mAlr.other;

    ret += "\r\n";
    ret += "-";
    ret += mAlr.additionalInfo;

    ret += "\r\n";
    ret += "-";
    ret += mAlr.rescueInfo;
    ret += ")";

    return ret;
}

void AlrMessage::setVersion(ParseVersion::Version v)
{
    mAlr.type = Transliteration::toVersion(TelegramType::ALR, v);

    AtmMessage::setVersion(v);

    mAlr.emergencyState = Transliteration::toVersion(mAlr.emergencyState, v);
    mAlr.emergencySender = Transliteration::toVersion(mAlr.emergencySender, v);
    mAlr.emergencyMessage = Transliteration::toVersion(mAlr.emergencyMessage, v);

    mAlr.acid = Transliteration::acidToVersion(mAlr.acid, v);
    mAlr.flightRules = Transliteration::toVersion(mAlr.flightRules, v);
    mAlr.typeOfFlight = Transliteration::toVersion(mAlr.typeOfFlight, v);

    mAlr.aircraftType = Transliteration::aircraftTypeToVersion(mAlr.aircraftType, v);
    mAlr.turbulence = Transliteration::toVersion(mAlr.turbulence, v);

    mAlr.equipment = Transliteration::toVersion(mAlr.equipment, v);

    mAlr.adep = Transliteration::toVersion(mAlr.adep, v);

    mAlr.speed = Transliteration::toVersion(mAlr.speed, v);
    mAlr.flightLevel = Transliteration::toVersion(mAlr.flightLevel, v);
    mAlr.route = Transliteration::routeToVersion(mAlr.route, v);

    mAlr.ades = Transliteration::toVersion(mAlr.ades, v);
    mAlr.alt1 = Transliteration::toVersion(mAlr.alt1, v);
    mAlr.alt2 = Transliteration::toVersion(mAlr.alt2, v);

    mAlr.other = Transliteration::toVersion(mAlr.other, v);

    mAlr.additionalInfo = Transliteration::toVersion(mAlr.additionalInfo, v);
    mAlr.rescueInfo = Transliteration::toVersion(mAlr.rescueInfo, v);

    mAlr.version = v;

    mType = Transliteration::toVersion(TelegramType::ALR, v);
}

AtmStruct & AlrMessage::atm()
{
    return (AtmStruct &)mAlr;
}

const AtmStruct & AlrMessage::atmConst() const
{
    return (const AtmStruct &)mAlr;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
