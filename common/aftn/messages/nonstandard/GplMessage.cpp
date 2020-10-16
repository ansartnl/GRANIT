#include "GplMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

GplMessage::GplMessage()
{
}

GplMessage::GplMessage(const GplStruct &s) :
    mGpl(s)
{
    setVersion(ParseVersion::PV_LATIN);
}

GplMessage::~GplMessage()
{
}

QString GplMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mGpl.start.toString("yyMMdd");

    ret += "-";
    ret += mGpl.finish.toString("yyMMdd");

    ret += ")";

    return ret;
}

ParseVersion::Version GplMessage::version() const
{
    return ParseVersion::PV_LATIN;
}

void GplMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mType = TelegramType::GPL;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
