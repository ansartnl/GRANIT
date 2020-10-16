#ifndef CNLMESSAGE_H
#define CNLMESSAGE_H

#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct CnlStruct
 * \brief Структура сообщения CNL
 */

struct CnlStruct : AtsStruct
{
    QDateTime departureTime;
    QString other;
};

inline QDataStream & operator<< (QDataStream &stream, const CnlStruct &msg)
{
    stream << (const AtsStruct &)msg;
    stream << msg.departureTime;
    stream << msg.other;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, CnlStruct &msg)
{
    stream >> (AtsStruct &)msg;
    stream >> msg.departureTime;
    stream >> msg.other;
    return stream;
}

/*!
 * \class CnlMessage
 * \brief Класс CNL сообщения
 */

class CnlMessage : public AtsMessage
{
public:
    CnlMessage();
    CnlMessage(const CnlStruct &s);
    virtual ~CnlMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    CnlStruct mCnl;

    inline friend QDataStream & operator<< (QDataStream &stream, const CnlMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mCnl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, CnlMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mCnl;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::CnlStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(CnlStruct)
#endif

#endif // CNLMESSAGE_H
