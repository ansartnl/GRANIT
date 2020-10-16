#include "ApzMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ApzMessage::ApzMessage()
{
}

ApzMessage::ApzMessage(const ApzStruct &s) :
    mApz(s)
{
    setVersion(s.version);
}

ApzMessage::~ApzMessage()
{
}

QString ApzMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mApz.other.isEmpty()) {
        ret += "-";
        ret += mApz.other;
    } else {
        QDate dof = mApz.dof;
        if (!dof.isValid())
            dof = QDateTime::currentDateTimeUtc().date();
        if (!mApz.airport.isEmpty())
            ret += Transliteration::toVersion(QString("-%1%2/%3 DOF/%4")
                                              .arg(mApz.airport)
                                              .arg(mApz.validFrom.toString("hhmm"))
                                              .arg(mApz.validTill.toString("hhmm"))
                                              .arg(dof.toString("yyMMdd"))
                                              , mApz.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(dof.toString("yyMMdd"))
                                              , mApz.version);
    }

    ret += ")";

    return ret;
}

void ApzMessage::setVersion(ParseVersion::Version v)
{
    mApz.type = Transliteration::toVersion(TelegramType::APZ, v);

    AtmMessage::setVersion(v);

    mApz.other = Transliteration::toVersion(mApz.other, v);
    mApz.airport = Transliteration::toVersion(mApz.airport, v);
}

AtmStruct & ApzMessage::atm()
{
    return (AtmStruct &)mApz;
}

const AtmStruct & ApzMessage::atmConst() const
{
    return (const AtmStruct &)mApz;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
