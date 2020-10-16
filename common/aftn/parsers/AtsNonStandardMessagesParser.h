#ifndef ATSNONSTANDARDMESSAGESPARSER_H
#define ATSNONSTANDARDMESSAGESPARSER_H

#include "ParseErrors.h"
#include "Share.h"

#include <QtCore/QVariantMap>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

dusc(SOURCE_MESSAGE)
dusc(SOURCE_FIELDS_LIST)
dusc(MESSAGE_TYPE)

dusc(GPL_START_TIME)
dusc(GPL_FINISH_TIME)

dusc(RTC_RES_DATE)
dusc(RTC_RES_TIME)
dusc(RTC_RES_DATE_TIME)

namespace AtsNonStandardMessagesParser
{

const QString gplSignature("GPL");
const QString rtcSignature("RTC");
const QString resSignature("RES");

const QString M_BEGIN("(");
const QString M_END(")");

typedef QList < ParserErrors::NonStandardMessageError > NonStandardMessageErrors;

QVariantMap processMessage(const QString &message, NonStandardMessageErrors &errorCodes);

bool processMessageType(const QString &field, QVariantMap &map
                        , NonStandardMessageErrors &errorCodes);
void validateMessageType(QVariantMap &map, NonStandardMessageErrors &errorCodes);


bool processGplStartTime(const QString &field, QVariantMap &map
                         , NonStandardMessageErrors &errorCodes);
void validateGplStartTime(QVariantMap &map, NonStandardMessageErrors &errorCodes);
bool processGplFinishTime(const QString &field, QVariantMap &map
                          , NonStandardMessageErrors &errorCodes);
void validateGplFinishTime(QVariantMap &map, NonStandardMessageErrors &errorCodes);

bool processRtcResDateTime(const QString &field, QVariantMap &map
                        , NonStandardMessageErrors &errorCodes);
void validateRtcResDateTime(QVariantMap &map, NonStandardMessageErrors &errorCodes);

} // namespace AtsNonStandardMessagesParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ATSNONSTANDARDMESSAGESPARSER_H
