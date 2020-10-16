#include "RviMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RviMessage::RviMessage()
{
}

RviMessage::RviMessage(const RviStruct &s) :
    mRvi(s)
{
    setVersion(s.version);
}

RviMessage::~RviMessage()
{
}

QString RviMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mRvi.other.isEmpty()) {
        ret += "-";
        ret += mRvi.other;
    } else {
        QDate dof = mRvi.dof;
        if (!dof.isValid())
            dof = QDateTime::currentDateTimeUtc().date();
        if (mRvi.number.isEmpty())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                           .arg(dof.toString("yyMMdd"))
                                           , mRvi.version);
        else
            ret += Transliteration::toVersion(QString("-%1 DOF/%2")
                                           .arg(mRvi.number)
                                           .arg(dof.toString("yyMMdd"))
                                           , mRvi.version);
    }

    ret += ")";

    return ret;
}

void RviMessage::setVersion(ParseVersion::Version v)
{
    mRvi.type = Transliteration::toVersion(TelegramType::RVI, v);

    AtmMessage::setVersion(v);

    mRvi.number = Transliteration::toVersion(mRvi.number, v);
    mRvi.other = Transliteration::toVersion(mRvi.other, v);
}

AtmStruct & RviMessage::atm()
{
    return (AtmStruct &)mRvi;
}

const AtmStruct & RviMessage::atmConst() const
{
    return (const AtmStruct &)mRvi;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
