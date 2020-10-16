#ifndef TELEGRAM_H
#define TELEGRAM_H

#include "ParseVersion.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \class Telegram
 * \brief Базовый класс входящих телеграмм
 *
 * Данный класс является базовым для всех входящих сообщений.
 */

class Telegram
{
public:
    Telegram();
    virtual ~Telegram();

    virtual QString data() const = 0;
    QString type() const;

    virtual ParseVersion::Version version() const;
    virtual void setVersion(ParseVersion::Version v);

    virtual const QString aftnChannel() const = 0;
    virtual int telegramNumber() const = 0;

    QString sourceText;

    QString mType;
    ParseVersion::Version mVersion;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TELEGRAM_H
