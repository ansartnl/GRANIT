#ifndef ALRCREATORSHORT_H
#define ALRCREATORSHORT_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AlrCreatorShort : public MessageCreator
{
public:
    AlrCreatorShort();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ALRCREATORSHORT_H
