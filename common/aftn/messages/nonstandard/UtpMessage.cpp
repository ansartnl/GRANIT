#include "UtpMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

UtpMessage::UtpMessage()
{
}

UtpMessage::UtpMessage(const UtpStruct &s) :
    mUtp(s)
{
    setVersion(s.version);
}

UtpMessage::~UtpMessage()
{
}

QString UtpMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mUtp.acid;
    ret += "\r\n";

    ret += "-";
    ret += mUtp.aircraft.join(" ");
    ret += "\r\n";

    ret += "-";
    ret += mUtp.adep + mUtp.departureTime.toString("hhmm");
    ret += "/" + mUtp.endExploration.toString("hhmm");
    ret += " ";
    ret += mUtp.beginFlights.toString("hhmm");
    ret += "/" + mUtp.endFlights.toString("hhmm");
    ret += "\r\n";

    ret += "-";
    ret += mUtp.route;
    ret += "\r\n";

    ret += "-";
    if (!mUtp.alt1.isEmpty())
        ret += mUtp.alt1 + " ";
    if (!mUtp.alt2.isEmpty())
        ret += mUtp.alt2;
    ret += "\r\n";

    ret += "-";
    ret += mUtp.other;
    ret += ")";

    return ret;
}

void UtpMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mUtp.acid = Transliteration::acidToVersion(mUtp.acid, v);

    mUtp.aircraft = Transliteration::toVersion(mUtp.aircraft.join(" "), v).split(" ");

    mUtp.adep = Transliteration::toVersion(mUtp.adep, v);

    mUtp.route = Transliteration::routeToVersion(mUtp.route, v);

    mUtp.ades = Transliteration::toVersion(mUtp.ades, v);
    mUtp.alt1 = Transliteration::toVersion(mUtp.alt1, v);
    mUtp.alt2 = Transliteration::toVersion(mUtp.alt2, v);

    mUtp.other = Transliteration::toVersion(mUtp.other, v);

    mUtp.version = v;

    mType = Transliteration::toVersion(TelegramType::UTP1, v);
}

FplChange UtpMessage::changes() const
{
    FplChange ret;

    return ret;
}

AtsStruct UtpMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mUtp.acid;
    ret.adep = mUtp.adep;
    ret.ades = mUtp.ades;
    ret.dof = mUtp.dof;

    ret.version = mUtp.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
