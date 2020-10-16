#include "SvcLrExpMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcLrExpMessage::SvcLrExpMessage()
{
}

SvcLrExpMessage::SvcLrExpMessage(const SvcLrExpStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcLrExpMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("ПОЛУЧЕН") : QString("LR");

    ret += " ";
    ret += mSvc.received;

    ret += " ";
    ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("ОЖИДАЛСЯ") : QString("EXP");

    ret += " ";
    ret += mSvc.expected;

    return ret;
}

ParseVersion::Version SvcLrExpMessage::version() const
{
    return mSvc.version;
}

void SvcLrExpMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.received = Transliteration::toVersion(mSvc.received, v);
    mSvc.expected = Transliteration::toVersion(mSvc.expected, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
