#include "NonStandardRvmMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

NonStandardRvmMessage::NonStandardRvmMessage()
{
}

NonStandardRvmMessage::NonStandardRvmMessage(const NonStandardRvmStruct &s) :
    mRvm(s)
{
    setVersion(s.version);
}

NonStandardRvmMessage::~NonStandardRvmMessage()
{
}

ParseVersion::Version NonStandardRvmMessage::version() const
{
    return mRvm.version;
}

QString NonStandardRvmMessage::data() const
{
    QString ret;

    ret += "(";
    ret += type();

    ret += "-";
    ret += mRvm.id;
    ret += "\r\n";

    ret += "-";
    ret += mRvm.type;
    ret += "\r\n";

    ret += "-";
    ret += mRvm.beginTime.toString("yyMMddhhmm");
    ret += mRvm.endTime.toString("yyMMddhhmm");
    ret += "\r\n";

    ret += "-";
    ret += mRvm.form;
    ret += "\r\n";

    if (!mRvm.upperLimit.isEmpty() && !mRvm.lowerLimit.isEmpty()) {
        ret += "-";
        ret += mRvm.upperLimit;
        ret += mRvm.lowerLimit;
        ret += "\r\n";
    }

    ret += "-";
    if (Transliteration::to4444(mRvm.form) == "K") {
        if (!mRvm.centerCircleCoordinate.isEmpty())
            ret += mRvm.centerCircleCoordinate;
        else if (!mRvm.centerCircleName.isEmpty())
            ret += mRvm.centerCircleName;
        ret += "/";
        ret += QString::number(mRvm.radius);
    } else if (Transliteration::to4444(mRvm.form) == "P") {
        if (!mRvm.polygonPointsCoordinate.isEmpty())
            ret += QStringList(mRvm.polygonPointsCoordinate.values()).join(" ");
        if (!mRvm.polygonPointsName.isEmpty())
            ret += QStringList(mRvm.polygonPointsName.values()).join(" ");
    }
    ret += "\r\n";

    ret += "-";
    ret += mRvm.name;
    ret += ")";

    return ret;
}

void NonStandardRvmMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mRvm.id = Transliteration::toVersion(mRvm.id, v);

    mRvm.type =  Transliteration::toVersion(mRvm.type, v);

    mRvm.form =  Transliteration::toVersion(mRvm.form, v);

    mRvm.upperLimit = Transliteration::toVersion(mRvm.upperLimit, v);
    mRvm.lowerLimit = Transliteration::toVersion(mRvm.lowerLimit, v);

    mRvm.centerCircleCoordinate = Transliteration::toVersion(mRvm.centerCircleCoordinate, v);
    mRvm.centerCircleName = Transliteration::toVersion(mRvm.centerCircleName, v);

    foreach (const int  &arg, mRvm.polygonPointsCoordinate.keys()) {
        QString value = Transliteration::toVersion(mRvm.polygonPointsCoordinate.value(arg), v);
        mRvm.polygonPointsCoordinate.remove(arg);
        mRvm.polygonPointsCoordinate.insert(arg, value);
    }

    foreach (const int  &arg, mRvm.polygonPointsName.keys()) {
        QString value = Transliteration::toVersion(mRvm.polygonPointsName.value(arg), v);
        mRvm.polygonPointsName.remove(arg);
        mRvm.polygonPointsName.insert(arg, value);
    }

    mRvm.name = Transliteration::toVersion(mRvm.name, v);

    mType = Transliteration::toVersion(TelegramType::RVM, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
