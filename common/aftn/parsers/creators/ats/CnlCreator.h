#ifndef CNLCREATOR_H
#define CNLCREATOR_H

#include "MessageCreator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class CnlCreator : public MessageCreator
{
public:
    CnlCreator();

protected:
    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CNLCREATOR_H
