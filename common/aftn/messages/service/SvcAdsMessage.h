#ifndef SVCADSMESSAGE_H
#define SVCADSMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcAdsStruct
 * \brief Структура SVC/СЖЦ сообщения ADS/АДС
 */

struct SvcAdsStruct
{
    QString transmissionIdentificator;
    QString urgency;
    QStringList receivers;
    enum Error {
        CHECK
        , UNKNOWN
    } error;
    QString badReceiver;

    ParseVersion::Version version;

    SvcAdsStruct() :
        error(UNKNOWN), version(ParseVersion::PV_UNKNOWN)
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
 * \class SvcAdsMessage
 * \brief Класс SVC/СЖЦ сообщения ADS/АДС
 */

class SvcAdsMessage : public AftnMessage
{
public:
    SvcAdsMessage();
    SvcAdsMessage(const SvcAdsStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcAdsStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCADSMESSAGE_H
