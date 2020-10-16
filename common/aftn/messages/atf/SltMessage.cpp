#include "SltMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SltMessage::SltMessage()
{
}

SltMessage::SltMessage(const SltStruct &s) :
    AtfFplMessage(s)
  , mDepartureTime(s.departureTime)
  , mSlotTime(s.slotTime)
{
}

SltMessage::~SltMessage()
{
}

QString SltMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mAtfFpl.acid;

    ret += "-";
    ret += mAtfFpl.adep + departureTime().toString("hhmm");

    if (!mAtfFpl.ades.isEmpty()) {
        ret += "-";
        ret += mAtfFpl.ades;
    }

    if (!mAtfFpl.other.isEmpty()) {
        ret += "-";
        ret += mAtfFpl.other;
    }

    ret += ")";

    return ret;
}

const SltStruct SltMessage::slt() const
{
    SltStruct ret(mAtfFpl);

    ret.departureTime = mDepartureTime;
    ret.slotTime = mSlotTime;

    return ret;
}

const QDateTime & SltMessage::departureTime() const
{
    return mDepartureTime;
}

void SltMessage::setDepartureTime(const QDateTime &time)
{
    mDepartureTime = time;
}

const QDateTime & SltMessage::slotTime() const
{
    return mSlotTime;
}

void SltMessage::setSlotTime(const QDateTime &time)
{
    mSlotTime = time;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
