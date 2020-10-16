#ifndef RESCREATOR_H
#define RESCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class ResCreator : public MessageCreator
{
public:
    ResCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RESCREATOR_H
