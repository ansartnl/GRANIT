#include "RrpMessage2.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RrpMessage2::RrpMessage2()
{
}

RrpMessage2::RrpMessage2(const RrpStruct2 &s) :
    AtfFplMessage(s), mRrp(s)
{
    setVersion(s.version);
}

RrpMessage2::~RrpMessage2()
{
}

QString RrpMessage2::data() const
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

    ret += "-15/";
    ret += mRrp.route;

    ret += ")";

    return ret;
}

void RrpMessage2::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mRrp.acid = Transliteration::acidToVersion(mRrp.acid, v);
    mRrp.adep = Transliteration::toVersion(mRrp.adep, v);
    mRrp.ades = Transliteration::toVersion(mRrp.ades, v);
    mRrp.other = Transliteration::toVersion(mRrp.other, v);
    mRrp.route = Transliteration::toVersion(mRrp.route, v);
}

AtfStruct & RrpMessage2::atf()
{
    return (AtfStruct &)mRrp;
}

const AtfStruct & RrpMessage2::atfConst() const
{
    return (const AtfStruct &)mRrp;
}

AtsStruct RrpMessage2::ats() const
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
