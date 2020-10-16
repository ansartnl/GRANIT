#ifndef SVCQTAMSRMESSAGE_H
#define SVCQTAMSRMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaMsrStruct
 * \brief Структура SVC/СЖЦ сообщения QTA MSR/ЩТА МСР
 */

struct SvcQtaMsrStruct
{
    QString transmissionIdentificator;

    ParseVersion::Version version;

    SvcQtaMsrStruct() :
        version(ParseVersion::PV_UNKNOWN)
    {}

    inline const QString aftnChannel() const
    {
        return transmissionIdentificator.left(3);
    }
    inline int telegramNumber() const
    {
        return transmissionIdentificator.right(3).toInt();
    }
};

/*!
 * \class SvcQtaMsrMessage
 * \brief Класс SVC/СЖЦ сообщения QTA MSR/ЩТА МСР
 */

class SvcQtaMsrMessage : public AftnMessage
{
public:
    SvcQtaMsrMessage();
    SvcQtaMsrMessage(const SvcQtaMsrStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaMsrStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTAMSRMESSAGE_H
