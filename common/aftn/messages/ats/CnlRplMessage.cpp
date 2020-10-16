#include "CnlRplMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CnlRplMessage::CnlRplMessage()
{
}

CnlRplMessage::CnlRplMessage(const CnlRplStruct &s) :
    mRpl(s)
{
    setVersion(s.version);
}

CnlRplMessage::~CnlRplMessage()
{
}

QString CnlRplMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRpl.validFrom.toString("yyMMdd");

    ret += "-";
    ret += mRpl.validTill.toString("yyMMdd");

    ret += "-";
    ret += mRpl.days.testFlag(CnlRplStruct::MONDAY)    ? "1" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::TUESDAY)   ? "2" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::THURSDAY)  ? "3" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::WEDNESDAY) ? "4" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::FRIDAY)    ? "5" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::SATURDAY)  ? "6" : "0";
    ret += mRpl.days.testFlag(CnlRplStruct::SUNDAY)    ? "7" : "0";

    ret += "-";
    ret += mRpl.acid;

    ret += "-";
    ret += mRpl.flightRules + mRpl.typeOfFlight;

    ret += "-";
    if (mRpl.numberOfAircrafts == 1)
        ret += QString("%1/%2").arg(mRpl.aircraftType).arg(mRpl.turbulence);
    else
        ret += QString("%1%2/%3").arg(mRpl.numberOfAircrafts).arg(mRpl.aircraftType)
                .arg(mRpl.turbulence);

    ret += "-";
    ret += mRpl.equipment;

    ret += "-";
    ret += mRpl.adep + mRpl.departureTime.toString("hhmm");

    ret += "-";
    ret += mRpl.speed + mRpl.flightLevel + " " + mRpl.route;

    ret += "-";
    ret += mRpl.ades + removeColonFromTime(mRpl.eet);
    if (!mRpl.alt1.isEmpty())
        ret += " " + mRpl.alt1;
    if (!mRpl.alt2.isEmpty())
        ret += " " + mRpl.alt2;
    if (!mRpl.otherAlternative.isEmpty())
        ret += " " + mRpl.otherAlternative.join(" ");

    ret += "-";
    ret += mRpl.other;
    ret += ")";

    return ret;
}

void CnlRplMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mRpl.acid = Transliteration::acidToVersion(mRpl.acid, v);
    mRpl.flightRules = Transliteration::toVersion(mRpl.flightRules, v);
    mRpl.typeOfFlight = Transliteration::toVersion(mRpl.typeOfFlight, v);

    mRpl.aircraftType = Transliteration::aircraftTypeToVersion(mRpl.aircraftType, v);
    mRpl.turbulence = Transliteration::toVersion(mRpl.turbulence, v);

    mRpl.equipment = Transliteration::toVersion(mRpl.equipment, v);

    mRpl.adep = Transliteration::toVersion(mRpl.adep, v);

    mRpl.speed = Transliteration::toVersion(mRpl.speed, v);
    mRpl.flightLevel = Transliteration::toVersion(mRpl.flightLevel, v);
    mRpl.route = Transliteration::routeToVersion(mRpl.route, v);

    mRpl.ades = Transliteration::toVersion(mRpl.ades, v);
    mRpl.alt1 = Transliteration::toVersion(mRpl.alt1, v);
    mRpl.alt2 = Transliteration::toVersion(mRpl.alt2, v);
    QStringList alt = mRpl.otherAlternative;
    mRpl.otherAlternative.clear();
    foreach (const QString &a, alt)
        mRpl.otherAlternative << Transliteration::toVersion(a, v);

    mRpl.other = Transliteration::toVersion(mRpl.other, v);

    mRpl.version = v;

    mType = Transliteration::toVersion(TelegramType::CNL_RPL1, v);
}

FplChange CnlRplMessage::changes() const
{
    FplChange ret;

    ret.acid = mRpl.acid;
    ret.flightRules = mRpl.flightRules;
    ret.typeOfFlight = mRpl.typeOfFlight;

    ret.numberOfAircrafts = mRpl.numberOfAircrafts;
    ret.aircraftType = mRpl.aircraftType;
    ret.turbulence = mRpl.turbulence;

    ret.equipment = mRpl.equipment;

    ret.adep = mRpl.adep;
    ret.departureTime = mRpl.departureTime;

    ret.speed = mRpl.speed;
    ret.flightLevel = mRpl.flightLevel;
    ret.route = mRpl.route;

    ret.ades = mRpl.ades;
    ret.eet = mRpl.eet;
    ret.alt1 = mRpl.alt1;
    ret.alt2 = mRpl.alt2;
    ret.otherAlternative = mRpl.otherAlternative;

    ret.arrivalTime = mRpl.arrivalTime;

    ret.other = mRpl.other;

    ret.exFpl = mRpl.exFpl;

    ret.enabled = mRpl.enabled;

    ret.version = mRpl.version;

    ret.changesFlags = FplChange::ALL;

    return ret;
}

AtsStruct CnlRplMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRpl.acid;
    ret.adep = mRpl.adep;
    ret.ades = mRpl.ades;
    ret.dof = mRpl.departureTime.date();

    ret.version = mRpl.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
