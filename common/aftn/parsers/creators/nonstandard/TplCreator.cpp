#include "TplCreator.h"

#include "TelegramTypes.h"

#include "TplMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

TplCreator::TplCreator()
{
    addType(TelegramType::TPL);
}

AftnMessage * TplCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    TplMessage *tpl = new TplMessage(createFplStruct(type, map));
    tpl->mFpl.version = ParseVersion::textVersion(tpl->mFpl.adep);
    return tpl;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
