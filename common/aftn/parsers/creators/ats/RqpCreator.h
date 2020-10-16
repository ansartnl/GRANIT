#ifndef RQPCREATOR_H
#define RQPCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RqpCreator : public MessageCreator
{
public:
    RqpCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RQPCREATOR_H
