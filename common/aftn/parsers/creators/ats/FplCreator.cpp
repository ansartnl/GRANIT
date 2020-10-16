#include "FplCreator.h"

#include "TelegramTypes.h"

#include "FplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

FplCreator::FplCreator()
{
    addType(TelegramType::FPL);
    addType(TelegramType::FPL1);
    addType(TelegramType::FPL, TypeKey::kAdexpDisambiguation);
    addType(TelegramType::FPL1, TypeKey::kAdexpDisambiguation);
}

AftnMessage * FplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    return new FplMessage(createFplStruct(type, map));
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
