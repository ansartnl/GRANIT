#ifndef APZCREATOR_H
#define APZCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class ApzCreator : public MessageCreator
{
public:
    ApzCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // APZCREATOR_H
