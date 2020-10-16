#ifndef NONSTANDARDRVICREATOR_H
#define NONSTANDARDRVICREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class NonStandardRviCreator : public MessageCreator
{
public:
    NonStandardRviCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // NONSTANDARDRVICREATOR_H
