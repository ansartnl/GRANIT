#include "TelegramParser.h"

#include "AftnTelegram.h"
#include "ChTelegram.h"
#include "Transliteration.h"
#include "TelegramTypes.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

TelegramParser::TelegramParser(QObject *parent) :
    QObject(parent)
{
}

Telegram * TelegramParser::parse(const QString &data, ParserErrors::AftnError &error)
{
    TelegramParser parser;

    Telegram *t = parser.parse(data);
    error = parser.lastError();

    return t;
}

Telegram * TelegramParser::parse(const QString &data)
{
    QString tmp = data;
    tmp.remove('\r');
    Telegram *ret = tryParseCh(tmp);
    if (!ret)
        ret = parseAftn(tmp);
    ret->sourceText = tmp;
    return ret;
}

const QString TelegramParser::message(const QString &data)
{
    TelegramParser parser;
    QScopedPointer < Telegram > t(parser.parse(data));
    AftnTelegram *aftn = static_cast < AftnTelegram * >(t.data());
    if (aftn)
        return aftn->mAftn.text.join("\n");
    return QString();
}

ParserErrors::AftnError TelegramParser::lastError() const
{
    return mError;
}

QStringList TelegramParser::splitText(const QString &text)
{
    QRegExp regExp("(\\r|\\n|^)\\s*(\\([A-Z|А-Я]{3}\\-.*\\))");
    if (regExp.indexIn(text) == -1) {
        return QStringList() << text;
    }
    QString messageIn = regExp.cap(2);

    QRegExp type(QString("(\\r|\\n|^)\\s*(\\(%1)")
                 .arg(TelegramType::allTypes().join("|\\("))
                 );

    QStringList ret;
    int index;
    while ((index = messageIn.indexOf(type, 1)) != -1) {
        ret << messageIn.left(index).trimmed();
        messageIn.remove(0, index);
    }
    if (ret.isEmpty())
        ret << text;
    else if (!messageIn.isEmpty())
        ret << messageIn.trimmed();
    return ret;
}

const TextWithBeginningAndEnding TelegramParser::splitText2(const QString &text)
{
    TextWithBeginningAndEnding ret;

    QString tempText = text;

    QRegExp rexp("(?:^|\\n)\\((.+)\\)");
    if ((rexp.indexIn(tempText) != -1) && (rexp.cap(1).size() > 3)) {
        QString textBeforeMessage = text.left(text.indexOf("("));
        if (!textBeforeMessage.isEmpty()) {
            ret.beginning = textBeforeMessage;
            tempText = tempText.mid(tempText.indexOf("("), tempText.count());
        }

        QString textAfterMessage = text.right(text.size() - text.lastIndexOf(")") - 1);
        if (!textAfterMessage.isEmpty()) {
            ret.ending = textAfterMessage;
            tempText = tempText.left(tempText.lastIndexOf(")") + 1);
        }
    }
    ret.text = splitText(tempText);

    return ret;
}

