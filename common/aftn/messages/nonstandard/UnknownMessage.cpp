#include "UnknownMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

UnknownMessage::UnknownMessage(const QString &s) :
    source(s), mVersion(ParseVersion::PV_UNKNOWN)
{
    mType = TelegramType::Unknown;
}

UnknownMessage::~UnknownMessage()
{
}

QString UnknownMessage::data() const
{
    return source;
}

ParseVersion::Version UnknownMessage::version() const
{
    return mVersion;
}

void UnknownMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    source = Transliteration::toVersion(source, v);

    mVersion = v;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
