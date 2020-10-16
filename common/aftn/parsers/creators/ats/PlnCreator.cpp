#include "PlnCreator.h"

#include "TelegramTypes.h"

#include "PlnMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

PlnCreator::PlnCreator()
{
    addType(TelegramType::PLN);
    addType(TelegramType::PLN1);
}

AftnMessage * PlnCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    return new PlnMessage(createFplStruct(type, map));
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
