#include "ArrMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ArrMessage::ArrMessage()
{
}

ArrMessage::ArrMessage(const ArrStruct &s) :
    mArr(s)
{
    setVersion(s.version);
}

ArrMessage::~ArrMessage()
{
}

QString ArrMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mArr.acid;

    ret += "-";
    ret += mArr.adep + mArr.departureTime.toString("hhmm");
    if (!mArr.ades.isEmpty()) {
        ret += "-";
        ret += mArr.ades;
    }

    ret += "-";
    ret += mArr.arrivalAirport + mArr.arrivalTime.toString("hhmm");
    if ((mArr.arrivalAirport == "ZZZZ") || (mArr.arrivalAirport == QString::fromUtf8("ЗЗЗЗ")))
        ret += " " + mArr.arrivalAirportName;

#if defined(BAN_BUILD)
    if (!mArr.other.isEmpty()) {
        ret += "-";
        ret += mArr.other;
    }
#endif

    ret += ")";

    return ret;
}

QString ArrMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IARR";
    // 7 field
    ret += " -ARCID " + mArr.acid;
    // 13 field
    ret += " -ADEP " + mArr.adep;
    // 16 field
    if (!mArr.ades.isEmpty())
        ret += " -ADES " + mArr.ades;
    // 17 field
    ret += " -ADARR " + mArr.arrivalAirport;
    ret += " -ATA " + mArr.arrivalTime.toString("hhmm");
    if ((mArr.arrivalAirport == "ZZZZ") || (mArr.arrivalAirport == QString::fromUtf8("ЗЗЗЗ")))
        ret += " -ADARRZ " + mArr.arrivalAirportName;

#if defined(BAN_BUILD)
    if (!mArr.other.isEmpty())
        ret += otherToAdexp(mArr.other);
#endif

    return ret;
}

void ArrMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mArr.acid = Transliteration::acidToVersion(mArr.acid, v);
    mArr.adep = Transliteration::toVersion(mArr.adep, v);
    mArr.ades = Transliteration::toVersion(mArr.ades, v);
    mArr.arrivalAirport = Transliteration::toVersion(mArr.arrivalAirport, v);
    mArr.arrivalAirportName = Transliteration::toVersion(mArr.arrivalAirportName, v);

#if defined(BAN_BUILD)
    mArr.other = Transliteration::toVersion(mArr.other, v);
#endif

    mArr.version = v;

    mType = Transliteration::toVersion(TelegramType::ARR, v);
}

FplChange ArrMessage::changes() const
{
    FplChange ret;

    ret.arrivalTime = mArr.arrivalTime;
    ret.exFpl = FplChange::ARRIVED;
    ret.boardNumber = mArr.boardNumber;

    ret.version = mArr.version;

    ret.changesFlags = FplChange::Changes(FplChange::ARRIVAL_TIME | FplChange::EX_FPL);
    if (!ret.boardNumber.isEmpty())
        ret.changesFlags = FplChange::Changes(ret.changesFlags | FplChange::BOARD_NUMBER);

    return ret;
}

AtsStruct ArrMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mArr.acid;
    ret.adep = mArr.adep;
    ret.ades = mArr.ades;
    if (mArr.ades.isEmpty())
        ret.ades = mArr.arrivalAirport;
    ret.dof = mArr.arrivalTime.date();

    ret.version = mArr.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
