#ifndef CHGMESSAGE_H
#define CHGMESSAGE_H

#include "AtsMessage.h"

#include "enumstreaming.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct ChgStruct
 * \brief Структура сообщения CHG
 */

struct ChgStruct : AtsStruct
{
    // 18
    QString other;
    // 22
    FplChange changes;
};

inline QDataStream & operator<< (QDataStream &stream, const ChgStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.other;
    stream << msg.changes;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, ChgStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.other;
    stream >> msg.changes;
    return stream;
}

/*!
 * \class ChgMessage
 * \brief Класс CHG сообщения
 */

class ChgMessage : public AtsMessage
{
public:
    ChgMessage();
    ChgMessage(const ChgStruct &s);
    virtual ~ChgMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    ChgStruct mChg;

    inline friend QDataStream & operator<< (QDataStream &stream, const ChgMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mChg;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, ChgMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mChg;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::ChgStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(ChgStruct)
#endif

#endif // CHGMESSAGE_H
