#ifndef RCFMESSAGE_H
#define RCFMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RcfStruct
 * \brief Структура сообщения RCF
 */

struct RcfStruct
{
    QString acid;

    QString lastContactTime;
    QString lastContactFrequency;
    QString lastReportedPosition;
    QString lastReportedPositionTime;
    QString remainingEquipment;
    QString remarks;

    ParseVersion::Version version;
};

inline QDataStream & operator<< (QDataStream &stream, const RcfStruct &msg)
{
    stream << msg.acid;
    stream << msg.lastContactTime;
    stream << msg.lastContactFrequency;
    stream << msg.lastReportedPosition;
    stream << msg.lastReportedPositionTime;
    stream << msg.remainingEquipment;
    stream << msg.remarks;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RcfStruct &msg)
{
    stream >> msg.acid;
    stream >> msg.lastContactTime;
    stream >> msg.lastContactFrequency;
    stream >> msg.lastReportedPosition;
    stream >> msg.lastReportedPositionTime;
    stream >> msg.remainingEquipment;
    stream >> msg.remarks;
    stream >> msg.version;
    return stream;
}

/*!
 * \class RcfMessage
 * \brief Класс RCF сообщения
 */

class RcfMessage : public AtsMessage
{
public:
    RcfMessage();
    RcfMessage(const RcfStruct &s);
    virtual ~RcfMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    RcfStruct mRcf;

    inline friend QDataStream & operator<< (QDataStream &stream, const RcfMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mRcf;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, RcfMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mRcf;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::RcfStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(RcfStruct)
#endif

#endif // RCFMESSAGE_H
