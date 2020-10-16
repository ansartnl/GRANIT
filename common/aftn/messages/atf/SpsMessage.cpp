#include "SpsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SpsMessage::SpsMessage()
{
}

SpsMessage::SpsMessage(const SpsStruct &s) :
    mSps(s)
{
    setVersion(s.version);
}

SpsMessage::~SpsMessage()
{
}

QString SpsMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mSps.other.isEmpty()) {
        ret += "-";
        ret += mSps.other;
    } else {
        if (mSps.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mSps.dof.toString("yyMMdd"))
                                              , mSps.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mSps.version);
    }

    ret += ")";

    return ret;
}

void SpsMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mSps.other = Transliteration::toVersion(mSps.other, v);
}

AtfStruct & SpsMessage::atf()
{
    return (AtfStruct &)mSps;
}

const AtfStruct & SpsMessage::atfConst() const
{
    return (const AtfStruct &)mSps;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
