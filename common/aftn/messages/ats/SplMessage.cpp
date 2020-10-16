#include "SplMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SplMessage::SplMessage()
{
}

SplMessage::SplMessage(const SplStruct &s) :
    mSpl(s)
{
    setVersion(s.version);
}

SplMessage::~SplMessage()
{
}

QString SplMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mSpl.acid;

    ret += "-";
    ret += mSpl.adep;
    if (mSpl.departureTime.isValid())
        ret += mSpl.departureTime.toString("hhmm");

    if (!mSpl.ades.isEmpty()) {
        ret += "-";
        ret += mSpl.ades;
        ret += removeColonFromTime(mSpl.eet);
    }

    if (!mSpl.other.isEmpty()) {
        ret += "-";
        ret += mSpl.other;
    } else {
        ret += "-0";
    }

    if (!mSpl.additional.isEmpty()) {
        ret += "-";
        ret += mSpl.additional;
    } else {
        ret += "-0";
    }

    ret += ")";

    return ret;
}

QString SplMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE ISpl";
    // 7 field
    ret += " -ARCID " + mSpl.acid;
    // 13 field
    ret += " -ADEP " + mSpl.adep;
    // 16 field
    if (!mSpl.ades.isEmpty())
        ret += " -ADES " + mSpl.ades;
    // 18 field
    if (!mSpl.other.isEmpty())
        ret += otherToAdexp(mSpl.other);

    return ret;
}

void SplMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mSpl.acid = Transliteration::acidToVersion(mSpl.acid, v);
    mSpl.adep = Transliteration::toVersion(mSpl.adep, v);
    mSpl.ades = Transliteration::toVersion(mSpl.ades, v);
    mSpl.other = Transliteration::toVersion(mSpl.other, v);

    mSpl.version = v;

    mType = Transliteration::toVersion(TelegramType::SPL, v);
}

FplChange SplMessage::changes() const
{
    FplChange ret;

    ret.additionalInfo = mSpl.additional;

    ret.version = mSpl.version;

    ret.changesFlags = FplChange::Changes(FplChange::ADDITIONAL_INFO);

    return ret;
}

AtsStruct SplMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mSpl.acid;
    ret.adep = mSpl.adep;
    ret.ades = mSpl.ades;
    ret.dof = mSpl.departureTime.date();

    ret.version = mSpl.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
