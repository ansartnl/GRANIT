#ifndef PLRCREATOR_H
#define PLRCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class PlrCreator : public MessageCreator
{
public:
    PlrCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PLRCREATOR_H
