#include "AtsNonStandardMessagesParser.h"

#include <QtCore/QDate>
#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace AtsNonStandardMessagesParser
{

// returns true, if ++field wanted, else - false
typedef bool (*ProcessFieldCallback)(
        const QString &field, QVariantMap &map, NonStandardMessageErrors &errorCodes);

typedef void (*ValidateFieldCallback)(
        QVariantMap &map, NonStandardMessageErrors &errorCodes);

#define MAXFIELDS 23
struct FieldProcessor
{
    const QString type;
    ProcessFieldCallback process[MAXFIELDS];
    ValidateFieldCallback validate[MAXFIELDS];
};

static const FieldProcessor fieldProcessor[] = {
    {
        gplSignature
        , {processMessageType, processGplStartTime, processGplFinishTime, 0}
        , {validateMessageType, validateGplStartTime, validateGplFinishTime, 0}
    },

    {
        rtcSignature
        , {processMessageType, processRtcResDateTime, 0}
        , {validateMessageType, validateRtcResDateTime, 0}
    },

    {
        resSignature
        , {processMessageType, processRtcResDateTime, 0}
        , {validateMessageType, validateRtcResDateTime, 0}
    },

    {
        QString::null
        , {0}
        , {0}
    }
};

QVariantMap processMessage(const QString &message, NonStandardMessageErrors &errorCodes)
{
    QVariantMap ret;
    try {
        int beginMessageIndex = message.indexOf(M_BEGIN);
        if (beginMessageIndex < 0)
            throw ParserErrors::InvalidMessage;

        int endMessageIndex = message.lastIndexOf(M_END);
        if (endMessageIndex < beginMessageIndex)
            throw ParserErrors::InvalidMessage;

        QString trimmedMessage = message.mid(beginMessageIndex + 1
                                             , endMessageIndex - beginMessageIndex - 1);
        if (trimmedMessage.isEmpty())
            throw ParserErrors::InvalidMessage;

        trimmedMessage.replace("&", "\n");
        trimmedMessage.replace("–", "-"); // not equal !!

        ret.insert(SOURCE_MESSAGE, M_BEGIN + trimmedMessage + M_END);

        QStringList fields = trimmedMessage.split("-");
        if (fields.isEmpty())
            throw ParserErrors::InvalidMessage;

        ret.insert(SOURCE_FIELDS_LIST, fields);

        QString type = fields[0].mid(0, 3);

        bool found = false;
        int i = 0;

        while (!fieldProcessor[i].type.isNull()) {
            if (fieldProcessor[i].type == type) {
                found = true;

                int j = 0;
                int field = 0;

                while (fieldProcessor[i].process[j] && (field < fields.count())) {
                    bool next = fieldProcessor[i].process[j](fields[field], ret
                                                             , errorCodes);
                    fieldProcessor[i].validate[j](ret, errorCodes);
                    if (next)
                        ++field;
                    ++j;
                }

                break;
            } else {
                ++i;
            }
        }

        if (!found)
            throw ParserErrors::UnknownMessage;
    }
    catch (ParserErrors::NonStandardMessageError error) {
        errorCodes.append(error);
        return ret;
    }
    catch (...) {
        errorCodes.append(ParserErrors::UnhandledError);
        return ret;
    }

    return ret;
}

bool processMessageType(const QString &field, QVariantMap &map
                        , NonStandardMessageErrors &errorCodes)
{
    QRegExp reg("(GPL|RTC|RES)");
    if (reg.exactMatch(field)) {
        map.insert(MESSAGE_TYPE, reg.cap(1));
    } else {
        if (reg.indexIn(field)) {
            map.insert(MESSAGE_TYPE, reg.cap(1));
            errorCodes.append(ParserErrors::ErrorMessageType);
        } else {
            errorCodes.append(ParserErrors::ErrorMessageType);
            return false;
        }
    }
    return true;
}
void validateMessageType(QVariantMap &map, NonStandardMessageErrors &errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}


bool processGplStartTime(const QString &field, QVariantMap &map
                         , NonStandardMessageErrors &errorCodes)
{
    QRegExp reg("(\\d{2})"  // yy
                "(\\d{2})"  // MM
                "(\\d{2})"  // dd
                );
    if (reg.exactMatch(field)) {
        const QDate date = QDate::fromString(field, "yyMMdd");
        map.insert(GPL_START_TIME, date);
    } else {
        errorCodes.append(ParserErrors::ErrorGplStartTime);
    }
    return true;
}
void validateGplStartTime(QVariantMap &map, NonStandardMessageErrors &errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

bool processGplFinishTime(const QString &field, QVariantMap &map
                          , NonStandardMessageErrors &errorCodes)
{
    QRegExp reg("(\\d{2})"  // yy
                "(\\d{2})"  // MM
                "(\\d{2})"  // dd
                );
    if (reg.exactMatch(field)) {
        const QDate date = QDate::fromString(field, "yyMMdd");
        map.insert(GPL_FINISH_TIME, date);
    } else {
        errorCodes.append(ParserErrors::ErrorGplFinishTime);
    }
    return true;
}
void validateGplFinishTime(QVariantMap &map, NonStandardMessageErrors &errorCodes)
{
    if (map.contains(GPL_START_TIME) && map.contains(GPL_FINISH_TIME)) {
        const QDate startTime = map.value(GPL_START_TIME).toDate();
        const QDate finishTime = map.value(GPL_FINISH_TIME).toDate();

        if (finishTime < startTime)
            errorCodes.append(ParserErrors::InvalidGplTime);
    }
}

bool processRtcResDateTime(const QString &field, QVariantMap &map
                        , NonStandardMessageErrors &errorCodes)
{

    QRegExp rexp("(\\d{4})(\\d{2})(\\d{2})"     // date yyyyMMdd
                 "\\s"
                 "(\\d{2})(\\d{2})(\\d{2})"     // time hhmmss
                 );
    if (!rexp.exactMatch(field)) {
        errorCodes.append(ParserErrors::ErrorRtcResDateTime);
        return true;
    }
    const QDate date(rexp.cap(1).toInt(), rexp.cap(2).toInt(), rexp.cap(3).toInt());
    const QTime time(rexp.cap(4).toInt(), rexp.cap(5).toInt(), rexp.cap(6).toInt());

    map.insert(RTC_RES_DATE, date);
    map.insert(RTC_RES_TIME, time);
    map.insert(RTC_RES_DATE_TIME, QDateTime(date, time, Qt::UTC));

    return true;
}

void validateRtcResDateTime(QVariantMap &map, NonStandardMessageErrors &errorCodes)
{
    const QDateTime dateTime = map.value(RTC_RES_DATE_TIME).toDateTime();
    if (!dateTime.isValid())
        errorCodes.append(ParserErrors::InvalidRtcResDateTime);
}

} // namespace AtsNonStandardMessagesParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
