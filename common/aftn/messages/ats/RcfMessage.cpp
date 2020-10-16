#include "RcfMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RcfMessage::RcfMessage()
{
}

RcfMessage::RcfMessage(const RcfStruct &s) :
    mRcf(s)
{
    setVersion(s.version);
}

RcfMessage::~RcfMessage()
{
}

QString RcfMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRcf.acid;

    ret += "-";
    ret += mRcf.lastContactTime;
    ret += " ";
    ret += mRcf.lastContactFrequency;
    ret += " ";
    ret += mRcf.lastReportedPosition;
    ret += " ";
    ret += mRcf.lastReportedPositionTime;
    ret += " ";
    ret += mRcf.remainingEquipment;
    ret += " ";
    ret += mRcf.remarks;

    ret += ")";

    return ret;
}

QString RcfMessage::adexp() const
{
    QString ret;

    return ret;
}

void RcfMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mRcf.acid = Transliteration::acidToVersion(mRcf.acid, v);
    mRcf.lastContactTime = Transliteration::rfiToVersion(mRcf.lastContactTime, v);
    mRcf.lastContactFrequency = Transliteration::rfiToVersion(mRcf.lastContactFrequency, v);
    mRcf.lastReportedPosition = Transliteration::rfiToVersion(mRcf.lastReportedPosition, v);
    mRcf.lastReportedPositionTime = Transliteration::rfiToVersion(mRcf.lastReportedPositionTime, v);
    mRcf.remainingEquipment = Transliteration::rfiToVersion(mRcf.remainingEquipment, v);
    mRcf.remarks = Transliteration::rfiToVersion(mRcf.remarks, v);

    mRcf.version = v;

    mType = Transliteration::toVersion(TelegramType::RCF, v);
}

FplChange RcfMessage::changes() const
{
    FplChange ret;

    ret.version = mRcf.version;

    return ret;
}

AtsStruct RcfMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRcf.acid;

    ret.version = mRcf.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
