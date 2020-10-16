#ifndef SPSCREATOR_H
#define SPSCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class SpsCreator : public MessageCreator
{
public:
    SpsCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SPSCREATOR_H
