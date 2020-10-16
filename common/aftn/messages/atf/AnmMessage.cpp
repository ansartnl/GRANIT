#include "AnmMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AnmMessage::AnmMessage()
{
}

AnmMessage::AnmMessage(const AnmStruct &s) :
    mAnm(s)
{
    setVersion(s.version);
}

AnmMessage::~AnmMessage()
{
}

QString AnmMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mAnm.other.isEmpty()) {
        ret += "-";
        ret += mAnm.other;
    } else {
        if (mAnm.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mAnm.dof.toString("yyMMdd"))
                                              , mAnm.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mAnm.version);
    }

    ret += ")";

    return ret;
}

void AnmMessage::setVersion(ParseVersion::Version v)
{
    AtfMessage::setVersion(v);

    mAnm.other = Transliteration::toVersion(mAnm.other, v);
}

AtfStruct & AnmMessage::atf()
{
    return (AtfStruct &)mAnm;
}

const AtfStruct & AnmMessage::atfConst() const
{
    return (const AtfStruct &)mAnm;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
