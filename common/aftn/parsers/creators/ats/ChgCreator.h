#ifndef CHGCREATOR_H
#define CHGCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class ChgCreator : public MessageCreator
{
public:
    ChgCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CHGCREATOR_H