ChTelegram * TelegramParser::tryParseCh(const QString &data)
{
    ChStruct ch;

    int beginning = data.indexOf(QRegExp(QString::fromUtf8("[ZЗ][CЦ][ZЗ][CЦ]")));
    if (beginning == -1)
        return 0;

    if (data.contains("ZCZC"))
        ch.version = ParseVersion::PV_LATIN;
    else if (data.contains(QString::fromUtf8("ЗЦЗЦ")))
        ch.version = ParseVersion::PV_CYRILLIC;
    else
        ch.version = ParseVersion::PV_UNKNOWN;

    beginning += 5;
    int ending = data.indexOf(QRegExp(QString::fromUtf8("NNNN|НННН")));
    if (ending == -1)
        return 0;

    QString midData = data.mid(beginning, ending - beginning).trimmed();

    QRegExp tiRegExp("^(\\w{3}\\d{3})");
    if (tiRegExp.indexIn(midData))
        return 0;
    ch.transmissionIdentificator = tiRegExp.cap(1);
    midData = midData.mid(6);

    QRegExp asiRegExp(QString::fromUtf8("^ *([A-ZА-Я0-9 \\-\\?\\:\\(\\)\\.\\,\\’\\=\\/\\+\\!]{0,23})"));
    if (!asiRegExp.indexIn(midData)) {
        ch.additionServiceIndication = asiRegExp.cap(1).trimmed();
        midData = midData.mid(asiRegExp.cap(1).size()).trimmed();
    }

    QRegExp chRegExp(QString::fromUtf8("^(CH|ЦХ)"));
    if (chRegExp.indexIn(midData))
        return 0;
    midData = midData.mid(2).trimmed();

    QRegExp lrRegExp(QString::fromUtf8("^(LR|ЛР)"));
    if (!lrRegExp.indexIn(midData)) {
        midData = midData.mid(2).trimmed();

        QRegExp lrtiRegExp("^(\\w{3}\\d{3})");
        if (lrtiRegExp.indexIn(midData))
            return 0;
        ch.lr = lrtiRegExp.cap(1);
        midData = midData.mid(6).trimmed();
    }

    return new ChTelegram(ch);
}

