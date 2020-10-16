#include "FplMessage.h"

#include "TelegramTypes.h"

#include "AldMessage.h"
#include "ArrMessage.h"
#include "ChgMessage.h"
#include "CnlMessage.h"
#include "DepMessage.h"
#include "DlaMessage.h"
#include "FlaMessage.h"
#include "PpnMessage.h"
#include "RcfMessage.h"
#include "RqpMessage.h"
#include "RqsMessage.h"
#include "SplMessage.h"
#include "UlsMessage.h"
#include "UtpMessage.h"

#include "AnpMessage.h"
#include "AlrMessageShort.h"

#include "AtfFplMessage.h"
#include "SltMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

FplMessage::FplMessage()
{
}

FplMessage::FplMessage(const FplStruct &s) :
    mFpl(s)
{
    setVersion(s.version);
}

FplMessage::~FplMessage()
{
}

QString FplMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mFpl.acid;

    ret += "-";
    ret += mFpl.flightRules + mFpl.typeOfFlight;
    ret += "\r\n";

    ret += "-";
    if (mFpl.numberOfAircrafts == 1)
        ret += QString("%1/%2").arg(mFpl.aircraftType).arg(mFpl.turbulence);
    else
        ret += QString("%1%2/%3").arg(mFpl.numberOfAircrafts).arg(mFpl.aircraftType).arg(mFpl.turbulence);

    ret += "-";
    ret += mFpl.equipment;
    ret += "\r\n";

    ret += "-";
    ret += mFpl.adep + mFpl.departureTime.toString("hhmm");
    ret += "\r\n";

    ret += "-";
    ret += mFpl.speed + mFpl.flightLevel + " " + mFpl.route;
    ret += "\r\n";

    ret += "-";
    ret += mFpl.ades + removeColonFromTime(mFpl.eet);
    if (!mFpl.alt1.isEmpty())
        ret += " " + mFpl.alt1;
    if (!mFpl.alt2.isEmpty())
        ret += " " + mFpl.alt2;
    if (!mFpl.otherAlternative.isEmpty())
        ret += " " + mFpl.otherAlternative.join(" ");
    ret += "\r\n";

    ret += "-";
    ret += mFpl.other;

    if (!mFpl.additionalInfo.isEmpty()) {
        ret += "\r\n";
        ret += "-";
        ret += mFpl.additionalInfo;
    }

    ret += ")";

    return ret;
}

QString FplMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IFPL";
    // 7 field
    ret += " -ARCID " + mFpl.acid;
    // 8 field
    ret += " -FLTRUL " + mFpl.flightRules;
    ret += " -FLTTYP " + mFpl.typeOfFlight;
    // 9 field
    if (mFpl.numberOfAircrafts > 1)
        ret += " -NBARC " + mFpl.numberOfAircrafts;
    ret += " -ARCTYP " + mFpl.aircraftType;
    ret += " -WKTRC " + mFpl.turbulence;
    // 10 field
    QRegExp rexp("(.+)/(.+)");
    if (rexp.exactMatch(mFpl.equipment)) {
        ret += " -CEQPT " + rexp.cap(1);
        ret += " -SEQPT " + rexp.cap(2);
    } else {
        ret += " -CEQPT " + mFpl.equipment;
    }
    // 13 field
    ret += " -ADEP " + mFpl.adep;
    ret += " -EOBT " + mFpl.departureTime.toString("hhmm");
    // 15 field
    ret += " -SPEED " + mFpl.speed;
    ret += " -RFL " + mFpl.flightLevel;
    ret += " -ROUTE " + mFpl.speed + mFpl.flightLevel + " " + mFpl.route;
    // 16 field
    ret += " -ADES " + mFpl.ades;
    ret += " -TTLEET " + mFpl.departureTime.toString("hhmm");
    if (!mFpl.alt1.isEmpty())
        ret += " -ALTRNT1 " + mFpl.alt1;
    if (!mFpl.alt2.isEmpty())
        ret += " -ALTRNT2 " + mFpl.alt2;
    // 18 field
    if (!mFpl.other.isEmpty())
        ret += otherToAdexp(mFpl.other);

    return ret;
}

void FplMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mFpl.acid = Transliteration::acidToVersion(mFpl.acid, v);
    mFpl.flightRules = Transliteration::toVersion(mFpl.flightRules, v);
    mFpl.typeOfFlight = Transliteration::toVersion(mFpl.typeOfFlight, v);

    mFpl.aircraftType = Transliteration::aircraftTypeToVersion(mFpl.aircraftType, v);
    mFpl.turbulence = Transliteration::toVersion(mFpl.turbulence, v);

    mFpl.equipment = Transliteration::toVersion(mFpl.equipment, v);

    mFpl.adep = Transliteration::toVersion(mFpl.adep, v);

    mFpl.speed = Transliteration::toVersion(mFpl.speed, v);
    mFpl.flightLevel = Transliteration::toVersion(mFpl.flightLevel, v);
    mFpl.route = Transliteration::routeToVersion(mFpl.route, v);

    mFpl.ades = Transliteration::toVersion(mFpl.ades, v);
    mFpl.alt1 = Transliteration::toVersion(mFpl.alt1, v);
    mFpl.alt2 = Transliteration::toVersion(mFpl.alt2, v);
    QStringList alt = mFpl.otherAlternative;
    mFpl.otherAlternative.clear();
    foreach (const QString &a, alt)
        mFpl.otherAlternative << Transliteration::toVersion(a, v);

    mFpl.other = Transliteration::toVersion(mFpl.other, v);

    mFpl.version = v;

    mType = Transliteration::toVersion(TelegramType::FPL, v);

    mFpl.additionalInfo = Transliteration::toVersion(mFpl.additionalInfo, v);
}

