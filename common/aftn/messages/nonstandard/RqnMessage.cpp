#include "RqnMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqnMessage::RqnMessage(const QString &s, ParseVersion::Version v) :
    source(s), mVersion(ParseVersion::PV_LATIN)
{
    Q_UNUSED(v);
    mType = TelegramType::RQN;
}

RqnMessage::~RqnMessage()
{
}

QString RqnMessage::data() const
{
    return source;
}

ParseVersion::Version RqnMessage::version() const
{
    return mVersion;
}

void RqnMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    source = Transliteration::toVersion(source, v);

    mVersion = v;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
