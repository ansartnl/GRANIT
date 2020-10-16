#include "Telegram.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

Telegram::Telegram()
{
    mType = TelegramType::Unknown;
    mVersion = ParseVersion::PV_UNKNOWN;
}

Telegram::~Telegram()
{
}

QString Telegram::type() const
{
    return mType;
}

ParseVersion::Version Telegram::version() const
{
    return mVersion;
}

void Telegram::setVersion(ParseVersion::Version v)
{
    mVersion = v;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
