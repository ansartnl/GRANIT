#ifndef RQNCREATOR_H
#define RQNCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RqnCreator : public MessageCreator
{
public:
    RqnCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RQNCREATOR_H
