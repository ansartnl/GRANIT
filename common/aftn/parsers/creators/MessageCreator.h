#ifndef MESSAGECREATOR_H
#define MESSAGECREATOR_H

#include "ParseErrors.h"
#include "TypeKey.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QVariantMap>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AftnMessage;

class MessageCreator
{
public:
    MessageCreator();
    virtual ~MessageCreator();

    AftnMessage * create(const QString &type, const QVariantMap &map) const;

    const QStringList types() const;
    const QList < TypeKey > keys(const QString &type) const;

    const TelegramErrorList lastErrors() const;

protected:
    virtual AftnMessage * pCreate(const QString &type, const QVariantMap &map) const = 0;

    void addType(const QString &type, int disambiguation = TypeKey::kDefaultDisambiguation);
    void addError(const TelegramError &err) const;

private:
    QMultiMap < QString, TypeKey > mTypes;
    mutable TelegramErrorList mErrors;
};

typedef QSharedPointer < MessageCreator > PMessageCreator;

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // MESSAGECREATOR_H
