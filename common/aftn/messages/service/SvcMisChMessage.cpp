#include "SvcMisChMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcMisChMessage::SvcMisChMessage()
{
}

SvcMisChMessage::SvcMisChMessage(const SvcMisChStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcMisChMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("MIS", version());

    ret += " ";
    ret += Transliteration::toVersion("CH", version());
    if (mSvc.timeExpected.isValid())
        ret += mSvc.timeExpected.toString("hhmm");

    if (!mSvc.transmissionIdentificator.isEmpty()) {
        ret += " ";
        ret += Transliteration::toVersion("LR", version());

        ret += " ";
        ret += mSvc.transmissionIdentificator;
    }

    return ret;
}

ParseVersion::Version SvcMisChMessage::version() const
{
    return mSvc.version;
}

void SvcMisChMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.transmissionIdentificator = Transliteration::toVersion(mSvc.transmissionIdentificator, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
