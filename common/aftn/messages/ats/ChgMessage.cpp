#include "ChgMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ChgMessage::ChgMessage()
{
}

ChgMessage::ChgMessage(const ChgStruct &s) :
    mChg(s)
{
    setVersion(s.version);
}

ChgMessage::~ChgMessage()
{
}

QString ChgMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mChg.acid;

    ret += "-";
    ret += mChg.adep;

    ret += "-";
    ret += mChg.ades;

    if (!mChg.other.isEmpty()) {
        ret += "-";
        ret += mChg.other;
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::CHANGE_ACID)) {
        ret += "-";
        ret += "7/" + mChg.changes.acid;
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::FLIGHT_RULES)
            || mChg.changes.changesFlags.testFlag(FplChange::TYPE_OF_FLIGHT)) {
        ret += "-";
        ret += "8/" + mChg.changes.flightRules + mChg.changes.typeOfFlight;
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::NUMBER_OF_AIRCRAFTS)
            || mChg.changes.changesFlags.testFlag(FplChange::AIRCRAFT_TYPE)
            || mChg.changes.changesFlags.testFlag(FplChange::TURBULENCE)) {
        ret += "-";
        if (mChg.changes.numberOfAircrafts == 1)
            ret += QString("9/%1/%2").arg(mChg.changes.aircraftType).arg(mChg.changes.turbulence);
        else
            ret += QString("9/%1%2/%3").arg(mChg.changes.numberOfAircrafts).arg(mChg.changes.aircraftType)
                    .arg(mChg.changes.turbulence);
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::EQUIPMENT)) {
        ret += "-";
        ret += "10/" + mChg.changes.equipment;
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::CHANGE_ADEP)
            || mChg.changes.changesFlags.testFlag(FplChange::DEPARTURE_TIME)) {
        ret += "-";
        ret += "13/" + mChg.changes.adep + mChg.changes.departureTime.toString("hhmm");
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::SPEED)
            || mChg.changes.changesFlags.testFlag(FplChange::FLIGHT_LEVEL)
            || mChg.changes.changesFlags.testFlag(FplChange::ROUTE)) {
        ret += "-";
        ret += "15/" + mChg.changes.speed + mChg.changes.flightLevel + " " + mChg.changes.route;
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::CHANGE_ADES)
            || mChg.changes.changesFlags.testFlag(FplChange::EET)
            || mChg.changes.changesFlags.testFlag(FplChange::ALT1)
            || mChg.changes.changesFlags.testFlag(FplChange::ALT2)) {
        ret += "-";
        ret += "16/" + mChg.changes.ades + removeColonFromTime(mChg.changes.eet);
        if (!mChg.changes.alt1.isEmpty())
            ret += " " + mChg.changes.alt1;
        if (!mChg.changes.alt2.isEmpty())
            ret += " " + mChg.changes.alt2;
        if (!mChg.changes.otherAlternative.isEmpty())
            ret += " " + mChg.changes.otherAlternative.join(" ");
    }

    if (mChg.changes.changesFlags.testFlag(FplChange::OTHER)) {
        ret += "-";
        ret += "18/" + mChg.changes.other;
    }

    ret += ")";

    return ret;
}

QString ChgMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IFPL";
    // 7 field
    ret += " -ARCID " + mChg.acid;
    // 13 field
    ret += " -ADEP " + mChg.adep;
    // 16 field
    ret += " -ADES " + mChg.ades;

    // CHANGES

    // 8 field

    if (mChg.changes.changesFlags.testFlag(FplChange::FLIGHT_RULES))
        ret += " -FLTRUL " + mChg.changes.flightRules;
    if (mChg.changes.changesFlags.testFlag(FplChange::TYPE_OF_FLIGHT))
        ret += " -FLTTYP " + mChg.changes.typeOfFlight;
    // 9 field
    if (mChg.changes.changesFlags.testFlag(FplChange::NUMBER_OF_AIRCRAFTS))
