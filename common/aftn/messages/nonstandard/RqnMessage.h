#ifndef RQNMESSAGE_H
#define RQNMESSAGE_H

#include "AftnMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class RqnMessage : public AftnMessage
{
public:
    RqnMessage(const QString &s, ParseVersion::Version v);
    ~RqnMessage();
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

#endif // RQNMESSAGE_H
