#include "ManMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ManMessage::ManMessage()
{

}

ManMessage::ManMessage(const ManStruct &s) :
    mMan(s)
{
    setVersion(s.version);
}

ManMessage::~ManMessage()
{
}

QString ManMessage::data() const
{
    QString ret;
    return ret;
}

void ManMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mMan.acid = Transliteration::acidToVersion(mMan.acid, v);
    mMan.flightRules = Transliteration::toVersion(mMan.flightRules, v);
    mMan.typeOfFlight = Transliteration::toVersion(mMan.typeOfFlight, v);

    mMan.aircraftType = Transliteration::aircraftTypeToVersion(mMan.aircraftType, v);
    mMan.turbulence = Transliteration::toVersion(mMan.turbulence, v);

    mMan.equipment = Transliteration::toVersion(mMan.equipment, v);

    mMan.adep = Transliteration::toVersion(mMan.adep, v);

    mMan.speed = Transliteration::toVersion(mMan.speed, v);
    mMan.flightLevel = Transliteration::toVersion(mMan.flightLevel, v);
    mMan.route = Transliteration::routeToVersion(mMan.route, v);

    mMan.ades = Transliteration::toVersion(mMan.ades, v);
    mMan.alt1 = Transliteration::toVersion(mMan.alt1, v);
    mMan.alt2 = Transliteration::toVersion(mMan.alt2, v);
    QStringList alt = mMan.otherAlternative;
    mMan.otherAlternative.clear();
    foreach (const QString &a, alt)
        mMan.otherAlternative << Transliteration::toVersion(a, v);

    mMan.other = Transliteration::toVersion(mMan.other, v);

    QStringList newReasons;
    foreach (const QString &r, mMan.reasons)
        newReasons << Transliteration::toVersion(r, v);
    mMan.reasons = newReasons;

    mMan.version = v;

    mType = TelegramType::MAN;
}

FplChange ManMessage::changes() const
{
    FplChange ret;

    ret.version = mMan.version;

    return ret;
}

AtsStruct ManMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mMan.acid;
    ret.adep = mMan.adep;
    ret.ades = mMan.ades;
    ret.dof = mMan.departureTime.date();

    ret.version = mMan.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