FplChange FplMessage::changes() const
{
    FplChange ret;

    ret.acid = mFpl.acid;
    ret.flightRules = mFpl.flightRules;
    ret.typeOfFlight = mFpl.typeOfFlight;

    ret.numberOfAircrafts = mFpl.numberOfAircrafts;
    ret.aircraftType = mFpl.aircraftType;
    ret.turbulence = mFpl.turbulence;

    ret.equipment = mFpl.equipment;

    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;

    ret.speed = mFpl.speed;
    ret.flightLevel = mFpl.flightLevel;
    ret.route = mFpl.route;

    ret.ades = mFpl.ades;
    ret.eet = mFpl.eet;
    ret.alt1 = mFpl.alt1;
    ret.alt2 = mFpl.alt2;
    ret.otherAlternative = mFpl.otherAlternative;

    ret.arrivalTime = mFpl.arrivalTime;

    ret.other = mFpl.other;
    ret.letter = mFpl.letter;
    ret.boardNumber = mFpl.boardNumber;

    ret.exFpl = mFpl.exFpl;

    ret.enabled = mFpl.enabled;

    ret.version = mFpl.version;

    ret.changesFlags = FplChange::ALL;

    ret.additionalInfo = mFpl.additionalInfo;

    return ret;
}

AtsStruct FplMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = mFpl.departureTime.date();

    ret.version = mFpl.version;

    return ret;
}

ArrMessage * FplMessage::arr() const
{
    return new ArrMessage(arrStruct());
}

ChgMessage * FplMessage::chg() const
{
    return new ChgMessage(chgStruct());
}

CnlMessage * FplMessage::cnl() const
{
    return new CnlMessage(cnlStruct());
}

DepMessage * FplMessage::dep() const
{
    return new DepMessage(depStruct());
}

DlaMessage * FplMessage::dla() const
{
    return new DlaMessage(dlaStruct());
}

FlaMessage * FplMessage::fla() const
{
    return new FlaMessage(flaStruct());
}

PpnMessage * FplMessage::ppn() const
{
    return new PpnMessage(ppnStruct());
}

UlsMessage * FplMessage::uls() const
{
    return new UlsMessage(ulsStruct());
}

RqpMessage * FplMessage::rqp() const
{
    return new RqpMessage(rqpStruct());
}

RqsMessage * FplMessage::rqs() const
{
    return new RqsMessage(rqsStruct());
}

SplMessage * FplMessage::spl() const
{
    return new SplMessage(splStruct());
}

AnpMessage * FplMessage::anp() const
{
    return new AnpMessage(anpStruct());
}

AlrMessageShort * FplMessage::alrShort() const
{
    return new AlrMessageShort(alrStructShort());
}

AlrMessage * FplMessage::alr() const
{
    return new AlrMessage(alrStruct());
}

AtfFplMessage * FplMessage::atf() const
{
    return new AtfFplMessage(atfStruct());
}

SltMessage * FplMessage::slt() const
{
    return new SltMessage(sltStruct());
}

