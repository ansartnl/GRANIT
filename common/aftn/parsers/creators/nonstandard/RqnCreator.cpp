#include "RqnCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "RqnMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

RqnCreator::RqnCreator()
{
    addType(TelegramType::RQN);
}

AftnMessage * RqnCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    return new RqnMessage(map.value(AFTN_SOURCE_MESSAGE).toString()
                          , aftnParameters->supportedTypes()->version(type));
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
