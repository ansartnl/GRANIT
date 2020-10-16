#include "ResMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ResMessage::ResMessage()
{
}

ResMessage::ResMessage(const ResStruct &s) :
    mRes(s)
{
    setVersion(ParseVersion::PV_LATIN);
}

ResMessage::~ResMessage()
{
}

QString ResMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRes.time.toString("yyyyMMdd hhmmss");

    ret += ")";

    return ret;
}

ParseVersion::Version ResMessage::version() const
{
    return ParseVersion::PV_LATIN;
}

void ResMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mType = TelegramType::RES;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
