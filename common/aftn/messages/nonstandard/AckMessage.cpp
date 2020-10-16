#include "AckMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AckMessage::AckMessage()
{
}

AckMessage::AckMessage(const AckStruct &s) :
    mAck(s)
{
    setVersion(s.version);
}

AckMessage::~AckMessage()
{
}

QString AckMessage::data() const
{
    QString ret;
    return ret;
}

void AckMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mAck.acid = Transliteration::acidToVersion(mAck.acid, v);
    mAck.adep = Transliteration::toVersion(mAck.adep, v);
    mAck.ades = Transliteration::toVersion(mAck.ades, v);
    mAck.other = Transliteration::toVersion(mAck.other, v);

    mAck.version = v;

    mType = Transliteration::toVersion(TelegramType::ACK, v);
}

FplChange AckMessage::changes() const
{
    FplChange ret;

    ret.version = mAck.version;

    return ret;
}

AtsStruct AckMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mAck.acid;
    ret.adep = mAck.adep;
    ret.ades = mAck.ades;
    ret.dof = mAck.dof;

    ret.version = mAck.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
