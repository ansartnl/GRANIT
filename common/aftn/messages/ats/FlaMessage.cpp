#include "FlaMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

FlaMessage::FlaMessage()
{
}

FlaMessage::FlaMessage(const FlaStruct &s) :
    mFla(s)
{
    setVersion(s.version);
}

FlaMessage::~FlaMessage()
{
}

QString FlaMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mFla.acid;

    ret += "-";
    ret += mFla.ades;

    ret += "-";
    ret += mFla.arrivalAirport;

    QString other;
    if (mFla.other.isEmpty()) {
        if (mFla.dof.isValid())
            other = Transliteration::toVersion(QString("DOF/%1")
                                            .arg(mFla.dof.toString("yyMMdd"))
                                            , mFla.version);
        else
            other = Transliteration::toVersion(QString("DOF/%1")
                                            .arg(QDateTime::currentDateTimeUtc().date().toString("yyMMdd"))
                                            , mFla.version);
    } else {
        other = mFla.other;
    }

    ret += "-";
    ret += other;

    ret += ")";

    return ret;
}

void FlaMessage::setVersion(ParseVersion::Version v)
{
    AtsMessage::setVersion(v);

    mFla.acid = Transliteration::acidToVersion(mFla.acid, v);
    mFla.ades = Transliteration::toVersion(mFla.ades, v);
    mFla.arrivalAirport = Transliteration::toVersion(mFla.arrivalAirport, v);
    mFla.other = Transliteration::toVersion(mFla.other, v);

    mFla.version = v;

    mType = Transliteration::toVersion(TelegramType::FLA1, v);
}

FplChange FlaMessage::changes() const
{
    FplChange ret;

    ret.exFpl = FplChange::ALTERNATIVE;

    ret.version = mFla.version;

    ret.changesFlags = FplChange::Changes(FplChange::EX_FPL);

    return ret;
}

AtsStruct FlaMessage::ats() const
{
    AtsStruct ret;

    ret.acid = mFla.acid;
    ret.ades = mFla.ades;
    ret.dof = mFla.dof;

    ret.version = mFla.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
