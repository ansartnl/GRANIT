#include "CnlMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CnlMessage::CnlMessage()
{
}

CnlMessage::CnlMessage(const CnlStruct &s) :
    mCnl(s)
{
    setVersion(s.version);
}

CnlMessage::~CnlMessage()
{
}

QString CnlMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mCnl.acid;

    ret += "-";
    ret += mCnl.adep + mCnl.departureTime.toString("hhmm");

    if (!mCnl.ades.isEmpty()) {
        ret += "-";
        ret += mCnl.ades;
    }

    if (!mCnl.other.isEmpty()) {
        ret += "-";
        ret += mCnl.other;
    }

    ret += ")";

    return ret;
}

QString CnlMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE ICNL";
    // 7 field
    ret += " -ARCID " + mCnl.acid;
    // 13 field
    ret += " -ADEP " + mCnl.adep;
    ret += " -EOBT " + mCnl.departureTime.toString("hhmm");
    // 16 field
    if (!mCnl.ades.isEmpty())
        ret += " -ADES " + mCnl.ades;
    // 18 field
    if (!mCnl.other.isEmpty())
        ret += otherToAdexp(mCnl.other);

    return ret;
}

void CnlMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mCnl.acid = Transliteration::acidToVersion(mCnl.acid, v);
    mCnl.adep = Transliteration::toVersion(mCnl.adep, v);
    mCnl.ades = Transliteration::toVersion(mCnl.ades, v);
    mCnl.other = Transliteration::toVersion(mCnl.other, v);

    mCnl.version = v;

    mType = Transliteration::toVersion(TelegramType::CNL, v);
}

FplChange CnlMessage::changes() const
{
    FplChange ret;

    ret.exFpl = FplChange::CANCELLED;
    ret.departureTime = mCnl.departureTime;

    ret.version = mCnl.version;

    ret.changesFlags = FplChange::Changes(FplChange::DEPARTURE_TIME | FplChange::EX_FPL);

    return ret;
}

AtsStruct CnlMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mCnl.acid;
    ret.adep = mCnl.adep;
    ret.ades = mCnl.ades;
    ret.dof = mCnl.dof;

    ret.version = mCnl.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
