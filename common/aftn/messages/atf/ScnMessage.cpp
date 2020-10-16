#include "ScnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ScnMessage::ScnMessage()
{
}

ScnMessage::ScnMessage(const ScnStruct &s) :
    mScn(s)
{
    setVersion(s.version);
}

ScnMessage::~ScnMessage()
{
}

QString ScnMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mScn.other.isEmpty()) {
        ret += "-";
        ret += mScn.other;
    } else {
        if (mScn.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                           .arg(mScn.dof.toString("yyMMdd"))
                                           , mScn.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                           .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                           , mScn.version);
    }

    ret += ")";

    return ret;
}

void ScnMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mScn.other = Transliteration::toVersion(mScn.other, v);
}

AtfStruct & ScnMessage::atf()
{
    return (AtfStruct &)mScn;
}

const AtfStruct & ScnMessage::atfConst() const
{
    return (const AtfStruct &)mScn;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
