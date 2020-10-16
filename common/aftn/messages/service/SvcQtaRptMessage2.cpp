#include "SvcQtaRptMessage2.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaRptMessage2::SvcQtaRptMessage2()
{
}

SvcQtaRptMessage2::SvcQtaRptMessage2(const SvcQtaRptStruct2 &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaRptMessage2::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("RPT", version());

    ret += " ";
    ret += mSvc.aftnChannel;
    ret += QString("%1").arg(mSvc.numFrom, 3, 10, QChar('0'));
    if (mSvc.numFrom != mSvc.numTill) {
        ret += "-";
        ret += QString("%1").arg(mSvc.numTill, 3, 10, QChar('0'));
    }

    return ret;
}

ParseVersion::Version SvcQtaRptMessage2::version() const
{
    return mSvc.version;
}

void SvcQtaRptMessage2::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.aftnChannel = Transliteration::toVersion(mSvc.aftnChannel, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
