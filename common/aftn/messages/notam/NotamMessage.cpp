#include "NotamMessage.h"

#include "TelegramTypes.h"

#include <QtCore/QRegExp>
#include <QtCore/QSet>
#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

QList < int > NotamIdList::years() const
{
    QSet < int > ret;

    foreach (const NotamId &id, *this)
        ret.insert(id.year);

    return ret.toList();
}

QList < int > NotamIdList::numbersForYear(int year) const
{
    QList < int > ret;

    foreach (const NotamId &id, *this) {
        if (id.year == year)
            ret.append(id.number);
    }

    return ret;
}



NotamMessage::NotamMessage()
{
}

NotamMessage::NotamMessage(const NotamStruct &s) :
    mNotam(s)
{
    setVersion(s.version);
}

NotamMessage::~NotamMessage()
{
}

QString NotamMessage::data() const
{
    QString ret;

    ret += "(";

    ret += QString("%1%2/%3").arg(mNotam.series).arg(mNotam.number, 4, 10, QChar('0'))
            .arg(mNotam.notamYear(), 2, 10, QChar(0));
    ret += Transliteration::toVersion(QString(" NOTAM%1").arg(mNotam.type), mNotam.version);
    if (Transliteration::to4444(mNotam.type) != "N")
        ret += QString("%1%2/%3").arg(mNotam.replaceSeries)
                .arg(mNotam.replaceNumber, 4, 10, QChar('0'))
                .arg(mNotam.notamReplaceYear(), 2, 10, QChar(0));

    ret += "\r\n";

    ret += Transliteration::toVersion(QString("Q)%1/Q%2%3/%4/%5/%6/%7/%8")
                                   .arg(mNotam.fir)
                                   .arg(mNotam.subject)
                                   .arg(mNotam.condition)
                                   .arg(mNotam.traffic)
                                   .arg(mNotam.purpose)
                                   .arg(mNotam.scope)
                                   .arg(mNotam.qLowerLimit.isEmpty() ? " " : mNotam.qLowerLimit.rightJustified(3,'0'))
                                   .arg(mNotam.qUpperLimit.isEmpty() ? " " : mNotam.qUpperLimit.rightJustified(3,'0'))
                                   , mNotam.version);
    if (!mNotam.location.isEmpty())
        ret += QString("/%1%2").arg(mNotam.location).arg(QString::number(mNotam.radius).rightJustified(3,'0'));

    ret += "\r\n";

    ret += Transliteration::toVersion(QString("A)%1").arg(mNotam.aerodrome), mNotam.version);
    ret += Transliteration::toVersion(QString(" B)%1").arg(mNotam.validFrom.toString("yyMMddhhmm")), mNotam.version);
    if ((mNotam.validType == "PERM") || (mNotam.validType == QString::fromUtf8("ПОСТ"))) {
        ret += Transliteration::toVersion(QString(" C)%1").arg(mNotam.validType), mNotam.version);
    } else if (mNotam.validTill.isValid()) {
        ret += Transliteration::toVersion(QString(" C)%1%2")
                                       .arg(mNotam.validTill.toString("yyMMddhhmm"))
                                       .arg(mNotam.validType)
                                       , mNotam.version);
    }

    if (!mNotam.schedule.isEmpty())
        ret += Transliteration::toVersion(QString("\nD)%1").arg(mNotam.schedule), mNotam.version);

    if (!mNotam.description.isEmpty())
        ret += Transliteration::toVersion(QString("\nE)%1").arg(mNotam.description), mNotam.version);

    if (!mNotam.lowerLimit.isEmpty() || !mNotam.upperLimit.isEmpty())
        ret += "\r\n";

    if (!mNotam.lowerLimit.isEmpty()) {
        ret += Transliteration::toVersion(QString("F)%1").arg(mNotam.lowerLimit), mNotam.version);
        if (!mNotam.upperLimit.isEmpty())
            ret += " ";
    }

    if (!mNotam.upperLimit.isEmpty())
        ret += Transliteration::toVersion(QString("\nG)%1").arg(mNotam.upperLimit), mNotam.version);

    ret +=")";

    return ret;
}

ParseVersion::Version NotamMessage::version() const
{
    return mNotam.version;
}

void NotamMessage::setVersion(ParseVersion::Version v)
{
    AftnMessage::setVersion(v);

    mNotam.series = Transliteration::toVersion(mNotam.series, v);
    mNotam.type = Transliteration::toVersion(mNotam.type, v);
    mNotam.replaceSeries = Transliteration::toVersion(mNotam.replaceSeries, v);
    mNotam.subject = Transliteration::toVersion(mNotam.subject, v);
    mNotam.condition = Transliteration::toVersion(mNotam.condition, v);
    mNotam.traffic = Transliteration::toVersion(mNotam.traffic, v);
    mNotam.purpose = Transliteration::toVersion(mNotam.purpose, v);
    mNotam.scope = Transliteration::toVersion(mNotam.scope, v);
    mNotam.location = Transliteration::toVersion(mNotam.location, v);
    mNotam.aerodrome = Transliteration::toVersion(mNotam.aerodrome, v);
    if (NotamStruct::isPerm(mNotam.validType))
        mNotam.validType = NotamStruct::perm(v);
    else if (NotamStruct::isEst(mNotam.validType))
        mNotam.validType = NotamStruct::est(v);
    else
        mNotam.validType = Transliteration::toVersion(mNotam.validType, v);
    mNotam.schedule = Transliteration::toVersion(mNotam.schedule, v);
    mNotam.description = Transliteration::toVersion(mNotam.description, v);
    mNotam.lowerLimit = Transliteration::toVersion(mNotam.lowerLimit, v);
    mNotam.upperLimit = Transliteration::toVersion(mNotam.upperLimit, v);

    mNotam.version = v;

    mType = Transliteration::toVersion(TelegramType::NOTAM, v);
}

bool NotamMessage::isCheckList() const
{
    return mNotam.description.trimmed().startsWith(
                Transliteration::toVersion(
                    "СВОДКА ДЕЙСТВУЮЩИХ НОТАМ"
                    , mNotam.version));
}

NotamIdList NotamMessage::activeNotams() const
{
    NotamIdList ret;

    if (!isCheckList())
        return ret;

    QRegExp rexp("ГОД=(\\d{4})\\s((\\d{4}\\s*\\r?\\n?)+)");
    int offset = 0;
    while ((offset = rexp.indexIn(mNotam.description, offset)) != -1) {
        NotamId id;
        id.year = rexp.cap(1).toInt();
        QStringList numbers = rexp.cap(2).split(QRegExp("\\s|\\r|\\r\\n"), QString::SkipEmptyParts);
        foreach (const QString &n, numbers) {
            id.number = n.toInt();
            id.series = mNotam.series;
            ret.append(id);
        }
        offset += rexp.matchedLength();
    }

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
