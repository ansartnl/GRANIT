#ifndef RESTRICTIONPARSER_H
#define RESTRICTIONPARSER_H

#include "ParseErrors.h"
#include "Share.h"

#include <QtCore/QTextCodec>
#include <QtCore/QVariantMap>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

dusc(SOURCE_MESSAGE_RESTRICTION)
dusc(SOURCE_FIELDS_LIST_RESTRICTION)
dusc(TYPE_MESSAGE)
dusc(MESSAGE_ID_RESTRICTION)
dusc(TYPE_RESTRICTION)
dusc(TIME_RESTRICTION_BEGIN)
dusc(TIME_RESTRICTION_END)
dusc(FORM_RESTRICTION)
dusc(UPPER_LIMIT_RESTRICTION)
dusc(LOWER_LIMIT_RESTRICTION)
dusc(CENTER_CIRCLE_COORDINATE)
dusc(CENTER_CIRCLE_NAME)
dusc(RADIUS)
dusc(POINTS_COORDINATE)
dusc(POINTS_NAME)
dusc(NAME)

namespace RestrictionParser
{
    const QString rvmSignature("RVM");
    const QString rviSignature("RVI");

    const QString rvmSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РЖМ"));
    const QString rviSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РЖИ"));

    const QString M_BEGIN("(");
    const QString M_END(")");

    typedef QList < ParserErrors::RestrictionError > RestrictionErrors;

    QVariantMap processMessage(const QString &message, RestrictionErrors &errorCodes);

    bool processMessageType(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageType(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageId(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageId(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageTypeRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageTypeRestriction(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageTimeRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageTimeRestriction(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageFormRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageFormRestriction(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageLimitRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageLimitRestriction(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessagePointRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessagePointRestriction(QVariantMap &map, RestrictionErrors &errorCodes);

    bool processMessageNameRestriction(const QString &field, QVariantMap &map
                            , RestrictionErrors &errorCodes);
    void validateMessageNameRestriction(QVariantMap &map, RestrictionErrors &errorCodes);


} // namespace RestrictionParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // RESTRICTIONPARSER_H
