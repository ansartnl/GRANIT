#include "SvcQtaMsrMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaMsrMessage::SvcQtaMsrMessage()
{
}

SvcQtaMsrMessage::SvcQtaMsrMessage(const SvcQtaMsrStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaMsrMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("MSR", version());

    ret += " ";
    ret += mSvc.transmissionIdentificator;

    return ret;
}

ParseVersion::Version SvcQtaMsrMessage::version() const
{
    return mSvc.version;
}

void SvcQtaMsrMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.transmissionIdentificator = Transliteration::toVersion(mSvc.transmissionIdentificator, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
