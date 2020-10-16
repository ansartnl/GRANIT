#include "TplMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

TplMessage::TplMessage()
{
}

TplMessage::TplMessage(const TplStruct &s) :
    FplMessage(s)
{
    setVersion(s.version);
}

TplMessage::~TplMessage()
{
}

void TplMessage::setVersion(ParseVersion::Version v)
{
    FplMessage::setVersion(v);

    mType = TelegramType::TPL;
}

FplChange TplMessage::changes() const
{
    FplChange ret;

    ret.version = mFpl.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
