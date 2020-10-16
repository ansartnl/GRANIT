#include "PpnMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

PpnMessage::PpnMessage()
{
}

PpnMessage::PpnMessage(const PpnStruct &s) :
    mPpn(s)
{
    setVersion(s.version);
}

PpnMessage::~PpnMessage()
{
}

QString PpnMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mPpn.acid;

    if (!mPpn.other.isEmpty()) {
        ret += "-";
        ret += mPpn.other;
    } else {
        ret += Transliteration::toVersion("-DOF/", mPpn.version);
        ret += mPpn.dof.toString("yyMMdd");
    }

    ret += ")";

    return ret;
}

void PpnMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mPpn.acid = Transliteration::acidToVersion(mPpn.acid, v);

    mPpn.version = v;

    mType = Transliteration::toVersion(TelegramType::PPN, v);
}

FplChange PpnMessage::changes() const
{
    FplChange ret;

    ret.version = mPpn.version;

    return ret;
}

AtsStruct PpnMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mPpn.acid;
    ret.dof = mPpn.dof;

    ret.version = mPpn.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
