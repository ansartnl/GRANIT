#ifndef FLACREATOR_H
#define FLACREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class FlaCreator : public MessageCreator
{
public:
    FlaCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // FLACREATOR_H
