#include "ApwMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ApwMessage::ApwMessage()
{
}

ApwMessage::ApwMessage(const ApwStruct &s) :
    mApw(s)
{
    setVersion(s.version);
}

ApwMessage::~ApwMessage()
{
}

QString ApwMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mApw.other.isEmpty()) {
        ret += "-";
        ret += mApw.other;
    } else {
        QDate dof = mApw.dof;
        if (!dof.isValid())
            dof = QDateTime::currentDateTimeUtc().date();
        if (!mApw.airport.isEmpty())
            ret += Transliteration::toVersion(QString("-%1%2 DOF/%3")
                                           .arg(mApw.airport)
                                           .arg(mApw.validFrom.toString("hhmm"))
                                           .arg(dof.toString("yyMMdd"))
                                           , mApw.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                           .arg(dof.toString("yyMMdd"))
                                           , mApw.version);
    }

    ret += ")";

    return ret;
}

void ApwMessage::setVersion(ParseVersion::Version v)
{
    mApw.type = Transliteration::toVersion(TelegramType::APW, v);

    AtmMessage::setVersion(v);

    mApw.other = Transliteration::toVersion(mApw.other, v);
    mApw.airport = Transliteration::toVersion(mApw.airport, v);
}

AtmStruct & ApwMessage::atm()
{
    return (AtmStruct &)mApw;
}

const AtmStruct & ApwMessage::atmConst() const
{
    return (const AtmStruct &)mApw;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
