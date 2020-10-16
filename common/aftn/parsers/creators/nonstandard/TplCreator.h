#ifndef TPLCREATOR_H
#define TPLCREATOR_H

#include "FplStructCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class TplCreator : public FplStructCreator
{
public:
    TplCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TPLCREATOR_H
