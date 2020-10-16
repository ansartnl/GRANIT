#include "SvcLrLsMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcLrLsMessage::SvcLrLsMessage()
{
}

SvcLrLsMessage::SvcLrLsMessage(const SvcLrLsStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcLrLsMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("LR", version());

    ret += " ";
    ret += mSvc.received;

    ret += " ";
    ret += Transliteration::toVersion("LS", version());

    ret += " ";
    ret += mSvc.sent;

    return ret;
}

ParseVersion::Version SvcLrLsMessage::version() const
{
    return mSvc.version;
}

void SvcLrLsMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.received = Transliteration::toVersion(mSvc.received, v);
    mSvc.sent = Transliteration::toVersion(mSvc.sent, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
