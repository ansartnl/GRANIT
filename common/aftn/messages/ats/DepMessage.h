#ifndef DEPMESSAGE_H
#define DEPMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct DepStruct
 * \brief Структура сообщения DEP
 */

struct DepStruct : AtsStruct
{
    QDateTime departureTime;
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const DepStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.departureTime;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, DepStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.departureTime;
    stream >> msg.other;
    return stream;
}

/*!
 * \class DepMessage
 * \brief Класс DEP сообщения
 */

class DepMessage : public AtsMessage
{
public:
    DepMessage();
    DepMessage(const DepStruct &s);
    virtual ~DepMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    DepStruct mDep;

    inline friend QDataStream & operator<< (QDataStream &stream, const DepMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mDep;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, DepMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mDep;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::DepStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(DepStruct)
#endif

#endif // DEPMESSAGE_H
