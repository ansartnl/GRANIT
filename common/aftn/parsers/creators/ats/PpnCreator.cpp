#include "PpnCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "PpnMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

PpnCreator::PpnCreator()
{
    addType(TelegramType::PPN);
    addType(TelegramType::PPN1);
}

AftnMessage * PpnCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    PpnStruct ppn;
    ppn.version = aftnParameters->supportedTypes()->version(type);

    ppn.acid = Transliteration::acidToVersion(map.value(AFTN_MESSAGE_AIRCRAFT_ID).toString(), ppn.version);
    if (ppn.acid.isEmpty())
        addError(TelegramError(TelegramError::STANDARD, ParserErrors::InvalidACID));

    ppn.other = Transliteration::toVersion(map.value(AFTN_MESSAGE_OTHER_INFO).toString(), ppn.version);
    ppn.dof = QDateTime::currentDateTimeUtc().date();
    if (map.contains(AFTN_MESSAGE_OTHER_INFO_DOF))
        ppn.dof = map.value(AFTN_MESSAGE_OTHER_INFO_DOF).toDate();

    return new PpnMessage(ppn);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
