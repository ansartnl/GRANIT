#include "SvcMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcMessage::SvcMessage()
{
}

SvcMessage::SvcMessage(const SvcStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcMessage::data() const
{
    return mSvc.text;
}

ParseVersion::Version SvcMessage::version() const
{
    return mSvc.version;
}

void SvcMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.text = Transliteration::toVersion(mSvc.text, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
