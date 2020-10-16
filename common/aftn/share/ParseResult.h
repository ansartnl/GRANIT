#ifndef PARSERESULT_H
#define PARSERESULT_H

#include "AftnTelegram.h"
#include "AftnMessage.h"

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

typedef QSharedPointer < Telegram > PTelegram;
typedef QSharedPointer < AftnMessage > PMessage;

struct ParseResult
{
    PTelegram telegram;
    QList < PMessage > messageList;
    QDateTime arrivalTime;
    bool hasErrors;

    inline AftnTelegram * aftn() const
    {
        return static_cast < AftnTelegram * >(telegram.data());
    }

    inline const TelegramErrorList errorList() const
    {
        TelegramErrorList ret;
        foreach (const PMessage &m, messageList)
        {
            ret << m->errors();
        }
        return ret;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PARSERESULT_H
