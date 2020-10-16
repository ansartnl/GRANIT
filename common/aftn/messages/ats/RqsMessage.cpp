#include "RqsMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqsMessage::RqsMessage()
{
}

RqsMessage::RqsMessage(const RqsStruct &s) :
    mRqs(s)
{
    setVersion(s.version);
}

RqsMessage::~RqsMessage()
{
}

QString RqsMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRqs.acid;

    ret += "-";
    ret += mRqs.adep;

    if (!mRqs.ades.isEmpty()) {
        ret += "-";
        ret += mRqs.ades;
    }

    if (!mRqs.other.isEmpty()) {
        ret += "-";
        ret += mRqs.other;
    } else {
        ret += "-0";
    }

    ret += ")";

    return ret;
}

QString RqsMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IRqs";
    // 7 field
    ret += " -ARCID " + mRqs.acid;
    // 13 field
    ret += " -ADEP " + mRqs.adep;
    // 16 field
    if (!mRqs.ades.isEmpty())
        ret += " -ADES " + mRqs.ades;
    // 18 field
    if (!mRqs.other.isEmpty())
        ret += otherToAdexp(mRqs.other);

    return ret;
}

void RqsMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mRqs.acid = Transliteration::acidToVersion(mRqs.acid, v);
    mRqs.adep = Transliteration::toVersion(mRqs.adep, v);
    mRqs.ades = Transliteration::toVersion(mRqs.ades, v);
    mRqs.other = Transliteration::toVersion(mRqs.other, v);

    mRqs.version = v;

    mType = Transliteration::toVersion(TelegramType::RQS, v);
}

FplChange RqsMessage::changes() const
{
    FplChange ret;
    ret.version = mRqs.version;
    return ret;
}

AtsStruct RqsMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRqs.acid;
    ret.adep = mRqs.adep;
    ret.ades = mRqs.ades;
    ret.dof = mRqs.dof;

    ret.version = mRqs.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
