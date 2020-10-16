#ifndef NONSTANDARDRVM_H
#define NONSTANDARDRVM_H

#include "AftnMessage.h"

#include "QDateTime"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct NonStandardRvmStruct
 * \brief Структура сообщения RVM
 */

struct NonStandardRvmStruct
{
    QString id;

    QString type;

    QDateTime beginTime;
    QDateTime endTime;

    QString form;

    QString upperLimit;
    QString lowerLimit;

    QString centerCircleCoordinate;
    QString centerCircleName;
    int radius;

    QMap < int, QString > polygonPointsCoordinate;
    QMap < int, QString > polygonPointsName;

    QString name;

    ParseVersion::Version version;

    NonStandardRvmStruct() :
        radius(0), version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const NonStandardRvmStruct &msg)
{
    stream << msg.id;
    stream << msg.type;
    stream << msg.beginTime;
    stream << msg.endTime;
    stream << msg.form;
    stream << msg.upperLimit;
    stream << msg.lowerLimit;
    stream << msg.centerCircleCoordinate;
    stream << msg.centerCircleName;
    stream << msg.radius;
    stream << msg.polygonPointsCoordinate;
    stream << msg.polygonPointsName;
    stream << msg.name;

    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, NonStandardRvmStruct &msg)
{
    stream >> msg.id;
    stream >> msg.type;
    stream >> msg.beginTime;
    stream >> msg.endTime;
    stream >> msg.form;
    stream >> msg.upperLimit;
    stream >> msg.lowerLimit;
    stream >> msg.centerCircleCoordinate;
    stream >> msg.centerCircleName;
    stream >> msg.radius;
    stream >> msg.polygonPointsCoordinate;
    stream >> msg.polygonPointsName;
    stream >> msg.name;

    return stream;
}

/*!
 * \class NonStandardRvmMessage
 * \brief Класс RVM сообщения
 */

class NonStandardRvmMessage : public AftnMessage
{
public:
    NonStandardRvmMessage();
    NonStandardRvmMessage(const NonStandardRvmStruct &s);
    virtual ~NonStandardRvmMessage();

    ParseVersion::Version version() const;

    QString data() const;
    //QString adexp() const;

    void setVersion(ParseVersion::Version v);

    NonStandardRvmStruct mRvm;

    inline friend QDataStream & operator<< (QDataStream &stream, const NonStandardRvmMessage &msg)
    {
        stream << (const AftnMessage &)msg;
        stream << msg.mRvm;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, NonStandardRvmMessage &msg)
    {
        stream >> (AftnMessage &)msg;
        stream >> msg.mRvm;
        return stream;
    }
};


#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::NonStandardRvmStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(NonStandardRvmStruct)
#endif

#endif // NONSTANDARDRVM_H
