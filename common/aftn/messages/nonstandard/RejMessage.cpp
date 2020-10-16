#include "RejMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RejMessage::RejMessage()
{
}

RejMessage::RejMessage(const RejStruct &s) :
    mRej(s)
{
    setVersion(s.version);
}

RejMessage::~RejMessage()
{
}

QString RejMessage::data() const
{
    QString ret;
    return ret;
}

void RejMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mRej.acid = Transliteration::acidToVersion(mRej.acid, v);
    mRej.flightRules = Transliteration::toVersion(mRej.flightRules, v);
    mRej.typeOfFlight = Transliteration::toVersion(mRej.typeOfFlight, v);

    mRej.aircraftType = Transliteration::aircraftTypeToVersion(mRej.aircraftType, v);
    mRej.turbulence = Transliteration::toVersion(mRej.turbulence, v);

    mRej.equipment = Transliteration::toVersion(mRej.equipment, v);

    mRej.adep = Transliteration::toVersion(mRej.adep, v);

    mRej.speed = Transliteration::toVersion(mRej.speed, v);
    mRej.flightLevel = Transliteration::toVersion(mRej.flightLevel, v);
    mRej.route = Transliteration::routeToVersion(mRej.route, v);

    mRej.ades = Transliteration::toVersion(mRej.ades, v);
    mRej.alt1 = Transliteration::toVersion(mRej.alt1, v);
    mRej.alt2 = Transliteration::toVersion(mRej.alt2, v);
    QStringList alt = mRej.otherAlternative;
    mRej.otherAlternative.clear();
    foreach (const QString &a, alt)
        mRej.otherAlternative << Transliteration::toVersion(a, v);

    mRej.other = Transliteration::toVersion(mRej.other, v);

    QStringList newReasons;
    foreach (const QString &r, mRej.reasons)
        newReasons << Transliteration::toVersion(r, v);
    mRej.reasons = newReasons;

    mRej.version = v;

    mType = Transliteration::toVersion(TelegramType::REJ, v);
}

FplChange RejMessage::changes() const
{
    FplChange ret;

    ret.version = mRej.version;

    return ret;
}

AtsStruct RejMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mRej.acid;
    ret.adep = mRej.adep;
    ret.ades = mRej.ades;
    ret.dof = mRej.departureTime.date();

    ret.version = mRej.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
