#ifndef FPLCREATOR_H
#define FPLCREATOR_H

#include "FplStructCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class FplCreator : public FplStructCreator
{
public:
    FplCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // FPLCREATOR_H
