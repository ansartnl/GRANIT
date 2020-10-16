#include "GplCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "GplMessage.h"

#include "AtsNonStandardMessagesParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

GplCreator::GplCreator()
{
    addType(TelegramType::GPL);
}

AftnMessage * GplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    Q_UNUSED(type)
    GplStruct gpl;

    gpl.start = map.value(GPL_START_TIME).toDate();
    gpl.finish = map.value(GPL_FINISH_TIME).toDate();

    return new GplMessage(gpl);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
