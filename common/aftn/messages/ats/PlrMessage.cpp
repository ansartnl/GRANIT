#include "PlrMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

const QList < PlrPoint > PlrStruct::flightPoints() const
{
    QList < PlrPoint > ret;
    QRegExp fieldRexp("POINTS/");
    int index = fieldRexp.indexIn(other);
    if (index == -1)
        return ret;

    int offset = index + fieldRexp.matchedLength();

    QRegExp pointRexp("((\\d{4}[NS]\\d{5}[EW])|([A-Z]+))"
                      "/(\\d{2})(\\d{2})"
                      "(/([KN]\\d{4}|[M]\\d{3})([FA]\\d{3}|[SM]\\d{4}|VFR))?(\\s*)");
    while ((offset = pointRexp.indexIn(other, offset)) != -1) {
        PlrPoint p;
        p.point = pointRexp.cap(1);
        p.time = QTime(pointRexp.cap(4).toInt(), pointRexp.cap(5).toInt());
        p.speed = pointRexp.cap(7);
        p.flightLevel = pointRexp.cap(8);
        ret << p;
        offset += pointRexp.matchedLength();
    }

    return ret;
}

PlrMessage::PlrMessage()
{
}

PlrMessage::PlrMessage(const PlrStruct &s) :
    mPlr(s)
{
    setVersion(s.version);
}

PlrMessage::~PlrMessage()
{
}
int mPlr;
QString PlrMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mPlr.acid;

    ret += "-";
    ret += mPlr.adep + mPlr.departureTime.toString("hhmm");

    if (!mPlr.ades.isEmpty()) {
        ret += "-";
        ret += mPlr.ades;
    }

    if (!mPlr.other.isEmpty()) {
        ret += "-";
        ret += mPlr.other;
    }

    ret += ")";

    return ret;
}

QString PlrMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IPLR";
    // 7 field
    ret += " -ARCID " + mPlr.acid;
    // 13 field
    ret += " -ADEP " + mPlr.adep;
    ret += " -EOBT " + mPlr.departureTime.toString("hhmm");
    // 16 field
    if (!mPlr.ades.isEmpty())
        ret += " -ADES " + mPlr.ades;
    // 18 field
    if (!mPlr.other.isEmpty())
        ret += otherToAdexp(mPlr.other);

    return ret;
}

void PlrMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mPlr.acid = Transliteration::acidToVersion(mPlr.acid, v);
    mPlr.adep = Transliteration::toVersion(mPlr.adep, v);
    mPlr.ades = Transliteration::toVersion(mPlr.ades, v);
    mPlr.other = Transliteration::toVersion(mPlr.other, v);

    mPlr.version = v;

    mType = Transliteration::toVersion(TelegramType::PLR, v);
}

FplChange PlrMessage::changes() const
{
    FplChange ret;

    ret.departureTime = mPlr.departureTime;
    ret.exFpl = FplChange::DELAYED;

    ret.version = mPlr.version;

    ret.changesFlags = FplChange::Changes(FplChange::DEPARTURE_TIME | FplChange::EX_FPL);

    return ret;
}

AtsStruct PlrMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mPlr.acid;
    ret.adep = mPlr.adep;
    ret.ades = mPlr.ades;
    ret.dof = mPlr.departureTime.date();

    ret.version = mPlr.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