AftnTelegram * TelegramParser::parseAftn(const QString &data)
{
    mError = ParserErrors::AftnNoError;
    AftnStruct aftn;

    int beginning = data.indexOf(QRegExp(QString::fromUtf8("[ZЗ][CЦ][ZЗ][CЦ]")));
    if (beginning == -1) {
        mError = ParserErrors::AftnNoZCZC;
        return new AftnTelegram(aftn);
    }
    if (data.contains("ZCZC"))
        aftn.version = ParseVersion::PV_LATIN;
    else if (data.contains(QString::fromUtf8("ЗЦЗЦ")))
        aftn.version = ParseVersion::PV_CYRILLIC;
    else
        aftn.version = ParseVersion::PV_UNKNOWN;

    beginning += 5;
    int ending = data.indexOf(QRegExp(QString::fromUtf8("NNNN|НННН")));
    if (ending == -1) {
        mError = ParserErrors::AftnNoNNNN;
        return new AftnTelegram(aftn);
    }

    QString midData = data.mid(beginning, ending - beginning).trimmed();

    QRegExp tiRegExp("^(\\w{3}\\d{3})");
    if (tiRegExp.indexIn(midData)) {
        mError = ParserErrors::AftnErrorTransmissionIdentification;
        return new AftnTelegram(aftn);
    }
    aftn.transmissionIdentificator = tiRegExp.cap(1);
    midData = midData.mid(6);

    QRegExp asiRegExp(QString::fromUtf8("^ *([A-ZА-Я0-9 \\-\\?\\:\\(\\)\\.\\,\\’\\=\\/\\+\\!]{0,23})"));
    if (!asiRegExp.indexIn(midData)) {
        aftn.additionServiceIndication = asiRegExp.cap(1).trimmed();
        midData = midData.mid(asiRegExp.cap(1).size()).trimmed();
    }

    QRegExp piRegExp(QString::fromUtf8("^(KK|FF|GG|DD|SS|КК|ФФ|ГГ|ДД|СС)"));
    if (piRegExp.indexIn(midData)) {
        mError = ParserErrors::AftnErrorPriorityIndicator;
        return new AftnTelegram(aftn);
    }
    aftn.priorityIndicator = piRegExp.cap(1);
    midData = midData.mid(2).trimmed();

    QRegExp raRegExp("^(\\w{8})");
    while (!raRegExp.indexIn(midData)) {
        aftn.receiverAddresses.append(raRegExp.cap(1));
        midData = midData.mid(8).trimmed();
    }

    // < многострочные получатели
    if (!piRegExp.indexIn(midData)) {
        if (aftn.priorityIndicator != piRegExp.cap(1)) {
            mError = ParserErrors::AftnErrorPriorityIndicator;
            return new AftnTelegram(aftn);
        }
        midData = midData.mid(2).trimmed();
    }
    while (!raRegExp.indexIn(midData)) {
        aftn.receiverAddresses.append(raRegExp.cap(1));
        midData = midData.mid(8).trimmed();
    }
    if (!piRegExp.indexIn(midData)) {
        if (aftn.priorityIndicator != piRegExp.cap(1)) {
            mError = ParserErrors::AftnErrorPriorityIndicator;
            return new AftnTelegram(aftn);
        }
        midData = midData.mid(2).trimmed();
    }
    while (!raRegExp.indexIn(midData)) {
        aftn.receiverAddresses.append(raRegExp.cap(1));
        midData = midData.mid(8).trimmed();
    }

    if (aftn.receiverAddresses.isEmpty()) {
        mError = ParserErrors::AftnErrorReceiverAddress;
        return new AftnTelegram(aftn);
    }
    // >

    QRegExp ftRegExp("^(\\d{2})(\\d{2})(\\d{2})");
    if (ftRegExp.indexIn(midData)) {
        mError = ParserErrors::AftnErrorFillingTime;
        return new AftnTelegram(aftn);
    }
    aftn.fillingDate = ftRegExp.cap(1).toUInt();
    aftn.fillingHour = ftRegExp.cap(2).toUInt();
    aftn.fillingMinute = ftRegExp.cap(3).toUInt();
    midData = midData.mid(6).trimmed();

    QRegExp saRegExp(QString::fromUtf8("^(\\w{8})(Ю*)?"));
    if (saRegExp.indexIn(midData)) {
        mError = ParserErrors::AftnErrorSenderAddress;
        return new AftnTelegram(aftn);
    }
    aftn.senderAddress = saRegExp.cap(1);
    midData = midData.mid(8).trimmed();

    // TODO: add parsing beggining of the text

//    int corIndex = midData.lastIndexOf("\nCOR");
//    if (corIndex != -1)
//    {
//        aftn.correction = midData.mid(corIndex + 4);
//        midData.remove(corIndex, aftn.correction.size() + 4);
//    }

//    int cfmIndex = midData.lastIndexOf("\nCFM");
//    if (cfmIndex != -1)
//    {
//        aftn.correction = midData.mid(cfmIndex + 4);
//        midData.remove(cfmIndex, aftn.correction.size() + 4);
//    }

    aftn.part = AftnStruct::NOT_PART;

    QRegExp partRegExp(QString::fromUtf8("(.*)(\\n//КОНЕЦ ЧАСТИ\\s(\\d{2})(/(\\d{2}))?//)"));
    QRegExp part2RegExp(
                QString::fromUtf8(
                    "(ПРОДОЛЖЕНИЕ\\.\\s)?ЧАСТЬ\\s(\\d{1,2})(.*)((ПРОДОЛЖЕНИЕ\\sСЛЕДУЕТ)|(КОНЕЦ))"));
    if (partRegExp.indexIn(midData) != -1) {
        aftn.text << partRegExp.cap(1);
        aftn.partNumber = partRegExp.cap(3).toInt();
        if (partRegExp.cap(4).isEmpty()) {
            aftn.part = AftnStruct::PART;
        } else {
            aftn.part = AftnStruct::LAST_PART;
            if (partRegExp.cap(3) != partRegExp.cap(5)) {
                mError = ParserErrors::AftnErrorLastPart;
                return new AftnTelegram(aftn);
            }
        }
    } else if (part2RegExp.indexIn(midData) != -1) {
        aftn.text << part2RegExp.cap(3).trimmed();
        aftn.partNumber = part2RegExp.cap(2).toInt();
        if (part2RegExp.cap(5).isEmpty())
            aftn.part = AftnStruct::LAST_PART;
        else
            aftn.part = AftnStruct::PART;
    } else {
        TextWithBeginningAndEnding tbe = splitText2(midData);
        aftn.text = tbe.text;
        aftn.textBeforeMessage = tbe.beginning;
        aftn.textAfterMessage = tbe.ending;
    }
    return new AftnTelegram(aftn);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
