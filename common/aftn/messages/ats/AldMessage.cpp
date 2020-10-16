#include "AldMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AldMessage::AldMessage()
{
}

AldMessage::AldMessage(const AldStruct &s) :
    mAld(s)
{
    setVersion(s.version);
}

AldMessage::~AldMessage()
{
}

QString AldMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAld.acid;

    ret += "-";
    if (mAld.numberOfAircrafts == 1)
        ret += QString("%1/%2").arg(mAld.aircraftType).arg(mAld.turbulence);
    else
        ret += QString("%1%2/%3").arg(mAld.numberOfAircrafts).arg(mAld.aircraftType).arg(mAld.turbulence);

    if (!mAld.other.isEmpty()) {
        ret += "-";
        ret += mAld.other;
    } else {
        if (mAld.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mAld.dof.toString("yyMMdd"))
                                              , mAld.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mAld.version);
    }

    ret += ")";

    return ret;
}

QString AldMessage::adexp() const
{
    QString ret;

    return ret;
}

void AldMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mAld.acid = Transliteration::acidToVersion(mAld.acid, v);
    mAld.aircraftType = Transliteration::aircraftTypeToVersion(mAld.aircraftType, v);
    mAld.turbulence = Transliteration::toVersion(mAld.turbulence, v);
    mAld.other = Transliteration::toVersion(mAld.other, v);

    mAld.version = v;

    mType = Transliteration::toVersion(TelegramType::ALD, v);
}

FplChange AldMessage::changes() const
{
    FplChange ret;

    ret.version = mAld.version;

    return ret;
}

AtsStruct AldMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mAld.acid;
    ret.dof = mAld.dof;

    ret.version = mAld.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
