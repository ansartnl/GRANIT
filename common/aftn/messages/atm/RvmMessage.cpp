#include "RvmMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RvmMessage::RvmMessage()
{
}

RvmMessage::RvmMessage(const RvmStruct &s) :
    mRvm(s)
{
    setVersion(s.version);
}

RvmMessage::~RvmMessage()
{
}

QString RvmMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    if (!mRvm.other.isEmpty()) {
        ret += "-";
        ret += mRvm.other;
    } else {
        QDate dof = mRvm.dof;
        if (!dof.isValid())
            dof = QDateTime::currentDateTimeUtc().date();
        if (mRvm.number.isEmpty())
            ret += Transliteration::toVersion(QString("-DOF/%1")
                                           .arg(dof.toString("yyMMdd"))
                                           , mRvm.version);
        else
            ret += Transliteration::toVersion(QString("-%1 DOF/%2")
                                           .arg(mRvm.number)
                                           .arg(dof.toString("yyMMdd"))
                                           , mRvm.version);
    }

    ret += ")";

    return ret;
}

void RvmMessage::setVersion(ParseVersion::Version v)
{
    mRvm.type = Transliteration::toVersion(TelegramType::RVM, v);

    AtmMessage::setVersion(v);

    mRvm.number = Transliteration::toVersion(mRvm.number, v);
    mRvm.other = Transliteration::toVersion(mRvm.other, v);
}

AtmStruct & RvmMessage::atm()
{
    return (AtmStruct &)mRvm;
}

const AtmStruct & RvmMessage::atmConst() const
{
    return (const AtmStruct &)mRvm;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
