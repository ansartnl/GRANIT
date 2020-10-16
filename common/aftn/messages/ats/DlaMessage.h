#ifndef DLAMESSAGE_H
#define DLAMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct DlaStruct
 * \brief Структура сообщения DLA
 */

struct DlaStruct : AtsStruct
{
    QDateTime departureTime;
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const DlaStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.departureTime;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, DlaStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.departureTime;
    stream >> msg.other;
    return stream;
}

/*!
 * \class DlaMessage
 * \brief Класс DLA сообщения
 */

class DlaMessage : public AtsMessage
{
public:
    DlaMessage();
    DlaMessage(const DlaStruct &s);
    virtual ~DlaMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    DlaStruct mDla;

    inline friend QDataStream & operator<< (QDataStream &stream, const DlaMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mDla;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, DlaMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mDla;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::DlaStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(DlaStruct)
#endif

#endif // DLAMESSAGE_H
