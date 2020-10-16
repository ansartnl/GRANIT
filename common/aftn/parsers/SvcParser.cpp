#include "SvcParser.h"

#include "SvcAdsMessage.h"
#include "SvcLrExpMessage.h"
#include "SvcLrLsMessage.h"
#include "SvcMisChMessage.h"
#include "SvcQtaAdsMessage.h"
#include "SvcQtaMisMessage.h"
#include "SvcQtaMsrMessage.h"
#include "SvcQtaOgnMessage.h"
#include "SvcQtaRptMessage.h"
#include "SvcQtaRptMessage2.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace SvcParser
{

bool isSvc(const QString &data)
{
    return (data.startsWith("SVC") || data.startsWith(QString::fromUtf8("СЖЦ")));
}

SvcAdsMessage * svcAds(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(ADS|АДС)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     "[\\s\\r\\n]+"
                     "(KK|FF|GG|DD|SS|КК|ФФ|ГГ|ДД|СС)"
                     "\\s+"
                     "([A-ZА-Я\\s]+)"
                     "[\\s\\r\\n]+"
                     "(CHECK|ПРОВЕРКА|UNKNOWN|НЕИЗВЕСТНО)"
                     "\\s+"
                     "([A-ZА-Я]+)"
                     ));

    if (!rexp.exactMatch(data))
        return 0;

    SvcAdsStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.transmissionIdentificator = rexp.cap(3);
    s.urgency = rexp.cap(4);
    s.receivers = rexp.cap(5).split(' ', QString::SkipEmptyParts);
    if (rexp.cap(6) == "CHECK" || rexp.cap(6) == QString::fromUtf8("ПРОВЕРКА"))
        s.error = SvcAdsStruct::CHECK;
    else
        s.error = SvcAdsStruct::UNKNOWN;
    s.badReceiver = rexp.cap(7);

    return new SvcAdsMessage(s);
}

SvcLrLsMessage * svcLrLs(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(LR|ЛР)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     "\\s+"
                     "(LS|ЛС)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcLrLsStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.received = rexp.cap(3);
    s.sent = rexp.cap(5);

    return new SvcLrLsMessage(s);
}

SvcLrExpMessage * svcLrExp(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(LR|ПОЛУЧЕН)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     "\\s+"
                     "(EXP|ОЖИДАЛСЯ)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcLrExpStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.received = rexp.cap(3);
    s.expected = rexp.cap(5);

    return new SvcLrExpMessage(s);
}

SvcMisChMessage * svcMisCh(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(MIS|МИС)"
                     "\\s+"
                     "(CH|ЦХ)((\\d{2})(\\d{2}))?"
                     "\\s+"
                     "(LR|ЛР)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcMisChStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    if (!rexp.cap(4).isEmpty())
        s.timeExpected = QTime(rexp.cap(5).toInt(), rexp.cap(6).toInt());
    s.transmissionIdentificator = rexp.cap(8);

    return new SvcMisChMessage(s);
}

SvcQtaAdsMessage * svcQtaAds(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(ADS|АДС)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     "\\s+"
                     "(CORRUPT|ИСКАЖЕНО)"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaAdsStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.transmissionIdentificator = rexp.cap(4);

    return new SvcQtaAdsMessage(s);
}

SvcQtaMisMessage * svcQtaMis(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(MIS|МИС)"
                     "\\s+"
                     "([A-ZА-Я]{3})(\\d{3})(-(\\d{3}))?"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaMisStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.aftnChannel = rexp.cap(4);
    s.numFrom = rexp.cap(5).toInt();
    if (rexp.cap(6).isEmpty())
        s.numTill = s.numFrom;
    else
        s.numTill = rexp.cap(7).toInt();

    return new SvcQtaMisMessage(s);
}

SvcQtaMsrMessage * svcQtaMsr(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(MSR|МСР)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaMsrStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.transmissionIdentificator = rexp.cap(4);

    return new SvcQtaMsrMessage(s);
}

SvcQtaOgnMessage * svcQtaOgn(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(OGN|ОГН)"
                     "\\s+"
                     "([A-ZА-Я]{3}\\d{3})"
                     "\\s+"
                     "(CORRUPT|ИСКАЖЕНО)"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaOgnStruct s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.transmissionIdentificator = rexp.cap(4);

    return new SvcQtaOgnMessage(s);
}

SvcQtaRptMessage * svcQtaRpt(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(RPT|РПТ)"
                     "\\s+"
                     "(\\d{2})(\\d{2})(\\d{2})"
                     "\\s+"
                     "([A-ZА-Я]{8})"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaRptStruct s;

    const QDateTime now = QDateTime::currentDateTimeUtc();

    s.version = ParseVersion::textVersion(data.left(3));
    s.dateTime = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(4).toInt())
                           , QTime(rexp.cap(5).toInt(), rexp.cap(6).toInt()), Qt::UTC);
    s.address = rexp.cap(7);

    return new SvcQtaRptMessage(s);
}

SvcQtaRptMessage2 * svcQtaRpt2(const QString &data)
{
    QRegExp rexp(QString::fromUtf8(
                     "(SVC|СЖЦ)"
                     "\\s+"
                     "(QTA|ЩТА)"
                     "\\s+"
                     "(RPT|РПТ)"
                     "\\s+"
                     "([A-ZА-Я]{3})(\\d{3})(-(\\d{3}))?"
                     ));
    if (!rexp.exactMatch(data))
        return 0;

    SvcQtaRptStruct2 s;

    s.version = ParseVersion::textVersion(data.left(3));
    s.aftnChannel = rexp.cap(4);
    s.numFrom = rexp.cap(5).toInt();
    if (rexp.cap(6).isEmpty())
        s.numTill = s.numFrom;
    else
        s.numTill = rexp.cap(7).toInt();

    return new SvcQtaRptMessage2(s);
}

} // namespace SvcParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
