#include "NonStandardRviMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

NonStandardRviMessage::NonStandardRviMessage()
{
}

NonStandardRviMessage::NonStandardRviMessage(const NonStandardRviStruct &s) :
    mRvi(s)
{
    setVersion(s.version);
}

NonStandardRviMessage::~NonStandardRviMessage()
{
}

ParseVersion::Version NonStandardRviMessage::version() const
{
    return mRvi.version;
}

QString NonStandardRviMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRvi.id;

    ret += ")";

    return ret;
}

void NonStandardRviMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mRvi.id = Transliteration::toVersion(mRvi.id, v);

    mType = Transliteration::toVersion(TelegramType::RVI, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
