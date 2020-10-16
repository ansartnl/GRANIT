#ifndef FPLSTRUCTCREATOR_H
#define FPLSTRUCTCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

struct FplStruct;

class FplStructCreator : public MessageCreator
{
public:
    FplStructCreator();

protected:
    const FplStruct createFplStruct(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // FPLSTRUCTCREATOR_H
