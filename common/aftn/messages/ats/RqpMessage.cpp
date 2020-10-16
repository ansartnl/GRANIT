#include "RqpMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqpMessage::RqpMessage()
{
}

RqpMessage::RqpMessage(const RqpStruct &s) :
    mRqp(s)
{
    setVersion(s.version);
}

RqpMessage::~RqpMessage()
{
}

QString RqpMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRqp.acid;

    ret += "-";
    ret += mRqp.adep;

    if (!mRqp.ades.isEmpty()) {
        ret += "-";
        ret += mRqp.ades;
    }

    if (!mRqp.other.isEmpty()) {
        ret += "-";
        ret += mRqp.other;
    } else {
        ret += "-0";
    }

    ret += ")";

    return ret;
}

QString RqpMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IRqp";
    // 7 field
    ret += " -ARCID " + mRqp.acid;
    // 13 field
    ret += " -ADEP " + mRqp.adep;
    // 16 field
    if (!mRqp.ades.isEmpty())
        ret += " -ADES " + mRqp.ades;
    // 18 field
    if (!mRqp.other.isEmpty())
        ret += otherToAdexp(mRqp.other);

    return ret;
}

void RqpMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mRqp.acid = Transliteration::acidToVersion(mRqp.acid, v);
    mRqp.adep = Transliteration::toVersion(mRqp.adep, v);
    mRqp.ades = Transliteration::toVersion(mRqp.ades, v);
    mRqp.other = Transliteration::toVersion(mRqp.other, v);

    mRqp.version = v;

    mType = Transliteration::toVersion(TelegramType::RQP, v);
}

FplChange RqpMessage::changes() const
{
    FplChange ret;
    ret.version = mRqp.version;
    return ret;
}

AtsStruct RqpMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRqp.acid;
    ret.adep = mRqp.adep;
    ret.ades = mRqp.ades;
    ret.dof = mRqp.dof;

    ret.version = mRqp.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
