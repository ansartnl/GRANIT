#ifndef AFTNMESSAGE_H
#define AFTNMESSAGE_H

#include "ParseErrors.h"
#include "Transliteration.h"

#include <QtCore/QString>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \class AftnMessage
 * \brief Класс АФТН сообщения
 *
 * Базовый класс для всех АФТН сообщений.
 */

class AftnMessage
{
public:
    AftnMessage();
    virtual ~AftnMessage();

    virtual QString data() const = 0;
    virtual QString adexp() const;
    QString type() const;

    virtual ParseVersion::Version version() const = 0;
    virtual void setVersion(ParseVersion::Version v);

    const TelegramErrorList errors() const;
    const TelegramErrorSet uniqueErrors() const;
    void addError(const TelegramError &err);
    void addErrors(const TelegramErrorList &err);
    void addErrors(const TelegramErrorSet &err);
    void setErrors(const TelegramErrorList &err);
    void clearErrors();
    bool hasErrors() const;
    bool noErrors() const;

    inline friend QDataStream & operator<< (QDataStream &stream, const AftnMessage &msg)
    {
        stream << msg.mType;
        stream << msg.mErrors;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, AftnMessage &msg)
    {
        stream >> msg.mType;
        stream >> msg.mErrors;
        return stream;
    }

protected:
    QString mType;
    TelegramErrorList mErrors;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // AFTNMESSAGE_H
