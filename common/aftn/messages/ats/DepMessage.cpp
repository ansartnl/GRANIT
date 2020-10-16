#include "DepMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

DepMessage::DepMessage()
{
}

DepMessage::DepMessage(const DepStruct &s) :
    mDep(s)
{
    setVersion(s.version);
}

DepMessage::~DepMessage()
{
}

QString DepMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mDep.acid;

    ret += "-";
    ret += mDep.adep + mDep.departureTime.toString("hhmm");

    if (!mDep.ades.isEmpty()) {
        ret += "-";
        ret += mDep.ades;
    }

    if (!mDep.other.isEmpty()) {
        ret += "-";
        ret += mDep.other;
    }

    ret += ")";

    return ret;
}

QString DepMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IDEP";
    // 7 field
    ret += " -ARCID " + mDep.acid;
    // 13 field
    ret += " -ADEP " + mDep.adep;
    ret += " -EOBT " + mDep.departureTime.toString("hhmm");
    // 16 field
    if (!mDep.ades.isEmpty())
        ret += " -ADES " + mDep.ades;
    // 18 field
    if (!mDep.other.isEmpty())
        ret += otherToAdexp(mDep.other);

    return ret;
}

void DepMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mDep.acid = Transliteration::acidToVersion(mDep.acid, v);
    mDep.adep = Transliteration::toVersion(mDep.adep, v);
    mDep.ades = Transliteration::toVersion(mDep.ades, v);
    mDep.other = Transliteration::toVersion(mDep.other, v);

    mDep.version = v;

    mType = Transliteration::toVersion(TelegramType::DEP, v);
}

FplChange DepMessage::changes() const
{
    FplChange ret;

    ret.departureTime = mDep.departureTime;
    ret.exFpl = FplChange::DEPARTURED;
    ret.boardNumber = mDep.boardNumber;

    ret.version = mDep.version;

    ret.changesFlags = FplChange::Changes(FplChange::DEPARTURE_TIME | FplChange::EX_FPL);
    if (!ret.boardNumber.isEmpty())
        ret.changesFlags = FplChange::Changes(ret.changesFlags | FplChange::BOARD_NUMBER);

    return ret;
}

AtsStruct DepMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mDep.acid;
    ret.adep = mDep.adep;
    ret.ades = mDep.ades;
    ret.dof = mDep.departureTime.date();

    ret.version = mDep.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
