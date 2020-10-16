#ifndef PLNCREATOR_H
#define PLNCREATOR_H

#include "FplStructCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class PlnCreator : public FplStructCreator
{
public:
    PlnCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PLNCREATOR_H
