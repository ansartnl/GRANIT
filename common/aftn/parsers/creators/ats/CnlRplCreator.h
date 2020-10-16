#ifndef CNLRPLCREATOR_H
#define CNLRPLCREATOR_H

#include "FplStructCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class CnlRplCreator : public FplStructCreator
{
public:
    CnlRplCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CNLRPLCREATOR_H
