#include "AnpMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AnpMessage::AnpMessage()
{
}

AnpMessage::AnpMessage(const AnpStruct &s) :
    mAnp(s)
{
    setVersion(s.version);
}

AnpMessage::~AnpMessage()
{
}

QString AnpMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAnp.acid;

    ret += "-";
    if (mAnp.numberOfAircrafts == 1)
        ret += QString("%1").arg(mAnp.aircraftType);
    else
        ret += QString("%1%2").arg(mAnp.numberOfAircrafts).arg(mAnp.aircraftType);

    if (!mAnp.other.isEmpty()) {
        ret += "-";
        ret += mAnp.other;
    } else {
        if (mAnp.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mAnp.dof.toString("yyMMdd"))
                                              , mAnp.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mAnp.version);
    }

    ret += ")";

    return ret;
}

void AnpMessage::setVersion(ParseVersion::Version v)
{
    mAnp.type = Transliteration::toVersion(TelegramType::ANP, v);

    AtmMessage::setVersion(v);

    mAnp.acid = Transliteration::acidToVersion(mAnp.acid, v);
    mAnp.aircraftType = Transliteration::aircraftTypeToVersion(mAnp.aircraftType, v);
    mAnp.other = Transliteration::toVersion(mAnp.other, v);
}

AtmStruct & AnpMessage::atm()
{
    return (AtmStruct &)mAnp;
}

const AtmStruct & AnpMessage::atmConst() const
{
    return (const AtmStruct &)mAnp;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
