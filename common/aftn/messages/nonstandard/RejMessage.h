#ifndef REJMESSAGE_H
#define REJMESSAGE_H

#include "FplMessage.h"

#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct RejStruct
 * \brief Структура сообщения REJ
 */

struct RejStruct : FplStruct
{
    QStringList reasons;
};

inline QDataStream & operator<< (QDataStream &stream, const RejStruct &msg)
{
    stream << (const FplStruct &)msg;
    stream << msg.reasons;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, RejStruct &msg)
{
    stream >> (FplStruct &)msg;
    stream >> msg.reasons;
    return stream;
}

/*!
 * \class RejMessage
 * \brief Класс REJ сообщения
 */

class RejMessage : public AtsMessage
{
public:
    RejMessage();
    RejMessage(const RejStruct &s);
    virtual ~RejMessage();

    QString data() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    RejStruct mRej;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // REJMESSAGE_H
