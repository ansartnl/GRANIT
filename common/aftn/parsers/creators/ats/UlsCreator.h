#ifndef ULSCREATOR_H
#define ULSCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class UlsCreator : public MessageCreator
{
public:
    UlsCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ULSCREATOR_H
