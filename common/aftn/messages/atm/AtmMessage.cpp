#include "AtmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AtmMessage::AtmMessage()
{
}

AtmMessage::AtmMessage(const AtmStruct &s) :
    mAtm(s)
{
    setVersion(s.version);
}

AtmMessage::~AtmMessage()
{
}

QString AtmMessage::data() const
{
    QString ret = "(";
    ret += atmConst().type;
    ret += "-";
    ret += atmConst().other;
    ret += ")";

    return ret;
}

ParseVersion::Version AtmMessage::version() const
{
    return atmConst().version;
}

void AtmMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    atm().type = Transliteration::toVersion(atm().type, v);
    atm().text = Transliteration::toVersion(atm().text, v);

    atm().version = v;

    mType = atm().type;
}

AtmStruct & AtmMessage::atm()
{
    return mAtm;
}

const AtmStruct & AtmMessage::atmConst() const
{
    return mAtm;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