const ArrStruct FplMessage::arrStruct() const
{
    ArrStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;
    ret.arrivalAirport = mFpl.ades;
    ret.arrivalTime = mFpl.arrivalTime;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const ChgStruct FplMessage::chgStruct() const
{
    ChgStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    ret.changes = mFpl;

    return ret;
}

const CnlStruct FplMessage::cnlStruct() const
{
    CnlStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const DepStruct FplMessage::depStruct() const
{
    DepStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const DlaStruct FplMessage::dlaStruct() const
{
    DlaStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const FlaStruct FplMessage::flaStruct() const
{
    FlaStruct ret;

    ret.acid = mFpl.acid;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const PpnStruct FplMessage::ppnStruct() const
{
    PpnStruct ret;

    ret.acid = mFpl.acid;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const UlsStruct FplMessage::ulsStruct() const
{
    UlsStruct ret;

    ret.acid = mFpl.acid;
#ifdef PATCH1
    ret.hasField9 = true;
    ret.aircraftType = Transliteration::aircraftTypeToVersion(mFpl.aircraftType, ret.version);
    ret.turbulence = Transliteration::toVersion(mFpl.turbulence, ret.version);
    ret.numberOfAircrafts = mFpl.numberOfAircrafts;
#endif
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const RqpStruct FplMessage::rqpStruct() const
{
    RqpStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const RqsStruct FplMessage::rqsStruct() const
{
    RqsStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const SplStruct FplMessage::splStruct() const
{
    SplStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.departureTime = mFpl.departureTime;
    ret.ades = mFpl.ades;
    ret.eet = mFpl.eet;
    ret.other = mFpl.other;
    ret.additional = mFpl.additionalInfo;
    ret.version = mFpl.version;

    return ret;
}

const AldStruct FplMessage::aldStruct() const
{
    AldStruct ret;

    ret.acid = mFpl.acid;

    ret.numberOfAircrafts = mFpl.numberOfAircrafts;
    ret.aircraftType = mFpl.aircraftType;
    ret.turbulence = mFpl.turbulence;

    ret.dof = ats().dof;
    ret.version = mFpl.version;

    return ret;
}

const RcfStruct FplMessage::rcfStruct() const
{
    RcfStruct ret;

    ret.acid = mFpl.acid;

    ret.version = mFpl.version;

    return ret;
}

const AnpStruct FplMessage::anpStruct() const
{
    AnpStruct ret;

    ret.acid = mFpl.acid;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    ret.type = Transliteration::toVersion(TelegramType::ANP1, ret.version);

    ret.numberOfAircrafts = mFpl.numberOfAircrafts;
    ret.aircraftType = mFpl.aircraftType;

    ret.other = Transliteration::toVersion(QString("DOF/%1").arg(ret.dof.toString("yyMMdd"))
                                           , ret.version);

    return ret;
}

const AlrStructShort FplMessage::alrStructShort() const
{
    AlrStructShort ret;

    ret.acid = mFpl.acid;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    ret.type = Transliteration::toVersion(TelegramType::ALR, ret.version);

    ret.numberOfAircrafts = mFpl.numberOfAircrafts;
    ret.aircraftType = mFpl.aircraftType;

    ret.other = Transliteration::toVersion(QString("DOF/%1").arg(ret.dof.toString("yyMMdd"))
                                           , ret.version);

    return ret;
}

const AlrStruct FplMessage::alrStruct() const
{
    AlrStruct ret;

    ret.version = mFpl.version;

    ret.acid = Transliteration::acidToVersion(mFpl.acid, ret.version);
    ret.flightRules = Transliteration::toVersion(mFpl.flightRules, ret.version);
    ret.typeOfFlight = Transliteration::toVersion(mFpl.typeOfFlight, ret.version);

    ret.aircraftType = Transliteration::aircraftTypeToVersion(mFpl.aircraftType, ret.version);
    ret.turbulence = Transliteration::toVersion(mFpl.turbulence, ret.version);
    ret.numberOfAircrafts = mFpl.numberOfAircrafts;

    ret.equipment = Transliteration::toVersion(mFpl.equipment, ret.version);

    ret.adep = Transliteration::toVersion(mFpl.adep, ret.version);
    ret.departureTime = mFpl.departureTime;

    ret.speed = Transliteration::toVersion(mFpl.speed, ret.version);
    ret.eet = mFpl.eet;
    ret.flightLevel = Transliteration::toVersion(mFpl.flightLevel, ret.version);
    ret.route = Transliteration::routeToVersion(mFpl.route, ret.version);

    ret.ades = Transliteration::toVersion(mFpl.ades, ret.version);
    ret.alt1 = Transliteration::toVersion(mFpl.alt1, ret.version);
    ret.alt2 = Transliteration::toVersion(mFpl.alt2, ret.version);

    ret.arrivalTime = mFpl.arrivalTime;

    ret.other = Transliteration::toVersion(mFpl.other, ret.version);

    ret.additionalInfo = Transliteration::toVersion(mFpl.additionalInfo, ret.version);

    ret.dof = ats().dof;

    ret.type = Transliteration::toVersion(TelegramType::ALR, ret.version);

    ret.other = Transliteration::toVersion(QString("DOF/%1").arg(ret.dof.toString("yyMMdd"))
                                        , ret.version);

    ret.additionalInfo = Transliteration::toVersion(mFpl.additionalInfo, ret.version);

    return ret;
}

const AtfFplStruct FplMessage::atfStruct() const
{
    AtfFplStruct ret;

    ret.acid = mFpl.acid;
    ret.adep = mFpl.adep;
    ret.ades = mFpl.ades;
    ret.dof = ats().dof;
    ret.version = mFpl.version;

    ret.other = Transliteration::toVersion(QString("DOF/%1").arg(ret.dof.toString("yyMMdd"))
                                           , ret.version);

    return ret;
}

const SltStruct FplMessage::sltStruct() const
{
    SltStruct ret(atfStruct());

    ret.departureTime = mFpl.departureTime;
    ret.other += " " + Transliteration::toVersion(QString("ETA/%1%2")
                                                  .arg(mFpl.ades)
                                                  .arg(mFpl.arrivalTime.toString("hhmm"))
                                                  , ret.version);

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
