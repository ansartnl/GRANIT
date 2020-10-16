#include "RrpMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RrpMessage::RrpMessage()
{
}

RrpMessage::RrpMessage(const RrpStruct &s) :
    AtfFplMessage(s), mRrp(s)
{
    setVersion(s.version);
}

RrpMessage::~RrpMessage()
{
}

QString RrpMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRrp.acid;

    ret += "-";
    ret += mRrp.adep;

    ret += "-";
    ret += mRrp.ades;

    ret += "-";
    ret += mRrp.other;

    if (mRrp.changes.changesFlags.testFlag(FplChange::CHANGE_ACID)) {
        ret += "-";
        ret += "7/" + mRrp.changes.acid;
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::FLIGHT_RULES)
            || mRrp.changes.changesFlags.testFlag(FplChange::TYPE_OF_FLIGHT)) {
        ret += "-";
        ret += "8/" + mRrp.changes.flightRules + mRrp.changes.typeOfFlight;
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::NUMBER_OF_AIRCRAFTS)
            || mRrp.changes.changesFlags.testFlag(FplChange::AIRCRAFT_TYPE)
            || mRrp.changes.changesFlags.testFlag(FplChange::TURBULENCE)) {
        ret += "-";
        if (mRrp.changes.numberOfAircrafts == 1)
            ret += QString("9/%1/%2").arg(mRrp.changes.aircraftType).arg(mRrp.changes.turbulence);
        else
            ret += QString("9/%1%2/%3").arg(mRrp.changes.numberOfAircrafts).arg(mRrp.changes.aircraftType)
                    .arg(mRrp.changes.turbulence);
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::EQUIPMENT)) {
        ret += "-";
        ret += "10/" + mRrp.changes.equipment;
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::CHANGE_ADEP)
            || mRrp.changes.changesFlags.testFlag(FplChange::DEPARTURE_TIME)) {
        ret += "-";
        ret += "13/" + mRrp.changes.adep + mRrp.changes.departureTime.toString("hhmm");
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::SPEED)
            || mRrp.changes.changesFlags.testFlag(FplChange::FLIGHT_LEVEL)
            || mRrp.changes.changesFlags.testFlag(FplChange::ROUTE)) {
        ret += "-";
        ret += "15/" + mRrp.changes.speed + mRrp.changes.flightLevel + " " + mRrp.changes.route;
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::CHANGE_ADES)
            || mRrp.changes.changesFlags.testFlag(FplChange::EET)
            || mRrp.changes.changesFlags.testFlag(FplChange::ALT1)
            || mRrp.changes.changesFlags.testFlag(FplChange::ALT2)) {
        ret += "-";
        ret += "16/" + mRrp.changes.ades + removeColonFromTime(mRrp.changes.eet);
        if (!mRrp.changes.alt1.isEmpty())
            ret += " " + mRrp.changes.alt1;
        if (!mRrp.changes.alt2.isEmpty())
            ret += " " + mRrp.changes.alt2;
        if (!mRrp.changes.otherAlternative.isEmpty())
            ret += " " + mRrp.changes.otherAlternative.join(" ");
    }

    if (mRrp.changes.changesFlags.testFlag(FplChange::OTHER)) {
        ret += "-";
        ret += "18/" + mRrp.changes.other;
    }

    ret += ")";

    return ret;
}

void RrpMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mRrp.acid = Transliteration::acidToVersion(mRrp.acid, v);
    mRrp.adep = Transliteration::toVersion(mRrp.adep, v);
    mRrp.ades = Transliteration::toVersion(mRrp.ades, v);
    mRrp.other = Transliteration::toVersion(mRrp.other, v);
}

AtfStruct & RrpMessage::atf()
{
    return (AtfStruct &)mRrp;
}

const AtfStruct & RrpMessage::atfConst() const
{
    return (const AtfStruct &)mRrp;
}

AtsStruct RrpMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRrp.acid;
    ret.adep = mRrp.adep;
    ret.ades = mRrp.ades;
    ret.dof = mRrp.dof;

    ret.version = mRrp.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