//    if (mChg.numberOfAircrafts > 1)
        ret += " -NBARC " + mChg.changes.numberOfAircrafts;
    if (mChg.changes.changesFlags.testFlag(FplChange::AIRCRAFT_TYPE))
        ret += " -ARCTYP " + mChg.changes.aircraftType;
    if (mChg.changes.changesFlags.testFlag(FplChange::TURBULENCE))
        ret += " -WKTRC " + mChg.changes.turbulence;
    // 10 field
    if (mChg.changes.changesFlags.testFlag(FplChange::EQUIPMENT)) {
        QRegExp rexp("(.+)/(.+)");
        if (rexp.exactMatch(mChg.changes.equipment)) {
            ret += " -CEQPT " + rexp.cap(1);
            ret += " -SEQPT " + rexp.cap(2);
        } else {
            ret += " -CEQPT " + mChg.changes.equipment;
        }
    }
    // 13 field
    if (mChg.changes.changesFlags.testFlag(FplChange::DEPARTURE_TIME))
        ret += " -EOBT " + mChg.changes.departureTime.toString("hhmm");
    // 15 field
    if (mChg.changes.changesFlags.testFlag(FplChange::SPEED))
        ret += " -SPEED " + mChg.changes.speed;
    if (mChg.changes.changesFlags.testFlag(FplChange::FLIGHT_LEVEL))
        ret += " -RFL " + mChg.changes.flightLevel;
    if (mChg.changes.changesFlags.testFlag(FplChange::ROUTE))
        ret += " -ROUTE " + mChg.changes.speed + mChg.changes.flightLevel + " " + mChg.changes.route;
    // 16 field
    if (mChg.changes.changesFlags.testFlag(FplChange::DEPARTURE_TIME))
        ret += " -TTLEET " + mChg.changes.departureTime.toString("hhmm");
    if (mChg.changes.changesFlags.testFlag(FplChange::ALT1))
        ret += " -ALTRNT1 " + mChg.changes.alt1;
    if (mChg.changes.changesFlags.testFlag(FplChange::ALT2))
        ret += " -ALTRNT2 " + mChg.changes.alt2;
    // 18 field
    if (mChg.changes.changesFlags.testFlag(FplChange::OTHER))
        ret += otherToAdexp(mChg.changes.other);

    return ret;
}

void ChgMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mChg.acid = Transliteration::acidToVersion(mChg.acid, v);
    mChg.adep = Transliteration::toVersion(mChg.adep, v);
    mChg.ades = Transliteration::toVersion(mChg.ades, v);
    mChg.other = Transliteration::toVersion(mChg.other, v);

    mChg.changes.acid = Transliteration::acidToVersion(mChg.changes.acid, v);
    mChg.changes.flightRules = Transliteration::toVersion(mChg.changes.flightRules, v);
    mChg.changes.typeOfFlight = Transliteration::toVersion(mChg.changes.typeOfFlight, v);

    mChg.changes.aircraftType = Transliteration::aircraftTypeToVersion(mChg.changes.aircraftType, v);
    mChg.changes.turbulence = Transliteration::toVersion(mChg.changes.turbulence, v);

    mChg.changes.equipment = Transliteration::toVersion(mChg.changes.equipment, v);

    mChg.changes.adep = Transliteration::toVersion(mChg.changes.adep, v);

    mChg.changes.speed = Transliteration::toVersion(mChg.changes.speed, v);
    mChg.changes.flightLevel = Transliteration::toVersion(mChg.changes.flightLevel, v);
    mChg.changes.route = Transliteration::routeToVersion(mChg.changes.route, v);

    mChg.changes.ades = Transliteration::toVersion(mChg.changes.ades, v);
    mChg.changes.alt1 = Transliteration::toVersion(mChg.changes.alt1, v);
    mChg.changes.alt2 = Transliteration::toVersion(mChg.changes.alt2, v);
    QStringList alt = mChg.changes.otherAlternative;
    mChg.changes.otherAlternative.clear();
    foreach (const QString &a, alt)
        mChg.changes.otherAlternative << Transliteration::toVersion(a, v);

    mChg.changes.other = Transliteration::toVersion(mChg.changes.other, v);

    mChg.version = v;

    mType = Transliteration::toVersion(TelegramType::CHG, v);
}

FplChange ChgMessage::changes() const
{
    return mChg.changes;
}

AtsStruct ChgMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mChg.acid;
    ret.adep = mChg.adep;
    ret.ades = mChg.ades;
    ret.dof = mChg.dof;

    ret.version = mChg.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
