#ifndef NONSTANDARDRVMCREATOR_H
#define NONSTANDARDRVMCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class NonStandardRvmCreator : public MessageCreator
{
public:
    NonStandardRvmCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // NONSTANDARDRVMCREATOR_H
