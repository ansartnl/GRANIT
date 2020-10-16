#include "UnknownCreator.h"

#include "TelegramTypes.h"

#include "UnknownMessage.h"

UnknownCreator::UnknownCreator()
{
    addType(TelegramType::Unknown);
}

AftnMessage * UnknownCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    Q_UNUSED(map)
    return new UnknownMessage;
}
