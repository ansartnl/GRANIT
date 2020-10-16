#include "MessageCreator.h"

#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

MessageCreator::MessageCreator()
{
}

MessageCreator::~MessageCreator()
{
}

AftnMessage * MessageCreator::create(const QString &type, const QVariantMap &map) const
{
    mErrors.clear();
    return pCreate(type, map);
}

const QStringList MessageCreator::types() const
{
    return mTypes.keys().toSet().toList();
}

const QList<TypeKey> MessageCreator::keys(const QString &type) const
{
    return mTypes.values(type);
}

const TelegramErrorList MessageCreator::lastErrors() const
{
    return mErrors;
}

void MessageCreator::addType(const QString &type, int disambiguation)
{
    mTypes.insert(type, TypeKey(type, disambiguation));
}

void MessageCreator::addError(const TelegramError &err) const
{
    mErrors << err;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
