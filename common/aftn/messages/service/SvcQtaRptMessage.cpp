#include "SvcQtaRptMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcQtaRptMessage::SvcQtaRptMessage()
{
}

SvcQtaRptMessage::SvcQtaRptMessage(const SvcQtaRptStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcQtaRptMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("RPT", version());

    ret += " ";
    ret += mSvc.dateTime.toString("ddhhmm");

    ret += " ";
    ret += mSvc.address;

    return ret;
}

ParseVersion::Version SvcQtaRptMessage::version() const
{
    return mSvc.version;
}

void SvcQtaRptMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.address = Transliteration::addressToVersion(mSvc.address, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
