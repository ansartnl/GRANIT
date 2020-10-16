#ifndef RQSCREATOR_H
#define RQSCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RqsCreator : public MessageCreator
{
public:
    RqsCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RQSCREATOR_H
