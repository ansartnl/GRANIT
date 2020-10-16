#ifndef GPLCREATOR_H
#define GPLCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class GplCreator : public MessageCreator
{
public:
    GplCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // GPLCREATOR_H
