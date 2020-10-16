#include "SvcAdsMessage.h"

#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SvcAdsMessage::SvcAdsMessage()
{
}

SvcAdsMessage::SvcAdsMessage(const SvcAdsStruct &s) :
    mSvc(s)
{
    setVersion(s.version);
}

QString SvcAdsMessage::data() const
{
    QString ret;

    ret += type();

    ret += " ";
    ret += Transliteration::toVersion("QTA", version());

    ret += " ";
    ret += Transliteration::toVersion("ADS", version());

    ret += " ";
    ret += mSvc.transmissionIdentificator;

    int receiversInLine = 0;
    foreach (const QString &receiver, mSvc.receivers) {
        if (!(receiversInLine % 7)) {
            ret += "\r\n";
            ret += mSvc.urgency;
        }
        ret += " ";
        ret += receiver;
        ++receiversInLine;
    }

    ret += "\r\n";
    if (mSvc.error == SvcAdsStruct::CHECK)
        ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("ПРОВЕРКА") : QString("CHECK");
    else
        ret += (version() == ParseVersion::PV_CYRILLIC) ? QString::fromUtf8("НЕИЗВЕСТНО") : QString("UNKNOWN");

    ret += " ";
    ret += mSvc.badReceiver;

    return ret;
}

ParseVersion::Version SvcAdsMessage::version() const
{
    return mSvc.version;
}

void SvcAdsMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mSvc.transmissionIdentificator = Transliteration::toVersion(mSvc.transmissionIdentificator, v);
    mSvc.urgency = Transliteration::toVersion(mSvc.urgency, v);
    QStringList newReceivers;
    foreach (const QString &address, mSvc.receivers)
        newReceivers << Transliteration::addressToVersion(address, v);
    mSvc.receivers = newReceivers;
    mSvc.badReceiver = Transliteration::toVersion(mSvc.badReceiver, v);

    mSvc.version = v;

    mType = Transliteration::toVersion(TelegramType::SVC, v);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
