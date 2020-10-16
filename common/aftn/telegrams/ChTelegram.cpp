#include "ChTelegram.h"

#include "TelegramTypes.h"
#include "Transliteration.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

ChTelegram::ChTelegram()
{
    setVersion(ParseVersion::PV_LATIN);
}

ChTelegram::ChTelegram(const ChStruct &s)
{
    mCh = s;
    setVersion(s.version);
}

ChTelegram::~ChTelegram()
{
}

QString ChTelegram::data() const
{
    QString ret;

#if defined (BAN_BUILD)
    ret += "ЗЦЗЦ ";
    ret += mCh.transmissionIdentificator.toAscii();
    if (!mCh.additionServiceIndication.isEmpty())
        ret += " " + mCh.additionServiceIndication.toAscii();

    ret += "\r\n";
    ret += type() + " ЛР";
    ret += "\r\n";
    ret += "НННН";
#else

    if (version() == ParseVersion::PV_LATIN)
        ret += "ZCZC ";
    else if (version() == ParseVersion::PV_CYRILLIC)
        ret += "ЗЦЗЦ ";
    else
        return "Unknown version";

    ret += mCh.transmissionIdentificator.toAscii();
    if (!mCh.additionServiceIndication.isEmpty())
        ret += " " + mCh.additionServiceIndication.toAscii();

    ret += "     ";
    ret += "\r\n";

    ret += type();

    if (!mCh.lr.isEmpty()) {
        if (version() == ParseVersion::PV_LATIN)
            ret += " LR " + mCh.lr;
        else if (version() == ParseVersion::PV_CYRILLIC)
            ret += " ЛР " + mCh.lr;
    }

    ret += "\r\n";

    if (version() == ParseVersion::PV_LATIN)
        ret += "NNNN";
    else if (version() == ParseVersion::PV_CYRILLIC)
        ret += "НННН";

    ret += "\r\n";
#endif

    return ret;
}

ParseVersion::Version ChTelegram::version() const
{
    return mCh.version;
}

void ChTelegram::setVersion(ParseVersion::Version v)
{
    Telegram::setVersion(v);

    mCh.transmissionIdentificator = Transliteration::toVersion(mCh.transmissionIdentificator, v);
    mCh.lr = Transliteration::toVersion(mCh.lr, v);
    mCh.version = v;

    mType = Transliteration::toVersion(TelegramType::CH, v);
}

const QString ChTelegram::aftnChannel() const
{
    return mCh.transmissionIdentificator.left(3);
}

int ChTelegram::telegramNumber() const
{
    return mCh.transmissionIdentificator.right(3).toInt();
}

ChTelegram * ChTelegram::replyMessage() const
{
    return new ChTelegram(reply());
}

ChStruct ChTelegram::reply() const
{
    ChStruct ret;

    ret.additionServiceIndication = QDateTime::currentDateTimeUtc().toString("hhmm");
    ret.lr = mCh.transmissionIdentificator;
    ret.version = mCh.version;

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
