#ifndef SVCLRLSMESSAGE_H
#define SVCLRLSMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcLrLsStruct
 * \brief Структура SVC/СЖЦ сообщения LR LS/ЛР ЛС
 */

struct SvcLrLsStruct
{
    QString received;
    QString sent;

    ParseVersion::Version version;

    SvcLrLsStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}

    inline const QString receivedAftnChannel() const
    {
        return received.left(3);
    }
    inline int receivedTelegramNumber() const
    {
        return received.right(3).toInt();
    }
    inline const QString sentAftnChannel() const
    {
        return sent.left(3);
    }
    inline int sentTelegramNumber() const
    {
        return sent.right(3).toInt();
    }
};

/*!
 * \class SvcLrLsMessage
 * \brief Класс SVC/СЖЦ сообщения LR LS/ЛР ЛС
 */

class SvcLrLsMessage : public AftnMessage
{
public:
    SvcLrLsMessage();
    SvcLrLsMessage(const SvcLrLsStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcLrLsStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCLRLSMESSAGE_H
