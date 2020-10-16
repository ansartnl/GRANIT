#ifndef SVCLREXPMESSAGE_H
#define SVCLREXPMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcLrExpStruct
 * \brief Структура SVC/СЖЦ сообщения LR EXP/ПОЛУЧЕН ОЖИДАЛСЯ
 */

struct SvcLrExpStruct
{
    QString received;
    QString expected;

    ParseVersion::Version version;

    SvcLrExpStruct() :
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
    inline const QString expectedAftnChannel() const
    {
        return expected.left(3);
    }
    inline int expectedTelegramNumber() const
    {
        return expected.right(3).toInt();
    }
};

/*!
 * \class SvcLrExpMessage
 * \brief Класс SVC/СЖЦ сообщения LR EXP/ПОЛУЧЕН ОЖИДАЛСЯ
 */

class SvcLrExpMessage : public AftnMessage
{
public:
    SvcLrExpMessage();
    SvcLrExpMessage(const SvcLrExpStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcLrExpStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCLREXPMESSAGE_H
