#include "SvcQtaAdsMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaAdsMessage::SvcQtaAdsMessage()
{
}

SvcQtaAdsMessage::SvcQtaAdsMessage(const SvcQtaAdsStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaAdsMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("ADS", version());

    ret += " ";
    ret += mSvc.transmissionIdentificator;

    ret += " ";
    ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("ИСКАЖЕНО") : QString("CORRUPT");

    return ret;
}

ParseVersion::Version SvcQtaAdsMessage::version() const
{
    return mSvc.version;
}

void SvcQtaAdsMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.transmissionIdentificator = Transliteration::toVersion(mSvc.transmissionIdentificator, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
