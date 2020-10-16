#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AtfFplMessage::AtfFplMessage()
{
}

AtfFplMessage::AtfFplMessage(const AtfFplStruct &s) :
    mAtfFpl(s)
{
    setVersion(s.version);
}

AtfFplMessage::~AtfFplMessage()
{
}

QString AtfFplMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAtfFpl.acid;

    ret += "-";
    ret += mAtfFpl.adep;

    if (!mAtfFpl.ades.isEmpty()) {
        ret += "-";
        ret += mAtfFpl.ades;
    }

    if (!mAtfFpl.other.isEmpty()) {
        ret += "-";
        ret += mAtfFpl.other;
    }

    ret += ")";

    return ret;
}

void AtfFplMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mAtfFpl.acid = Transliteration::acidToVersion(mAtfFpl.acid, v);
    mAtfFpl.adep = Transliteration::toVersion(mAtfFpl.adep, v);
    mAtfFpl.ades = Transliteration::toVersion(mAtfFpl.ades, v);
    mAtfFpl.other = Transliteration::toVersion(mAtfFpl.other, v);
}

AtfStruct & AtfFplMessage::atf()
{
    return (AtfStruct &)mAtfFpl;
}

const AtfStruct & AtfFplMessage::atfConst() const
{
    return (const AtfStruct &)mAtfFpl;
}

AtsStruct AtfFplMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mAtfFpl.acid;
    ret.adep = mAtfFpl.adep;
    ret.ades = mAtfFpl.ades;
    ret.dof = mAtfFpl.dof;

    ret.version = mAtfFpl.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
