#ifndef RPLCREATOR_H
#define RPLCREATOR_H

#include "FplStructCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RplCreator : public FplStructCreator
{
public:
    RplCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RPLCREATOR_H
