#ifndef PPNCREATOR_H
#define PPNCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class PpnCreator : public MessageCreator
{
public:
    PpnCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PPNCREATOR_H
