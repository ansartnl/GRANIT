#include "AtfMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AtfMessage::AtfMessage()
{
}

AtfMessage::AtfMessage(const AtfStruct &s) :
    mAtf(s)
{
    setVersion(s.version);
}

AtfMessage::~AtfMessage()
{
}

QString AtfMessage::data() const
{
    return atfConst().text;
}

ParseVersion::Version AtfMessage::version() const
{
    return atfConst().version;
}

void AtfMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    atf().type = Transliteration::toVersion(atf().type, v);
    atf().text = Transliteration::toVersion(atf().text, v);

    atf().version = v;

    mType = atf().type;
}

AtfStruct & AtfMessage::atf()
{
    return mAtf;
}

const AtfStruct & AtfMessage::atfConst() const
{
    return mAtf;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
