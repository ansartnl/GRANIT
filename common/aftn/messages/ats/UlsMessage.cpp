#include "UlsMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

UlsMessage::UlsMessage()
{
}

UlsMessage::UlsMessage(const UlsStruct &s) :
    mUls(s)
{
    setVersion(s.version);
}

UlsMessage::~UlsMessage()
{
}

QString UlsMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mUls.acid;

#ifdef PATCH1
    if (mUls.hasField9) {
        ret += "-";
        if (mUls.numberOfAircrafts == 1)
            ret += QString("%1/%2").arg(mUls.aircraftType).arg(mUls.turbulence);
        else
            ret += QString("%1%2/%3").arg(mUls.numberOfAircrafts).arg(mUls.aircraftType).arg(mUls.turbulence);
    }
#endif

    ret += "-";
    ret += mUls.adep;

    ret += "-";
    ret += mUls.ades;

    QString other;
    if (mUls.other.isEmpty())
        other = mUls.generateOther();
    else
        other = mUls.other;

    ret += "-";
    ret += other;

    ret += ")";

    return ret;
}

void UlsMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mUls.acid = Transliteration::acidToVersion(mUls.acid, v);
#ifdef PATCH1
    mUls.aircraftType = Transliteration::aircraftTypeToVersion(mUls.aircraftType, v);
    mUls.turbulence = Transliteration::toVersion(mUls.turbulence, v);
#endif
    mUls.adep = Transliteration::toVersion(mUls.adep, v);
    mUls.ades = Transliteration::toVersion(mUls.ades, v);
    mUls.letter = Transliteration::toVersion(mUls.letter, v);
    mUls.dep = Transliteration::toVersion(mUls.dep, v);
    mUls.dest = Transliteration::toVersion(mUls.dest, v);
    mUls.other = Transliteration::toVersion(mUls.other, v);

    mUls.version = v;

    mType = Transliteration::toVersion(TelegramType::ULS, v);
}

FplChange UlsMessage::changes() const
{
    FplChange ret;

    ret.version = mUls.version;

    return ret;
}

AtsStruct UlsMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mUls.acid;
    ret.adep = mUls.adep;
    ret.ades = mUls.ades;
    ret.dof = mUls.dof;

    ret.version = mUls.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
