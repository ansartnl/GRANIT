#ifndef SVCQTAADSMESSAGE_H
#define SVCQTAADSMESSAGE_H

#include "SvcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SvcQtaAdsStruct
 * \brief Структура SVC/СЖЦ сообщения QTA ADS/ЩТА АДС
 */

struct SvcQtaAdsStruct
{
    QString transmissionIdentificator;

    ParseVersion::Version version;

    SvcQtaAdsStruct() :
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
 * \class SvcQtaAdsMessage
 * \brief Класс SVC/СЖЦ сообщения QTA ADS/ЩТА АДС
 */

class SvcQtaAdsMessage : public AftnMessage
{
public:
    SvcQtaAdsMessage();
    SvcQtaAdsMessage(const SvcQtaAdsStruct &s);

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    SvcQtaAdsStruct mSvc;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCQTAADSMESSAGE_H
