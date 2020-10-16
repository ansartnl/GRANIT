#ifndef UNKNOWNCREATOR_H
#define UNKNOWNCREATOR_H

#include "MessageCreator.h"

class UnknownCreator : public MessageCreator
{
public:
    UnknownCreator();

    AftnMessage * pCreate(const QString &type, const QVariantMap &map) const;
};

#endif // UNKNOWNCREATOR_H
