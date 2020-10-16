#include "SvcQtaMisMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaMisMessage::SvcQtaMisMessage()
{
}

SvcQtaMisMessage::SvcQtaMisMessage(const SvcQtaMisStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaMisMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("MIS", version());

    ret += " ";
    ret += mSvc.aftnChannel;
    ret += QString("%1").arg(mSvc.numFrom, 3, 10, QChar('0'));
    if (mSvc.numFrom != mSvc.numTill) {
        ret += "-";
        ret += QString("%1").arg(mSvc.numTill, 3, 10, QChar('0'));
    }

    return ret;
}

ParseVersion::Version SvcQtaMisMessage::version() const
{
    return mSvc.version;
}

void SvcQtaMisMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.aftnChannel = Transliteration::toVersion(mSvc.aftnChannel, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
