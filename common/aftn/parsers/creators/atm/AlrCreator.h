#ifndef ALRCREATOR_H
#define ALRCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AlrCreator : public MessageCreator
{
public:
    AlrCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif


#endif // ALRCREATOR_H
