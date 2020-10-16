#ifndef ANMCREATOR_H
#define ANMCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AnmCreator : public MessageCreator
{
public:
    AnmCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ANMCREATOR_H
