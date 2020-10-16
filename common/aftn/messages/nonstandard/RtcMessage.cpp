#include "RtcMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RtcMessage::RtcMessage()
{
}

RtcMessage::RtcMessage(const RtcStruct &s) :
    mRtc(s)
{
    setVersion(ParseVersion::PV_LATIN);
}

RtcMessage::~RtcMessage()
{
}

QString RtcMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRtc.time.toString("yyyyMMdd hhmmss");

    ret += ")";

    return ret;
}

ParseVersion::Version RtcMessage::version() const
{
    return ParseVersion::PV_LATIN;
}

void RtcMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mType = TelegramType::RTC;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
