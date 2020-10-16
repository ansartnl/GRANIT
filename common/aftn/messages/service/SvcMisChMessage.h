#ifndef SVCMISCHMESSAGE_H
#define SVCMISCHMESSAGE_H

#include "SvcMessage.h"

#include <QtCore/QTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcMisChStruct
 * \brief Структура SVC/СЖЦ сообщения MIS CH/МИС ЦХ
 */

struct SvcMisChStruct
{
    QTime timeExpected;
    QString transmissionIdentificator;

    ParseVersion::Version version;

    SvcMisChStruct() :
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
 * \class SvcMisChMessage
 * \brief Класс SVC/СЖЦ сообщения MIS CH/МИС ЦХ
 */

class SvcMisChMessage : public AftnMessage
{
public:
    SvcMisChMessage();
    SvcMisChMessage(const SvcMisChStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcMisChStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCMISCHMESSAGE_H
