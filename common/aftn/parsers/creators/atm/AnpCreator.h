#ifndef ANPCREATOR_H
#define ANPCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AnpCreator : public MessageCreator
{
public:
    AnpCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ANPCREATOR_H
