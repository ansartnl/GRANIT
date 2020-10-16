#include "AcpMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AcpMessage::AcpMessage()
{
}

AcpMessage::AcpMessage(const AcpStruct &s) :
    mAcp(s)
{
    setVersion(s.version);
}

AcpMessage::~AcpMessage()
{
}

QString AcpMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAcp.acid;

    ret += "-";
    ret += mAcp.adep;

    ret += "-";
    ret += mAcp.ades;

    ret += ")";

    return ret;
}

ParseVersion::Version AcpMessage::version() const
{
    return mAcp.version;
}

void AcpMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mAcp.acid = Transliteration::acidToVersion(mAcp.acid, v);
    mAcp.adep = Transliteration::acidToVersion(mAcp.adep, v);
    mAcp.ades = Transliteration::acidToVersion(mAcp.ades, v);

    mAcp.version = v;

    mType = Transliteration::toVersion(TelegramType::ACP, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
