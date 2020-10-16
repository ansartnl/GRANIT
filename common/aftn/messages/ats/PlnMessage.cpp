#include "PlnMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

PlnMessage::PlnMessage()
{
}

PlnMessage::PlnMessage(const PlnStruct &s) :
    FplMessage(s)
{
    setVersion(s.version);
}

PlnMessage::~PlnMessage()
{
}

void PlnMessage::setVersion(ParseVersion::Version v)
{
    FplMessage::setVersion(v);

    mType = Transliteration::toVersion(TelegramType::PLN, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
