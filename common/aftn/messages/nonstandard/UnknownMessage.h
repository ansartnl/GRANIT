#ifndef UNKNOWNMESSAGE_H
#define UNKNOWNMESSAGE_H

#include "AftnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class UnknownMessage : public AftnMessage
{
public:
    UnknownMessage(const QString &s = QString());
    virtual ~UnknownMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

protected:
    QString source;
    ParseVersion::Version mVersion;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // UNKNOWNMESSAGE_H
