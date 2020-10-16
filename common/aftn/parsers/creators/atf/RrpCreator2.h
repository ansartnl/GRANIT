#ifndef RRPCREATOR2_H
#define RRPCREATOR2_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RrpCreator2 : public MessageCreator
{
public:
    RrpCreator2();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RRPCREATOR2_H
