#ifndef NOTAMCREATOR_H
#define NOTAMCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class NotamCreator : public MessageCreator
{
public:
    NotamCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // NOTAMCREATOR_H
