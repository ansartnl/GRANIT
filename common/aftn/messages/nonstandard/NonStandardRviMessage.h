#ifndef NONSTANDARDRiM_H
#define NONSTANDARDRiM_H

#include "AftnMessage.h"

#include "QDateTime"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct NonStandardRviStruct
 * \brief Структура сообщения RVI
 */

struct NonStandardRviStruct
{
    QString id;

    ParseVersion::Version version;

    NonStandardRviStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const NonStandardRviStruct &msg)
{
    stream << msg.id;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, NonStandardRviStruct &msg)
{
    stream >> msg.id;
    return stream;
}

/*!
 * \class NonStandardRviMessage
 * \brief Класс RVI сообщения
 */

class NonStandardRviMessage : public AftnMessage
{
public:
    NonStandardRviMessage();
    NonStandardRviMessage(const NonStandardRviStruct &s);
    virtual ~NonStandardRviMessage();

    ParseVersion::Version version() const;

    QString data() const;
    //QString adexp() const;

    void setVersion(ParseVersion::Version v);

    NonStandardRviStruct mRvi;

    inline friend QDataStream & operator<< (QDataStream &stream, const NonStandardRviMessage &msg)
    {
        stream << (const AftnMessage &)msg;
        stream << msg.mRvi;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, NonStandardRviMessage &msg)
    {
        stream >> (AftnMessage &)msg;
        stream >> msg.mRvi;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::NonStandardRviStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(NonStandardRviStruct)
#endif

#endif // NONSTANDARDRVI_H
