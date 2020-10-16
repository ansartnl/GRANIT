#include "AimMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AimMessage::AimMessage()
{
}

AimMessage::AimMessage(const AimStruct &s) :
    mAim(s)
{
    setVersion(s.version);
}

AimMessage::~AimMessage()
{
}

QString AimMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mAim.other.isEmpty()) {
        ret += "-";
        ret += mAim.other;
    } else {
        if (mAim.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mAim.dof.toString("yyMMdd"))
                                              , mAim.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mAim.version);
    }

    ret += ")";

    return ret;
}

void AimMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mAim.other = Transliteration::toVersion(mAim.other, v);
}

AtfStruct & AimMessage::atf()
{
    return (AtfStruct &)mAim;
}

const AtfStruct & AimMessage::atfConst() const
{
    return (const AtfStruct &)mAim;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
