#ifndef METEOCREATOR_H
#define METEOCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class MeteoCreator : public MessageCreator
{
public:
    MeteoCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // METEOCREATOR_H
