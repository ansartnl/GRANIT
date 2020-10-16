#include "DlaMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

DlaMessage::DlaMessage()
{
}

DlaMessage::DlaMessage(const DlaStruct &s) :
    mDla(s)
{
    setVersion(s.version);
}

DlaMessage::~DlaMessage()
{
}

QString DlaMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mDla.acid;

    ret += "-";
    ret += mDla.adep + mDla.departureTime.toString("hhmm");

    if (!mDla.ades.isEmpty()) {
        ret += "-";
        ret += mDla.ades;
    }

    if (!mDla.other.isEmpty()) {
        ret += "-";
        ret += mDla.other;
    }

    ret += ")";

    return ret;
}

QString DlaMessage::adexp() const
{
    QString ret;

    // 3 field
    ret += "-TITLE IDLA";
    // 7 field
    ret += " -ARCID " + mDla.acid;
    // 13 field
    ret += " -ADEP " + mDla.adep;
    ret += " -EOBT " + mDla.departureTime.toString("hhmm");
    // 16 field
    if (!mDla.ades.isEmpty())
        ret += " -ADES " + mDla.ades;
    // 18 field
    if (!mDla.other.isEmpty())
        ret += otherToAdexp(mDla.other);

    return ret;
}

void DlaMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mDla.acid = Transliteration::acidToVersion(mDla.acid, v);
    mDla.adep = Transliteration::toVersion(mDla.adep, v);
    mDla.ades = Transliteration::toVersion(mDla.ades, v);
    mDla.other = Transliteration::toVersion(mDla.other, v);

    mDla.version = v;

    mType = Transliteration::toVersion(TelegramType::DLA, v);
}

FplChange DlaMessage::changes() const
{
    FplChange ret;

    ret.departureTime = mDla.departureTime;
    ret.exFpl = FplChange::DELAYED;
    ret.boardNumber = mDla.boardNumber;

    ret.version = mDla.version;

    ret.changesFlags = FplChange::Changes(FplChange::DEPARTURE_TIME | FplChange::EX_FPL);
    if (!ret.boardNumber.isEmpty())
        ret.changesFlags = FplChange::Changes(ret.changesFlags | FplChange::BOARD_NUMBER);

    return ret;
}

AtsStruct DlaMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mDla.acid;
    ret.adep = mDla.adep;
    ret.ades = mDla.ades;
    ret.dof = mDla.departureTime.date();

    ret.version = mDla.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
