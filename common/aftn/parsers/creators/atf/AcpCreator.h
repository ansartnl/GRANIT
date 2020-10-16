#ifndef ACPCREATOR_H
#define ACPCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AcpCreator : public MessageCreator
{
public:
    AcpCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ACPCREATOR_H