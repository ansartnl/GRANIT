#include "NonStandardRviCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "NonStandardRviMessage.h"

#include "RestrictionParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

NonStandardRviCreator::NonStandardRviCreator()
{
    addType(TelegramType::RVI, TypeKey::kNonstandardDisambiguation);
    addType(TelegramType::RVI1, TypeKey::kNonstandardDisambiguation);
}

AftnMessage * NonStandardRviCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    NonStandardRviStruct rvi;

    rvi.version = aftnParameters->supportedTypes()->version(type);
    rvi.id = Transliteration::toVersion(map.value(MESSAGE_ID_RESTRICTION).toString(), rvi.version);

    return new NonStandardRviMessage(rvi);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
