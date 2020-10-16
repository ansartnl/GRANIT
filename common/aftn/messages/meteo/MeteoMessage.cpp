#include "MeteoMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

MeteoMessage::MeteoMessage()
{
}

MeteoMessage::MeteoMessage(const MeteoStruct &s) :
    mMeteo(s)
{
    setVersion(ParseVersion::PV_LATIN);
}

MeteoMessage::~MeteoMessage()
{
}

QString MeteoMessage::data() const
{
    return mMeteo.text;
}

ParseVersion::Version MeteoMessage::version() const
{
    return ParseVersion::PV_LATIN;
}

void MeteoMessage::setVersion(ParseVersion::Version)
{
    mType = mMeteo.type;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
