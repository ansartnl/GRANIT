#include "AftnTelegram.h"

#include "Transliteration.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

const QString AftnStruct::fillingTime() const
{
    return QString("%1%2%3").arg(fillingDate, 2, 10, QChar('0'))
            .arg(fillingHour, 2, 10, QChar('0'))
            .arg(fillingMinute, 2, 10, QChar('0'));
}

AftnTelegram::AftnTelegram()
{
}

AftnTelegram::AftnTelegram(const AftnStruct &s)
{
    mAftn = s;
    setVersion(s.version);
}

AftnTelegram::~AftnTelegram()
{
}

QString AftnTelegram::data() const
{
    QString ret = header();
    ret += "\r\n";

    // TODO: beginning of the text

    if (!textBeforeMessage().isEmpty())
    {
        ret += textBeforeMessage();
    }

    ret += body();

//    if (!mAftn.confirmation.isEmpty())
//    {
//        ret += "\r\n";
//        ret += "CFM" + mAftn.confirmation;
//    }
//    if (!mAftn.correction.isEmpty())
//    {
//        ret += "\r\n";
//        ret += "COR" + mAftn.correction;
//    }

    if (!textAfterMessage().isEmpty())
    {
        ret += textAfterMessage();
    } else
    {
        ret += "\r\n";
    }

    // seven line feeds
    ret += footer();

    return ret;
}

QString AftnTelegram::header() const
{
    QString ret;
    if (mAftn.version == ParseVersion::PV_LATIN)
        ret += "ZCZC ";
    else if (mAftn.version == ParseVersion::PV_CYRILLIC)
        ret += "ЗЦЗЦ ";
    else
        return "Unknown version";
    ret += mAftn.transmissionIdentificator;
    if (!mAftn.additionServiceIndication.isEmpty())
        ret += " " + mAftn.additionServiceIndication;
    ret += "     ";
#if defined(BAN_BUILD)
    ret += "\r\n";
    ret += mAftn.priorityIndicator;
    int receiversInLine = 0;
    foreach (const QString &receiver, mAftn.receiverAddresses)
    {
        if (!(receiversInLine % 7) && receiversInLine)
            ret += "\r\n";
        ret += " ";
        ret += receiver;
        ++receiversInLine;
    }
#else
    int receiversInLine = 0;
    foreach (const QString &receiver, mAftn.receiverAddresses)
    {
        if (!(receiversInLine % 7))
        {
            ret += "\r\n";
            ret += mAftn.priorityIndicator;
        }
        ret += " ";
        ret += receiver;
        ++receiversInLine;
    }
#endif
    ret += "\r\n";
    ret += mAftn.fillingTime() + " ";
    ret += mAftn.senderAddress;
    return ret;
}

QString AftnTelegram::body() const
{
    return mAftn.text.join("\r\n");
}

QString AftnTelegram::footer() const
{
    QString ret;
    // seven line feeds
    ret += "\r\n\r\n\r\n\r\n\r\n\r\n\r\n";
    if (mAftn.version == ParseVersion::PV_LATIN)
        ret += "NNNN";
    else if (mAftn.version == ParseVersion::PV_CYRILLIC)
        ret += "НННН";
    return ret;
}

QString AftnTelegram::textBeforeMessage() const
{
    return mAftn.textBeforeMessage;
}

QString AftnTelegram::textAfterMessage() const
{
    return mAftn.textAfterMessage;
}

ParseVersion::Version AftnTelegram::version() const
{
    return mAftn.version;
}

void AftnTelegram::setVersion(ParseVersion::Version v)
{
    Telegram::setVersion(v);

    mAftn.transmissionIdentificator = Transliteration::toVersion(
                mAftn.transmissionIdentificator, v);
    mAftn.priorityIndicator = Transliteration::toVersion(mAftn.priorityIndicator, v);
    QStringList newReceiverAdresses;
    foreach (const QString &address, mAftn.receiverAddresses)
        newReceiverAdresses << Transliteration::addressToVersion(address, v);
    mAftn.receiverAddresses = newReceiverAdresses;
    mAftn.senderAddress = Transliteration::addressToVersion(mAftn.senderAddress, v);
    mAftn.confirmation = Transliteration::toVersion(mAftn.confirmation, v);
    mAftn.correction = Transliteration::toVersion(mAftn.correction, v);

    mAftn.version = v;
}

const QString AftnTelegram::aftnChannel() const
{
    return mAftn.transmissionIdentificator.left(3);
}

int AftnTelegram::telegramNumber() const
{
    return mAftn.transmissionIdentificator.right(3).toInt();
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
