#include "SvcQtaOgnMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaOgnMessage::SvcQtaOgnMessage()
{
}

SvcQtaOgnMessage::SvcQtaOgnMessage(const SvcQtaOgnStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaOgnMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("OGN", version());

    ret += " ";
    ret += mSvc.transmissionIdentificator;

    ret += " ";
    ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("ИСКАЖЕНО") : QString("CORRUPT");

    return ret;
}

ParseVersion::Version SvcQtaOgnMessage::version() const
{
    return mSvc.version;
}

void SvcQtaOgnMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.transmissionIdentificator = Transliteration::toVersion(mSvc.transmissionIdentificator, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
