#include "AlrMessageShort.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

AlrMessageShort::AlrMessageShort()
{
}

AlrMessageShort::AlrMessageShort(const AlrStructShort &s) :
    mAlr(s)
{
    setVersion(s.version);
}

AlrMessageShort::~AlrMessageShort()
{
}

QString AlrMessageShort::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAlr.acid;

    ret += "-";
    if (mAlr.numberOfAircrafts == 1)
        ret += QString("%1").arg(mAlr.aircraftType);
    else
        ret += QString("%1%2").arg(mAlr.numberOfAircrafts).arg(mAlr.aircraftType);

    if (!mAlr.other.isEmpty()) {
        ret += "-";
        ret += mAlr.other;
    } else {
        if (mAlr.dof.isValid())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(mAlr.dof.toString("yyMMdd"))
                                              , mAlr.version);
        else
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                              .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                              , mAlr.version);
    }

    ret += ")";

    return ret;
}

void AlrMessageShort::setVersion(ParseVersion::Version v)
{
    mAlr.type = Transliteration::toVersion(TelegramType::ALR, v);

    AtmMessage::setVersion(v);

    mAlr.acid = Transliteration::acidToVersion(mAlr.acid, v);
    mAlr.aircraftType = Transliteration::aircraftTypeToVersion(mAlr.aircraftType, v);
    mAlr.other = Transliteration::toVersion(mAlr.other, v);
}

AtmStruct & AlrMessageShort::atm()
{
    return (AtmStruct &)mAlr;
}

const AtmStruct & AlrMessageShort::atmConst() const
{
    return (const AtmStruct &)mAlr;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
