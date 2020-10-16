#include "AftnMessageParser.h"
#include "private/AftnMessageParserPrivate.h"

#include "ParseErrors.h"

#include <QtDebug>

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QTextCodec>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlDriver>

#include "plvalidator.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

bool AftnMessageParserPrivate::saveOnlyFir;
bool AftnMessageParserPrivate::validateRoute;
QString AftnMessageParserPrivate::firName;

int AftnMessageParserPrivate::firIdPrivate;

QStringList AftnMessageParserPrivate::flightRules;
QStringList AftnMessageParserPrivate::flightRulesR;
QStringList AftnMessageParserPrivate::flightTypes;
QStringList AftnMessageParserPrivate::flightTypesR;

QStringList AftnMessageParserPrivate::turbulenceCategories;
QStringList AftnMessageParserPrivate::turbulenceCategoriesR;
QStringList AftnMessageParserPrivate::radioDevices;
QStringList AftnMessageParserPrivate::radioDevicesR;
QStringList AftnMessageParserPrivate::equipment;
QStringList AftnMessageParserPrivate::equipmentR;

QStringList AftnMessageParserPrivate::standardDevice12;
QStringList AftnMessageParserPrivate::radioDevices12;
QStringList AftnMessageParserPrivate::equipment12;

QStringList AftnMessageParserPrivate::standardDevice12R;
QStringList AftnMessageParserPrivate::radioDevices12R;
QStringList AftnMessageParserPrivate::equipment12R;

QStringList AftnMessageParserPrivate::borderTerms;
QStringList AftnMessageParserPrivate::borderTermsR;

QHash<QString, int> AftnMessageParserPrivate::sids;
QHash<QString, int> AftnMessageParserPrivate::stars;

QHash<QString, int> AftnMessageParserPrivate::routes;
QHash<QString, int> AftnMessageParserPrivate::points;

QHash<QString, QStringList > AftnMessageParserPrivate::routepoint;
QStringList AftnMessageParserPrivate::airports;

QString fplSignature("FPL");
QString chgSignature("CHG");
QString cnlSignature("CNL");
QString dlaSignature("DLA");
QString depSignature("DEP");
QString arrSignature("ARR");

static QString fplSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ФПЛ"));
static QString chgSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ЦХГ"));
static QString cnlSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ЦНЛ"));
static QString dlaSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ДЛА"));
static QString depSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ДЕП"));
static QString arrSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АРР"));

#define AFTN_PARSER_TBL_FPL_CNL \
        "UPDATE "       \
        AFTN_PARSER_TBL_FPL " "     \
        "SET "          \
        AFTN_PARSER_TBL_FPL_EXFPL       "='C', " \
        AFTN_PARSER_TBL_FPL_MESSAGE_ID  "=? " \
        "WHERE "                    \
        AFTN_PARSER_TBL_FPL_AIRCRAFT_ID "= ? AND " /* Поле типа 7.  a) Опознавательный индекс воздушного судна    */ \
        AFTN_PARSER_TBL_FPL_ADEP        "= ? AND " /* Поле типа 13. a) Аэродром вылета                            */ \
        AFTN_PARSER_TBL_FPL_ADES        "= ?"      /* Поле типа 16. a) Аэродром назначения                        */

#define AFTN_PARSER_TBL_FPL_DLA \
        "UPDATE "       \
        AFTN_PARSER_TBL_FPL " "     \
        "SET "          \
        AFTN_PARSER_TBL_FPL_EXFPL       "='D', " \
        AFTN_PARSER_TBL_FPL_TIME        "= ?, "   \
        AFTN_PARSER_TBL_FPL_MESSAGE_ID  "=? " \
        "WHERE "                     \
        AFTN_PARSER_TBL_FPL_AIRCRAFT_ID "= ? AND " /* Поле типа 7.  a) Опознавательный индекс воздушного судна    */ \
        AFTN_PARSER_TBL_FPL_ADEP        "= ? AND " /* Поле типа 13. a) Аэродром вылета                            */ \
        AFTN_PARSER_TBL_FPL_ADES        "= ?"      /* Поле типа 16. a) Аэродром назначения                        */

#define AFTN_PARSER_TBL_FPL_DEP \
        "UPDATE "       \
        AFTN_PARSER_TBL_FPL " "     \
        "SET "          \
        AFTN_PARSER_TBL_FPL_EXFPL       "='F', " \
        AFTN_PARSER_TBL_FPL_TIME        "= ?, "   \
        AFTN_PARSER_TBL_FPL_MESSAGE_ID  "=? " \
        "WHERE "                     \
        AFTN_PARSER_TBL_FPL_AIRCRAFT_ID "= ? AND " /* Поле типа 7.  a) Опознавательный индекс воздушного судна    */ \
        AFTN_PARSER_TBL_FPL_ADEP        "= ? AND " /* Поле типа 13. a) Аэродром вылета                            */ \
        AFTN_PARSER_TBL_FPL_ADES        "= ?"      /* Поле типа 16. a) Аэродром назначения                        */

#define AFTN_PARSER_TBL_FPL_ARR \
        "UPDATE "       \
        AFTN_PARSER_TBL_FPL " "     \
        "SET "          \
        AFTN_PARSER_TBL_FPL_EXFPL       "='A', "    \
        AFTN_PARSER_TBL_FPL_MESSAGE_ID  "=? " \
        "WHERE "                                                                                         \
        AFTN_PARSER_TBL_FPL_AIRCRAFT_ID "= ? AND " /* Поле типа 7.  a) Опознавательный индекс воздушного судна    */ \
        AFTN_PARSER_TBL_FPL_ADEP        "= ? AND " /* Поле типа 13. a) Аэродром вылета                            */ \
        AFTN_PARSER_TBL_FPL_ADES        "= ?"      /* Поле типа 16. a) Аэродром назначения                        */

#define AFTN_PARSER_TBL_NOTAM_INSERT_FORMAT \
        "INSERT INTO "              \
        AFTN_PARSER_TBL_NOTAM                  \
        "("                         \
        AFTN_PARSER_TBL_NOTAM_SERIES          ", " \
        AFTN_PARSER_TBL_NOTAM_SNUMBER         ", " \
        AFTN_PARSER_TBL_NOTAM_YEAR            ", " \
        AFTN_PARSER_TBL_NOTAM_NATURE          ", " \
        AFTN_PARSER_TBL_NOTAM_REPLACE_SERIES  ", " \
        AFTN_PARSER_TBL_NOTAM_REPLACE_SNUMBER ", " \
        AFTN_PARSER_TBL_NOTAM_REPLACE_YEAR    ", " \
        AFTN_PARSER_TBL_NOTAM_FIR             ", " \
        AFTN_PARSER_TBL_NOTAM_SUBJECT         ", " \
        AFTN_PARSER_TBL_NOTAM_CONDITION       ", " \
        AFTN_PARSER_TBL_NOTAM_TRAFFIC         ", " \
        AFTN_PARSER_TBL_NOTAM_PURPOSE         ", " \
        AFTN_PARSER_TBL_NOTAM_SCOPE           ", " \
        AFTN_PARSER_TBL_NOTAM_Q_LOWER_LIMIT   ", " \
        AFTN_PARSER_TBL_NOTAM_Q_UPPER_LIMIT   ", " \
        AFTN_PARSER_TBL_NOTAM_LOCATION        ", " \
        AFTN_PARSER_TBL_NOTAM_RADIUS          ", " \
        AFTN_PARSER_TBL_NOTAM_AERODROME       ", " \
        AFTN_PARSER_TBL_NOTAM_VALID_FROM      ", " \
        AFTN_PARSER_TBL_NOTAM_VALID_UNTIL     ", " \
        AFTN_PARSER_TBL_NOTAM_VALID_UNTIL_T   ", " \
        AFTN_PARSER_TBL_NOTAM_SCHEDULE        ", " \
        AFTN_PARSER_TBL_NOTAM_DESCRIPTION     ", " \
        AFTN_PARSER_TBL_NOTAM_LOWER_LIMIT     ", " \
        AFTN_PARSER_TBL_NOTAM_UPPER_LIMIT     ", " \
        AFTN_PARSER_TBL_NOTAM_SEQUENCE_ID     ", " \
        AFTN_PARSER_TBL_NOTAM_STATE           " "  \
        ") "      \
        "VALUES " \
        "("       \
        "?, ?, ?, ?, ?, ?, ?, ?, " \
        "?, ?, ?, ?, ?, ?, ?, ?, " \
        "?, ?, ?, ?, ?, ?, ?, ?, "   \
        "?, ?, ?" \
        ")"

//static QString qstrBaseYear()
//{
//    QString year = QString::number(QDateTime::currentDateTime().toUTC().date().year());

//    return year.left(year.size() - 2);
//}

Standard standard = FPL_2007;

static int qintBaseYear()
{
    return (QDateTime::currentDateTime().toUTC().date().year() / 100) * 100;
}

AftnMessageParserPrivate::AftnMessageParserPrivate()
{
}

AftnMessageParserPrivate::~AftnMessageParserPrivate()
{
}

AftnMessageParser::AftnMessageParser(const QSqlDatabase& database, QObject* parent) :
    QObject(parent), d_ptr(new AftnMessageParserPrivate)
{
    Q_UNUSED(database)
    //setStandard();

//    setDatabase(database.isValid() ? database : QSqlDatabase::database());
}

AftnMessageParser::~AftnMessageParser()
{
}

// анализ поля типа 1
// Тип, номер сообщения и исходные данные
// анализ поля типа 1
// Тип, номер сообщения и исходные данные
void AftnMessageParser::processType3(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(ALR|RCF|CPL|EST|CDN|ACP|LAM|RQP|RQS|SPL|CHG|CNL|DLA|DEP|ARR|FPL|PLN|PPN|ФПЛ|ЦХГ|ЦНЛ|ДЛА|ДЕП|АРР"
                 "|АЛР|ПЛН|ППН|УЛС|ФЛА|АНП|РЖМ|РЖИ|АПЗ|АПВ|TPL|ACK|REJ|SPS|СПС|AIM|ANM|АНМ|SCN|СЦН|RRP|РРП|SAM|САМ"
                 "|SLC|СЛЦ|SLT|СЛТ|CSL|ЦСЛ|ULS|FLA|RPL|РПЛ|MAN|АЦК|РЕЙ)" // a) Индекс типа сообщения
                 "(\\w{1,4}[/]\\w{1,4}\\d{3})?" // b) Номер сообщения
                 "(\\w{1,4}[/]\\w{1,4}\\d{3})?\\n?"); // c) Исходные данные

    if (rexp.exactMatch(field))
    {
        map.insert(AFTN_PARSER_MESSAGE_TYPE, rexp.cap(1));

        if (!rexp.cap(2).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_ID, rexp.cap(2));

        if (!rexp.cap(3).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_PARENT_ID, rexp.cap(3));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField3Format, QString::null));
    }
}

void AftnMessageParser::processType4Rpl(const QString &field, QVariantMap &map, AftnMessageErrors &errorCodes)
{
    //            12------23------34------41 56---67---78---89---90---01---12---25
    QRegExp rexp("((\\d{2})(\\d{2})(\\d{2}))|((1|0)(2|0)(3|0)(4|0)(5|0)(6|0)(7|0))");

    if (rexp.exactMatch(field))
    {
        if (!rexp.cap(1).isEmpty())
        {
            QDate date = QDate(rexp.cap(2).toInt() + qintBaseYear(), rexp.cap(3).toInt(), rexp.cap(4).toInt());
            if (!map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_FROM))
                map.insert(AFTN_PARSER_MESSAGE_RPL_VALID_FROM, date);
            else if (!map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_TILL))
                map.insert(AFTN_PARSER_MESSAGE_RPL_VALID_TILL, date);
            else
                errorCodes.append(AFTN_PARSER_ErrorInfo(RplInvalidField4Format, QString::null));
        } else
        {
            if (!map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_FROM)
                    || !map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_TILL))
            {
                errorCodes.append(AFTN_PARSER_ErrorInfo(RplInvalidField4Format, QString::null));
            } else
            {
                if (rexp.cap(6) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_MONDAY, rexp.cap(6));
                if (rexp.cap(7) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_TUESDAY, rexp.cap(7));
                if (rexp.cap(8) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_WEDNESDAY, rexp.cap(8));
                if (rexp.cap(9) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_THURSDAY, rexp.cap(9));
                if (rexp.cap(10) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_FRIDAY, rexp.cap(10));
                if (rexp.cap(11) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_SATURDAY, rexp.cap(11));
                if (rexp.cap(12) != "0")
                    map.insert(AFTN_PARSER_MESSAGE_RPL_SUNDAY, rexp.cap(12));
            }
        }
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField5Format, QString::null));
    }
}

void AftnMessageParser::processType5(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(INCERFA|ALERFA|DETRESFA)" // a) Аварийная стадия
                 "/"
                 "(\\w{8})"//b) Составитель сообщения
                 "/"
                 "([\\w\\s]+)"); //c) Характер аварийного положения

    if (rexp.exactMatch(field))
    {
        map.insert(AFTN_PARSER_MESSAGE_EMERGENCY_STAGE, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_EMERGENCY_SENDER, rexp.cap(2));
        map.insert(AFTN_PARSER_MESSAGE_EMERGENCY_MESSAGE, rexp.cap(3));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField5Format, QString::null));
    }
}

void AftnMessageParser::processType7(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("([\\w\\d]{1,7})" //a) Опознавательный индекс воздушного судна
                 "/?(A)?(\\d{4})?"
                 "\\n?"); //c) Код ВОРЛ

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_AIRCRAFT_ID, rexp.cap(1));

        if (!rexp.cap(2).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_VORL_A, rexp.cap(2));

        if (!rexp.cap(3).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_VORL_ID, rexp.cap(3));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField7Format, QString::null));
    }
}

void AftnMessageParser::processType8(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(\\w)" //a) Правила полетов
                 "(\\w)?"
                 "\\n?"); // b) Тип полета

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_FLIGHT_RULES, rexp.cap(1));

        if (!rexp.cap(2).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_FLIGHT_TYPE, rexp.cap(2));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField8Format, QString::null));
    }
}

void AftnMessageParser::processType9(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(\\d{1,2})?" //a) Число воздушных судов (если больше одного)
                 "([\\w\\d]{2,4})" //b) Тип воздушного судна
                 "/"
                 "(\\w)" // c) Категория турбулентности следа
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        int aircraftCount = rexp.cap(1).isEmpty() ? 1 : rexp.cap(1).toInt();
        map.insert(AFTN_PARSER_MESSAGE_AIRCRAFT_COUNT, aircraftCount);
        map.insert(AFTN_PARSER_MESSAGE_AIRCRAFT_TYPE, rexp.cap(2));
        map.insert(AFTN_PARSER_MESSAGE_TURBULENCE, rexp.cap(3));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField9Format, QString::null));
    }
}

void AftnMessageParser::processType9R(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(\\d{1,2})?" //a) Число воздушных судов (если больше одного)
                 "([\\w\\d]{2,4})" //b) Тип воздушного судна
                 "(/(\\w))?" // c) Категория турбулентности следа
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        int aircraftCount = rexp.cap(1).isEmpty() ? 1 : rexp.cap(1).toInt();
        map.insert(AFTN_PARSER_MESSAGE_AIRCRAFT_COUNT, aircraftCount);
        map.insert(AFTN_PARSER_MESSAGE_AIRCRAFT_TYPE, rexp.cap(2));
        if(!rexp.cap(4).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_TURBULENCE, rexp.cap(4));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField9Format, QString::null));
    }
}

void AftnMessageParser::processType10(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    if (standard == FPL_2012)
        return processType10v12(field, map, errorCodes);

    QRegExp rexp("(\\w+)" //      a) Средства радиосвязи, навигационные средства и средства захода на посадку
                 "/"
                 "(\\w{1,2})" // b) Оборудование наблюдения
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT, rexp.cap(0));
        map.insert(AFTN_PARSER_MESSAGE_RADIO_DEVICES, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT_MONITORING, rexp.cap(2));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField10Format, QString::null));
    }
}

void AftnMessageParser::processType10v12(const QString &field, QVariantMap &map, AftnMessageErrors &errorCodes)
{
    //Q_D( AftnMessageParser );

    QRegExp rexp(QString("((%1)((%2)+)?)/((%3)+)\\n?")
                 .arg(AftnMessageParserPrivate::standardDevice12.join("|")) // a) стандартное/не стандартное оборудование
                 .arg(AftnMessageParserPrivate::radioDevices12.join("|"))   // b) радио устройства
                 .arg(AftnMessageParserPrivate::equipment12.join("|"))      // c) оборудование
                );

    QRegExp rexpRu(QString("((%1)((%2)+)?)/((%3)+)\\n?")
                   .arg(AftnMessageParserPrivate::standardDevice12R.join("|")) // a) стандартное/не стандартное оборудование
                   .arg(AftnMessageParserPrivate::radioDevices12R.join("|"))   // b) радио устройства
                   .arg(AftnMessageParserPrivate::equipment12R.join("|"))      // c) оборудование
                   );

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT, rexp.cap(0));
        map.insert(AFTN_PARSER_MESSAGE_RADIO_DEVICES, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT_MONITORING, rexp.cap(5));
    }
    else if (rexpRu.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT, rexpRu.cap(0));
        map.insert(AFTN_PARSER_MESSAGE_RADIO_DEVICES, rexpRu.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_EQUIPMENT_MONITORING, rexpRu.cap(5));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField10Format, QString::null));
    }
}

void AftnMessageParser::processType13(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("([A-Z|А-Я]{4})" //a) Аэродром вылета
                 "(\\d{4})?" //b) Время
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_DEPARTURE_AERODROME, rexp.cap(1));

        if (!rexp.cap(2).isEmpty())
        {
            QDateTime currentDateTime = QDateTime::currentDateTime().toUTC();
            QDateTime departureTimestamp(currentDateTime.date(), QTime(), Qt::UTC);
            departureTimestamp.setTime(QTime::fromString(rexp.cap(2), "hhmm"));
            map.insert(AFTN_PARSER_MESSAGE_DEPARTURE_TIME, departureTimestamp);
        }
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField13Format, QString::null));
    }
}

void AftnMessageParser::processType14(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(\\w+)" //a) Пограничный пункт
                 "/" // ДЕЛИТЕЛЬНАЯ КОСАЯ ЧЕРТА
                 "(\\d{4})" //b) Время пролета пограничного пункта
                 "([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ПВП)" //c) Разрешенный уровень пролета
                 "([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ПВП)?" //d) Дополнительные данные о пролете
                 "(\\w)?" //e) Условия пролета
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_BORDER_POINT, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_BORDER_CROSS_TIME, rexp.cap(2));
        map.insert(AFTN_PARSER_MESSAGE_BORDER_PERMISSIBLE_LEVEL, rexp.cap(3));
        if (!rexp.cap(4).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_BORDER_LEVEL, rexp.cap(4));
        if (!rexp.cap(5).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_BORDER_TERMS, rexp.cap(5));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField14Format, QString::null));
    }
}

void AftnMessageParser::processType15(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);
    QRegExp rexp("([KNКН]\\d{4}|[MМ]\\d{3}|VFR|ЖФР|/RA/|/РА/)"// [req] a) Крейсерская скорость или число Маха
                 "([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ЖФР|ПВП)" // [req] b) Запрашиваемый крейсерский уровень
                 "\\s(.*)" // маршрут
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_REQUESTED_CRUISING_LEVEL, rexp.cap(2));
        map.insert(AFTN_PARSER_MESSAGE_ROUTE, rexp.cap(3));
        // ВЫРЕЗАЕМ НУЖНЫЙ FIR
        if (AftnMessageParserPrivate::saveOnlyFir)
        {
            QString result = cutFIRRoute(map.value(AFTN_PARSER_MESSAGE_ROUTE).toString(), errorCodes);
            if (errorCodes.isEmpty())
                map.insert(AFTN_PARSER_MESSAGE_ROUTE, result);
        }
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField15Format, QString::null));
    }
}

void AftnMessageParser::processType16(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("([A-Z|А-Я]{4})" // a
                 "(\\d{4})?" //b
                 "\\s?([A-Z|А-Я]{4})?" // c
                 "\\s?([A-Z|А-Я]{4})?" // and c
                 //"\\s?([\\w\\s]+)?"
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_DESTINATION_AERODROME, rexp.cap(1));

        if (!rexp.cap(2).isEmpty())
        {

            int hours = QVariant(rexp.cap(2).mid(0, 2)).toInt();
            int minutes = QVariant(rexp.cap(2).mid(2, 2)).toInt();

            map.insert(AFTN_PARSER_MESSAGE_ELAPSED_TIME, QString("%1:%2").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes, 2,
                                                                                                           10, QLatin1Char('0')));
        }

        if (!rexp.cap(3).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_DISPERSAL_FIELD, rexp.cap(3));

        if (!rexp.cap(4).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_DISPERSAL_FIELD_2, rexp.cap(4));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField16Format, QString::null));
    }
}

void AftnMessageParser::processType16Arr(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("([A-Z|А-Я]{4})" // a
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        processType16(field, map, errorCodes);
    }
    else
    {
        processType17(field, map, errorCodes);
        if (map.contains(AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME))
        {
            map.insert(AFTN_PARSER_MESSAGE_DESTINATION_AERODROME, map[AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME]);
        }
    }
}

void AftnMessageParser::processType17(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("([A-Z|А-Я]{4})" // a
                 "(\\d{4})" // b
                 "\\s?(.*)?" //c
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME, rexp.cap(1));
        QDateTime currentDateTime = QDateTime::currentDateTime().toUTC();
        QDateTime arrivalTimestamp(currentDateTime.date(), QTime(), Qt::UTC);
        QTime time = QTime::fromString(rexp.cap(2), "hhmm");
        arrivalTimestamp.setTime(time);
        map.insert(AFTN_PARSER_MESSAGE_ARRIVAL_TIME, arrivalTimestamp);
        if (!rexp.cap(3).isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME_NAME, rexp.cap(3));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField17Format, QString::null));
    }
}

void AftnMessageParser::processType17Fla(const QString &field, QVariantMap &map, AftnMessageErrors &errorCodes)
{
    QRegExp rexp("(\\w{4})" // a
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
        map.insert(AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME, rexp.cap(1));
    else
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField17Format, QString::null));
}

// Поле типа 18. Прочая информация: DOF
void AftnMessageParser::processType18(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(.+)"
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO, rexp.cap(1));
        // parse date of flight
        QRegExp dofRexp("(?:DOF|ДОФ)/(\\d{2})(\\d{2})(\\d{2})");
        int dofIndex = dofRexp.indexIn(rexp.cap(1));

        if (dofIndex != -1)
        {
            QString dd = dofRexp.cap(3);
            QString mm = dofRexp.cap(2);
            QString yy = dofRexp.cap(1);

            QDate dateOfFlight = QDate(qintBaseYear() + yy.toInt(), mm.toInt(), dd.toInt());
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF, dateOfFlight);
            QDateTime departureTimestamp = map.value(AFTN_PARSER_MESSAGE_DEPARTURE_TIME).toDateTime();
            departureTimestamp.setDate(dateOfFlight);

            map.insert(AFTN_PARSER_MESSAGE_DEPARTURE_TIME, departureTimestamp);
        }

        // parse eet for flight FIR's
        QRegExp eetRexp("(?:EET|ЕЕТ)/(\\w{4}\\d{4}\\s*)*");

        int eetIndex = eetRexp.indexIn(rexp.cap(1));

        if (eetIndex != -1)
        {
            // found other eet field
            QStringList firs = eetRexp.cap(0).remove(QRegExp("EET/|ЕЕТ/")).split(' ');
            QVariantMap entryEETs;
            foreach (const QString& fir, firs)
            {
                // if next field, break
                if (fir.contains("/"))
                    break;

                int hours = QVariant(fir.mid(4, 4).mid(0, 2)).toInt();
                int minutes = QVariant(fir.mid(4, 4).mid(2, 2)).toInt();
                QString eet =
                        QString("%1:%2").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes,
                                                                                 2, 10, QLatin1Char('0'));

                entryEETs.insert(fir.mid(0, 4), eet);

            }
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_FIR_EET, entryEETs);
        }

        // parse STS(СТС)
        QRegExp stsRexp("(?:STS|СТС)/(VIP |A |K |HEAD |А |К |ХЕАД |VIP$|A$|K$|HEAD$|А$|К$|ХЕАД$)");
        int stsIndex = 0;
        QVariantList entrySTSs;

        while ((stsIndex = stsRexp.indexIn(rexp.cap(1), stsIndex)) != -1)
        {
            if (stsRexp.cap(1).contains("HEAD"))
                entrySTSs << "A";
            else if (stsRexp.cap(1).contains("ХЕАД"))
                entrySTSs << "А";
            else if (stsRexp.cap(1).contains("VIP"))
                entrySTSs << "VIP";
            else
                entrySTSs << stsRexp.cap(1).left(1);

            stsIndex += stsRexp.matchedLength();
        }

        if (!entrySTSs.isEmpty())
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_STS, entrySTSs.last());

        // parse REG
        QRegExp regRexp("(?:REG|РЕГ)/(\\w* |\\w*$)");

        if (regRexp.indexIn(rexp.cap(1)) != -1)
            map.insert(AFTN_PARSER_MESSAGE_OTHER_BOARD_NUMBER, regRexp.cap(1));

        // parse date
        QRegExp dateRexp("(?:DATE|ДАТА)/(\\d{2})(\\d{2})");
        dofIndex = dateRexp.indexIn(rexp.cap(1));

        if ((dofIndex != -1) && !(map.contains(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF)))
        {
            QDate dateOfFlight = QDate(QDateTime::currentDateTimeUtc().date().year()
                                       , dateRexp.cap(2).toInt()
                                       , dateRexp.cap(1).toInt());
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF, dateOfFlight);
            QDateTime departureTimestamp = map.value(AFTN_PARSER_MESSAGE_DEPARTURE_TIME).toDateTime();
            departureTimestamp.setDate(dateOfFlight);

            map.insert(AFTN_PARSER_MESSAGE_DEPARTURE_TIME, departureTimestamp);
        }

        // APZ airport
        QRegExp otherAirport("(\\w{4})(\\d{2})(\\d{2})(/(\\d{2})(\\d{2}))?");
        if (otherAirport.indexIn(rexp.cap(1)) != -1)
        {
            QDate date = QDateTime::currentDateTimeUtc().date();
            if (map.contains(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF))
                date = map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF).toDate();
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_AIRPORT, otherAirport.cap(1));
            map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_START_TIME
                       , QDateTime(date, QTime(otherAirport.cap(2).toInt(), otherAirport.cap(3).toInt()), Qt::UTC));
            if (!otherAirport.cap(4).isNull())
            {
                map.insert(AFTN_PARSER_MESSAGE_OTHER_INFO_FINISH_TIME
                           , QDateTime(date, QTime(otherAirport.cap(5).toInt(), otherAirport.cap(6).toInt()), Qt::UTC));
            }

        }
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField18Format, QString::null));
    }
}

void AftnMessageParser::processType19(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(.+)"
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_ADDITIONAL_INFO, rexp.cap(1));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField19Format, QString::null));
    }
}

void AftnMessageParser::processType20(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(.+)" //all
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_RESCUE_INFO, rexp.cap(1));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField20Format, QString::null));
    }
}

void AftnMessageParser::processType21(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp rexp("(\\d{4})" //a
                 "\\s"
                 "([\\d\\.]{5})" //b
                 "\\s"
                 "(\\w+)" //c
                 "\\s"
                 "(\\d{4})" //d
                 "\\s"
                 "(.+)" //e and f
                 "\\n?");

    if (rexp.exactMatch(field.simplified()))
    {
        map.insert(AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME, rexp.cap(1));
        map.insert(AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_FREQUENCY, rexp.cap(2));
        map.insert(AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_LOCATION, rexp.cap(3));
        map.insert(AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME_LOCATION, rexp.cap(4));
        map.insert(AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_POSSIBLE_LINK, rexp.cap(5));
    }
    else
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField21Format, QString::null));
    }
}

void AftnMessageParser::processType22(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    QRegExp splitter("-\\d{1,2}/");
    QRegExp rexp("(\\d{1,2})" //a
                 "/"
                 "(.+)" //b
                 "\\n?");
    int i = 0;
    QString current = field.section(splitter, i, i, QString::SectionIncludeLeadingSep);

    // "Workaround bug with QString::section() in windows"
    int fieldCount = field.split(splitter).count();

    QVariantMap changedMap;

    //while (!current.isNull()) {
    while (fieldCount)
    {
        --fieldCount;

        if (rexp.exactMatch(current))
        {
            int fieldNumber = rexp.cap(1).toInt();
            QString newFieldValue = rexp.cap(2);

            switch (fieldNumber)
            {
            case 3:
                break;
            case 5:
                break;
            case 7:
                processType7(newFieldValue, changedMap, errorCodes);
                validateType7(changedMap, errorCodes);
                break;
            case 8:
                processType8(newFieldValue, changedMap, errorCodes);
                validateType8(changedMap, errorCodes);
                break;
            case 9:
                processType9(newFieldValue, changedMap, errorCodes);
                validateType9(changedMap, errorCodes);
                break;
            case 10:
                processType10(newFieldValue, changedMap, errorCodes);
                validateType10(changedMap, errorCodes);
                break;
            case 13:
                processType13(newFieldValue, changedMap, errorCodes);
                validateType13(changedMap, errorCodes);
                break;
            case 14:
                processType14(newFieldValue, changedMap, errorCodes);
                validateType14(changedMap, errorCodes);
                break;
            case 15:
                processType15(newFieldValue, changedMap, errorCodes);
                validateType15(changedMap, errorCodes);
                break;
            case 16:
                processType16(newFieldValue, changedMap, errorCodes);
                validateType16(changedMap, errorCodes);
                break;
            case 17:
                processType17(newFieldValue, changedMap, errorCodes);
                validateType17(changedMap, errorCodes);
                break;
            case 18:
                processType18(newFieldValue, changedMap, errorCodes);
                validateType18(changedMap, errorCodes);
                break;
            case 19:
                processType19(newFieldValue, changedMap, errorCodes);
                validateType19(changedMap, errorCodes);
                break;
            case 20:
                processType20(newFieldValue, changedMap, errorCodes);
                validateType20(changedMap, errorCodes);
                break;
            case 21:
                processType21(newFieldValue, changedMap, errorCodes);
                validateType21(changedMap, errorCodes);
                break;
            case 22:
                break;
            default:

                break;
            }
        }
        else
        {
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField22Format, QString::null));
        }

        if (!fieldCount)
            break;

        ++i;

        current = field.section(splitter, i, i, QString::SectionIncludeLeadingSep);

        current = current.mid(1);
    }

    map.insert(AFTN_PARSER_MESSAGE_CHANGED_FIELDS, changedMap);
}

void AftnMessageParser::processTooManyFields(const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(field)
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType3(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType4Rpl(QVariantMap &map, AftnMessageErrors &errorCodes)
{
    if (map.contains(AFTN_PARSER_MESSAGE_RPL_MONDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_TUESDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_WEDNESDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_THURSDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_FRIDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_SATURDAY)
            || map.contains(AFTN_PARSER_MESSAGE_RPL_SUNDAY))
        return;

    if (map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_FROM)
            && map.contains(AFTN_PARSER_MESSAGE_RPL_VALID_TILL))
    {
        QDate f = map.value(AFTN_PARSER_MESSAGE_RPL_VALID_FROM).toDate();
        QDate t = map.value(AFTN_PARSER_MESSAGE_RPL_VALID_TILL).toDate();
        if (f >= t)
            errorCodes.append(AFTN_PARSER_ErrorInfo(RplInvalidValidTime, QString::null));
        if (t < QDateTime::currentDateTimeUtc().date())
            errorCodes.append(AFTN_PARSER_ErrorInfo(RplValidTimeTooOld, QString::null));
    }
}

void AftnMessageParser::validateType5(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType7(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType8(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);
    QString flightRule = map.value(AFTN_PARSER_MESSAGE_FLIGHT_RULES).toString();
QStringList f = AftnMessageParserPrivate::flightRules;
    if (!AftnMessageParserPrivate::flightRules.contains(flightRule) && !AftnMessageParserPrivate::flightRulesR.contains(flightRule))
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidFlightRules, flightRule));

    if (map.contains(AFTN_PARSER_MESSAGE_FLIGHT_TYPE))
    {
        QString flightType = map.value(AFTN_PARSER_MESSAGE_FLIGHT_TYPE).toString();

        if (!AftnMessageParserPrivate::flightTypes.contains(flightType) && !AftnMessageParserPrivate::flightTypesR.contains(flightType))
        {
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidFlightType, flightType));
        }
    }
}

void AftnMessageParser::validateType9(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);
    QString turbulence = map.value(AFTN_PARSER_MESSAGE_TURBULENCE).toString();

    if (!AftnMessageParserPrivate::turbulenceCategories.contains(turbulence) && !AftnMessageParserPrivate::turbulenceCategoriesR.contains(turbulence))
        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidTurbulenceCategory, turbulence));
}

void AftnMessageParser::validateType9R(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);

    Q_UNUSED(map);
    Q_UNUSED(errorCodes);
    //Q_UNUSED(d);
}

void AftnMessageParser::validateType10(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    if (standard == FPL_2012)
        return validateType10v12(map, errorCodes);

    //Q_D( AftnMessageParser);
    QString radioDevices = map.value(AFTN_PARSER_MESSAGE_RADIO_DEVICES).toString();
    foreach(QChar current, radioDevices)
    {
        if (!AftnMessageParserPrivate::radioDevices.contains(current) && !AftnMessageParserPrivate::radioDevicesR.contains(current))
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRadioDevice, current));
    }
    QString equipment = map.value(AFTN_PARSER_MESSAGE_EQUIPMENT_MONITORING).toString();
    foreach(QChar current, equipment)
    {
        if (!AftnMessageParserPrivate::equipment.contains(current) && !AftnMessageParserPrivate::equipmentR.contains(current))
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidEquipment, current));
    }
}

void AftnMessageParser::validateType10v12(QVariantMap &map, AftnMessageErrors &errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
    if (standard == FPL_2012)
    {

    }
}

void AftnMessageParser::validateType13(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    if (map.contains(AFTN_PARSER_MESSAGE_DEPARTURE_TIME))
    {
        if (!map.value(AFTN_PARSER_MESSAGE_DEPARTURE_TIME).toDateTime().isValid())
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidDepartureTime,
                                            map.value(AFTN_PARSER_MESSAGE_DEPARTURE_TIME).toDateTime().toString(QLocale().dateTimeFormat())));
    }
}

void AftnMessageParser::validateType14(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);

    if (map.contains(AFTN_PARSER_MESSAGE_BORDER_CROSS_TIME))
    {
    }

    if (map.contains(AFTN_PARSER_MESSAGE_BORDER_TERMS))
        if (!AftnMessageParserPrivate::borderTerms.contains(map.value(AFTN_PARSER_MESSAGE_BORDER_TERMS).toString()) && !AftnMessageParserPrivate::borderTermsR.contains(map.value(AFTN_PARSER_MESSAGE_BORDER_TERMS).toString()))
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidBorderCrossTerm, map.value(AFTN_PARSER_MESSAGE_BORDER_TERMS).toString()));
}

void AftnMessageParser::validateType15(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D( AftnMessageParser);

    if (AftnMessageParserPrivate::saveOnlyFir)
        map.insert(AFTN_PARSER_MESSAGE_ROUTE, cutFIRRoute(map.value(AFTN_PARSER_MESSAGE_ROUTE).toString(), errorCodes));

    QStringList routeParts = map.value(AFTN_PARSER_MESSAGE_ROUTE).toString().split(QRegExp("(\\s|\\n)+"),
                                                                            QString::SkipEmptyParts);

    if (AftnMessageParserPrivate::validateRoute)
    {
        // Если нет роута или одна точка возврщаем невалидность
        if (routeParts.isEmpty())
        {
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRoute, tr("Too few fields")));
            return;
        }

        // AFTN Route validation alhorythm
        //return;
        //  Принцип
        //
        //  Смотрим точки
        //  Принадлежит ли первая точка следующий трассе.
        //  За трассой должна идти точка, с этой же трассы.
        //  Перед координатной точкой и после должны быть точки
        enum
        {
            LastNull, LastPoint, LastRoute, LastCoords, LastVFR, LastIFR, LastDCT, LastT, LastHeightPlus
                } lastItem;
        lastItem = LastNull;
        QString lastValue;
        QStringList validationQueries;
        QStringList ignoreItems;
        ignoreItems << "VFR" << "IFR" << "DCT" << "T" << "C" << "sa";
        QRegExp pointC("\\d{4}(N|S|С|Ю)\\d{5}(E|W|В|З)((/[KNКН]\\d{4}|[MМ]\\d{3})([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ПВП))?");
        QRegExp pointD("\\d{2}(N|S|С|Ю)\\d{3}(E|W|В|З)((/[KNКН]\\d{4}|[MМ]\\d{3})([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ПВП))?");
        QRegExp plusHeight("C/.*");
        QStringList::ConstIterator partIt = routeParts.constBegin();
        QStringList::ConstIterator partEnd = routeParts.constEnd() - 1;
        QStringList::ConstIterator partBegin = routeParts.constBegin();

        // обрабатываем первый элемент
        // Если маршрут взлета
        if (isSIDRoute(*partBegin))
        {
            ++partBegin;
        }

        // Обрабатываем последний элемент
        // Если маршрут посадки
        if (isSTARRoute(*partEnd))
        {
        }
        else
        {
            // иначе обрабатываем как обычно
            ++partEnd;
        }

        // Будем обрабатывать точку как обычно
        for (partIt = partBegin; partIt != partEnd; ++partIt)
        {
            QStringList item = partIt->split("/");
            const QString& itemFront = item.front();

            // Игнорируем некоторые элементы
            if (ignoreItems.contains(itemFront))
            {
                continue;
            }

            // Похоже на координаты
            if (pointC.exactMatch(itemFront))
            {
                // Если перед координатной точкой нет обычной - это ошибка
                if (lastItem != LastPoint && lastItem != LastCoords && lastItem != LastNull)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItemSequence, QString("%1 -> %2").arg(lastValue,
                                                                                                      *partIt)));
                }

                lastItem = LastCoords;
                // Похоже на координаты
            }
            else if (pointD.exactMatch(itemFront))
            {
                // Если перед координатной точкой нет обычной - это ошибка
                if (lastItem != LastPoint && lastItem != LastCoords && lastItem != LastNull)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItemSequence, QString("%1 -> %2").arg(lastValue,
                                                                                                      *partIt)));
                }

                lastItem = LastCoords;
                // Если данный элемент - точка
            }
            else if (isPoint(itemFront))
            {
                // Проверяем принадлежность данной точки предыдущей трассе, если трасса была.
                if (lastItem == LastRoute)
                {
                    validationQueries.append(QString("SELECT count(*) FROM route_point rp"
                                                     " JOIN point p ON rp.point_id = p.id AND p.name = '%1'"
                                                     " JOIN route r ON rp.route_id = r.id AND r.name = '%2'"
                                                     " ").arg(itemFront, lastValue));
                }

                lastItem = LastPoint;
                // Если данный элемент - трасса
            }
            else if (isRoute(itemFront))
            {
                // Проверяем принадлежность данной трассы предыдущей точке, если она была.
                if (lastItem == LastPoint)
                {
                    validationQueries.append(QString("SELECT count(*) FROM route_point rp"
                                                     " JOIN point p ON rp.point_id = p.id AND p.name = '%1'"
                                                     " JOIN route r ON rp.route_id = r.id AND r.name = '%2'"
                                                     " ").arg(lastValue, itemFront));
                }
                else
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItemSequence, QString("%1 -> %2").arg(lastValue,
                                                                                                      *partIt)));
                }

                lastItem = LastRoute;
                // Если данный элемент непонятен нам, пишем ошибку и пропускаем
            }
            else
            {
                errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItem, *partIt));
                continue;
            }

            lastValue = itemFront;
        }

        partIt = partEnd;
        lastItem = LastNull;
        lastValue.clear();

        while (partIt != partBegin)
        {
            --partIt;
            QStringList item = partIt->split("/");
            const QString& itemFront = item.front();

            // Игнорируем некоторые элементы
            if (ignoreItems.contains(itemFront))
            {
                continue;
            }

            // Похоже на координаты
            if (pointC.exactMatch(itemFront))
            {
                // Если перед координатной точкой, нет любой точки - это ошибка
                if (lastItem != LastPoint && lastItem != LastCoords && lastItem != LastNull)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItemSequence, QString("%1 -> %2").arg(lastValue,
                                                                                                      *partIt)));
                }

                lastItem = LastCoords;
                // Похоже на координаты
            }
            else if (pointD.exactMatch(itemFront))
            {
                // Если перед координатной точкой, нет любой точки - это ошибка
                if (lastItem != LastPoint && lastItem != LastCoords && lastItem != LastNull)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRouteItemSequence, QString("%1 -> %2").arg(lastValue,
                                                                                                      *partIt)));
                }

                lastItem = LastCoords;
            }

            lastValue = *partIt;
        }

//        if (!validationQueries.isEmpty())
//        {
//            QSqlQuery query(AftnMessageParserPrivate::database);
//            if (query.exec(validationQueries.join(" UNION ")))
//            {
//                while (query.next())
//                {
//                    if (!query.value(0).toInt())
//                        errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRoutePointPair, QString::null));
//                }
//            }
//        }
    }
}

void AftnMessageParser::validateType16(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
    if (map.contains(AFTN_PARSER_MESSAGE_ELAPSED_TIME))
    {
    }
}

void AftnMessageParser::validateType16Arr(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    validateType16(map, errorCodes);
    validateType17(map, errorCodes);
}

void AftnMessageParser::validateType17(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    if (map.contains(AFTN_PARSER_MESSAGE_ARRIVAL_TIME))
    {
        if (!map.value(AFTN_PARSER_MESSAGE_ARRIVAL_TIME).toDateTime().isValid())
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidArrivalTime,
                                            map.value(AFTN_PARSER_MESSAGE_ARRIVAL_TIME).toDateTime().toString(QLocale().dateTimeFormat())));
//        else if (map.value(AFTN_MESSAGE_ARRIVAL_TIME).toDateTime() < QDateTime::currentDateTimeUtc())
//            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidArrivalTime,
//                                            map.value(AFTN_MESSAGE_ARRIVAL_TIME).toDateTime().toString(QLocale().dateTimeFormat())));
    }
}

void AftnMessageParser::validateType18(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    //Q_D(AftnMessageParser);

    if (standard == FPL_2012)
        validateType18v12(map, errorCodes);

    if (map.contains(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF))
    {
        if (!map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF).toDate().isValid())
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidDateOfFLight,
                                            map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF).toDateTime().toString(QLocale().dateTimeFormat())));
        else if (map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF).toDate() < QDateTime::currentDateTimeUtc().date().addDays(-1))
            errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidDateOfFLight,
                                            map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_DOF).toDateTime().toString(QLocale().dateTimeFormat())));
        if (map.value(AFTN_PARSER_MESSAGE_ARRIVAL_TIME).toDateTime().isValid())
            if (map.value(AFTN_PARSER_MESSAGE_ARRIVAL_TIME).toDateTime() < QDateTime::currentDateTimeUtc())
                errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidArrivalTime,
                                                map.value(AFTN_PARSER_MESSAGE_ARRIVAL_TIME).toDateTime().toString(QLocale().dateTimeFormat())));
    }
#if defined(DUSHANBE_BUILD) //Pasha inconsistency of plans and ARINC in Dushanbe, temporary
#else
    if (!AftnMessageParserPrivate::airports.contains(map.value(AFTN_PARSER_MESSAGE_DEPARTURE_AERODROME).toString()))
    {
        if(AftnMessageParserPrivate::firName.size())
        {
            QVariantMap firEET = map.value(AFTN_PARSER_MESSAGE_OTHER_INFO_FIR_EET).toMap();
            if (!firEET.contains(AftnMessageParserPrivate::firName.split(",").at(0)))
                errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidFirEet, ""));
        }
    }
#endif
}

void AftnMessageParser::validateType18v12(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    if (standard == FPL_2012)
    {
        if (map.contains(AFTN_PARSER_MESSAGE_RADIO_DEVICES))
        {
            const QString radioDevices = map.value(AFTN_PARSER_MESSAGE_RADIO_DEVICES).toString();
            const QString sOther = map.value(AFTN_PARSER_MESSAGE_OTHER_INFO).toString();
            if(radioDevices.contains("R"))
            {
                QRegExp rdRexp("(PBN)/");
                int rdIndex = rdRexp.indexIn(sOther);
                if(rdIndex == -1)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField18Format, QString::null));
                }
            }
            if(radioDevices.contains("Z"))
            {
                QRegExp rdRexp("(COM|NAV|DAT)/");
                int rdIndex = rdRexp.indexIn(sOther);
                if(rdIndex == -1)
                {
                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField18Format, QString::null));
                }
            }
//            if(radioDevices.contains("G"))
//            {
//                QRegExp rdRexp("(NAV)/");
//                int rdIndex = rdRexp.indexIn(sOther);
//                if(rdIndex == -1)
//                {
//                    errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidField18Format, QString::null));
//                }
//            }
        }
    }
}

void AftnMessageParser::validateType19(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType20(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType21(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateType22(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    Q_UNUSED(errorCodes)
}

void AftnMessageParser::validateTooManyFields(QVariantMap& map, AftnMessageErrors& errorCodes)
{
    Q_UNUSED(map)
    errorCodes.append(AFTN_PARSER_ErrorInfo(TooManyFields, QString::null));
}

QString AftnMessageParser::errorString(const AFTN_PARSER_ErrorInfo& error) const
{
    QString result = tr("Unknown error");

    switch (error.first)
    {
    case UnknownError:
        result = tr("Unknown error");
        break;
    case Success:
        result = tr("Success");
        break;
    case InvalidMessage:
        result = tr("Invalid message");
        break;
    case InvalidMessageFormat:
        result = tr("Invalid message format");
        break;
    case InvalidField3Format:
        result = tr("Invalid field of type 3 (messasge type, number and reference data)");
        break;
    case InvalidField5Format:
        result = tr("Invalid field of type 5 (description of emergency)");
        break;
    case InvalidField7Format:
        result = tr("Invalid field of type 7 (Aircraft identification and SSR mode and code)");
        break;
    case InvalidField8Format:
        result = tr("Invalid field of type 8 (Flight rules and type of flight)");
        break;
    case InvalidField9Format:
        result = tr("Invalid field of type 9 (Number and type of aircraft and wake turbulence category)");
        break;
    case InvalidField10Format:
        result = tr("Invalid field of type 10 (Equipment)");
        break;
    case InvalidField13Format:
        result = tr("Invalid field of type 13 (Departure aerodrome and time)");
        break;
    case InvalidField14Format:
        result = tr("Invalid field of type 14 (Estimate data)");
        break;
    case InvalidField15Format:
        result = tr("Invalid field of type 15 (Route)");
        break;
    case InvalidField16Format:
        result = tr(
                "Invalid field of type 16 (Destination aerodrome and total estimated elapsed time, alternate aerodrome(s)");
        break;
    case InvalidField17Format:
        result = tr("Invalid field of type 17 (Arrival aerodrome and time)");
        break;
    case InvalidField18Format:
        result = tr("Invalid field of type 18 (Other information)");
        break;
    case InvalidField19Format:
        result = tr("Invalid field of type 19 (Supplementary information)");
        break;
    case InvalidField20Format:
        result = tr("Invalid field of type 20 (Alerting search and rescue information)");
        break;
    case InvalidField21Format:
        result = tr("Invalid field of type 21 (Radio failure information)");
        break;
    case InvalidField22Format:
        result = tr("Invalid field of type 22 (Amendment)");
        break;
    case TooManyFields:
        result = tr("Too many fields");
        break;
    case InvalidFlightRules:
        result = tr("Invalid flight rules");
        break;
    case InvalidFlightType:
        result = tr("Invalid flight type");
        break;
    case InvalidTurbulenceCategory:
        result = tr("Invalid turbulence category");
        break;
    case InvalidRadioDevice:
        result = tr("Invalid radio device");
        break;
    case InvalidEquipment:
        result = tr("Invalid equipment");
        break;
    case InvalidBorderCrossTime:
        result = tr("Invalid border cross time");
        break;
    case InvalidBorderCrossTerm:
        result = tr("Invalid border cross term");
        break;
    case InvalidDepartureTime:
        result = tr("Invalid departure time");
        break;
    case InvalidRoute:
        result = tr("Invalid route");
        break;
    case InvalidRouteItem:
        result = tr("Invalid route item");
        break;
    case InvalidRouteItemSequence:
        result = tr("Invalid route item sequence");
        break;
    case InvalidRoutePointPair:
        result = tr("Invalid route point pair");
        break;
    case NeededRouteNotExists:
        result = tr("Needed route not exists for FIR %1").arg(AftnMessageParserPrivate::firName.split(",").at(0));
        break;
    case InvalidElapsedTime:
        result = tr("Invalid elapsed time");
        break;
    case InvalidArrivalTime:
        result = tr("Invalid arrival time");
        break;
    case InvalidDateOfFLight:
        result = tr("Invalid date of flight");
        break;
    case InvalidFirEet:
        result = tr("Invalid FIR EET");
        break;
    case SqlErrorInsertFpl:
        result = tr("Sql error when inserting FPL");
        break;
    case SqlErrorInsertChg:
        result = tr("Sql error when changing FPL");
        break;
    case SqlErrorChgFplNotFound:
        result = tr("Sql error FPL for changing not exists");
        break;
    case SqlErrorInsertCnl:
        result = tr("Sql error when cancelling FPL");
        break;
    case SqlErrorCnlFplNotFound:
        result = tr("Sql error FPL for cancelling not exists");
        break;
    case SqlErrorInsertDla:
        result = tr("Sql error when delaying FPL");
        break;
    case SqlErrorDlaFplNotFound:
        result = tr("Sql error FPL for delaying not exists");
        break;
    case SqlErrorInsertDep:
        result = tr("Sql error when departure FPL");
        break;
    case SqlErrorDepFplNotFound:
        result = tr("Sql error FPL for departure not exists");
        break;
    case SqlErrorInsertArr:
        result = tr("Sql error when arriving FPL");
        break;
    case SqlErrorArrFplNotFound:
        result = tr("Sql error FPL for arriving not exists");
        break;
    case SqlErrorInsertNotam:
        result = tr("Sql error when inserting NOTAM");
        break;
    case SqlErrorNotamForReplacingNotExists:
        result = tr("NOTAM for replacing not exists");
        break;
    case NotamTimeExpired:
        result = tr("NOTAM time is expired");
        break;
    case NotamFieldQRequired:
        result = tr("NOTAM field Q required");
        break;
    case NotamFieldARequired:
        result = tr("NOTAM field A required");
        break;
    case NotamFieldBRequired:
        result = tr("NOTAM field B required");
        break;
    case NotamFieldCRequired:
        result = tr("NOTAM field C required");
        break;
    case NotamFieldDRequired:
        result = tr("NOTAM field D required");
        break;
    case NotamFieldERequired:
        result = tr("NOTAM field E required");
        break;
    case SqlErrorInsertFplSFplFound:
        result = tr("FPL is active. Changing are not allowed.");
        break;
    case GeneralSqlError:
        result = tr("SQL error");
        break;
    default:
        break;
    }

    if (!error.second.isEmpty())
    {
        result += ": ";
        result += error.second;
    }

    return result;
}

QStringList AftnMessageParser::errorStrings(const AftnMessageErrors& errors) const
{
    QStringList result;
    AftnMessageErrors::ConstIterator errorIt = errors.constBegin();
    AftnMessageErrors::ConstIterator end = errors.constEnd();

    for (; errorIt != end; ++errorIt)
        result.append(errorString(*errorIt));

    return result;
}

bool AftnMessageParser::isSIDRoute(const QString& identifier)
{
    //Q_D(const AftnMessageParser);
    return AftnMessageParserPrivate::sids.contains(identifier);
}

bool AftnMessageParser::isSTARRoute(const QString& identifier)
{
    //Q_D(const AftnMessageParser);
    return  AftnMessageParserPrivate::stars.contains(identifier);
}

bool AftnMessageParser::isPoint(const QString& identifier)
{
    //Q_D(const AftnMessageParser);
    return AftnMessageParserPrivate::points.contains(identifier);
}

bool AftnMessageParser::isRoute(const QString& identifier)
{
    //Q_D(const AftnMessageParser);
    return AftnMessageParserPrivate::routes.contains(identifier);
}

qint64 AftnMessageParser::firId(const QString& identifier)
{
    Q_UNUSED(identifier)
    //Q_D(const AftnMessageParser);
    return AftnMessageParserPrivate::firIdPrivate;
}

QStringList AftnMessageParser::slice(QStringList src, int start, int end)
{
    if (start <= end && end != -1)
    {
        return src.mid(start, end - start + 1);
    } else if (end == -1) {
        return src.mid(start);
    } else {
        QStringList result;
        while (start != end)
        {
            result << src.at(start);
            --start;
        }
        result << src.at(start);
        return result;
    }
    return QStringList();
}

QString AftnMessageParser::cutFIRRoute(const QString& route, AftnMessageErrors& errorCodes)
{
    //Q_D(AftnMessageParser);
    QStringList preRouteParts = route.split(QRegExp("(\\s|\\n)+"), QString::SkipEmptyParts);
    QStringList pointsResult;

    for (int i = 0; i < preRouteParts.length(); ++i)
    {
        QString routePoint = preRouteParts.at(i).split("/").front();
        if (isRoute(routePoint))
        {
            if (i == 0)
            {
                errorCodes.append(AFTN_PARSER_ErrorInfo(ParserErrors::InvalidRoute, routePoint));
                return QString::null;
            }
            if (i + 1  == preRouteParts.length())
            {
                errorCodes.append(AFTN_PARSER_ErrorInfo(ParserErrors::InvalidRoute, routePoint));
                return QString::null;
            }

//            QStringList allroute;
//            QSqlQuery query(AftnMessageParserPrivate::database);
//            query.prepare(QString("select distinct route.name, point.name, route_point.sn from route_point "
//                                  " join route on route.id = route_point.route_id and route.name = '%1'"
//                                  " join point on point.id = route_point.point_id "
//                                  " order by route.name, route_point.sn").arg(routePoint));
//            if (query.exec())
//                while (query.next())
//                    allroute.append(query.value(1).toString());

//            QStringList routetmp = AftnMessageParserPrivate::routepoint[preRouteParts.at(i).split("/").front()];
//            //pointsResult.pop_back();
//            QString startPoint = preRouteParts.at(i - 1).split("/").front();
//            QString endPoint = preRouteParts.at(i + 1).split("/").front();
//            if (!isPoint(startPoint) && !isPoint(endPoint)) {
//                bool success = false;
//                if (allroute.indexOf(startPoint) != -1
//                    && allroute.indexOf(endPoint) != -1)
//                {
//                    QStringList tmproute = slice(allroute, allroute.indexOf(startPoint), allroute.indexOf(endPoint));

//                    foreach(const QString& point, tmproute)
//                    {
//                        if (isPoint(point))
//                        {
//                            success = true;
//                        }
//                    }
//                }
//                if (!success)
//                {
//                    pointsResult.append(preRouteParts.at(i));
//                    continue;
//                }
//            }
//            if (allroute.indexOf(startPoint) == -1)
//            {
//                errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRoute, startPoint));
//                return QString::null;
//            }

//            if (allroute.indexOf(endPoint) == -1)
//            {
//                errorCodes.append(AFTN_PARSER_ErrorInfo(InvalidRoute, endPoint));
//                return QString::null;
//            }
//            QStringList tmproute = slice(allroute, allroute.indexOf(startPoint), allroute.indexOf(endPoint));
//            QStringList resultRoute;

//            foreach(const QString& point, tmproute)
//                if (routetmp.contains(point))
//                    resultRoute.append(point);

//            pointsResult.append(resultRoute);
//            ++i;
        } else
            pointsResult.append(preRouteParts.at(i));
    }

    pointsResult.removeDuplicates();
    QStringList routeParts = pointsResult;
    if (!routeParts.count())
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(ParserErrors::InvalidRoute, route));
        return QString::null;
    }
    QStringList::ConstIterator beginRoute = routeParts.constEnd();
    QStringList::ConstIterator endRoute = routeParts.constEnd();
    // cut fir
    QStringList::ConstIterator partIt = routeParts.constBegin();
    QStringList::ConstIterator partEnd = routeParts.constEnd() - 1;
    QStringList::ConstIterator partBegin = routeParts.constBegin();

    // обрабатываем первый элемент
    // Если маршрут взлета
    if (isSIDRoute(*partBegin))
        // начинаемы вырезку
        beginRoute = partBegin;

    // Обрабатываем последний элемент
    if (isSTARRoute(*partEnd))
        // заканчиваем вырезку
    {
        endRoute = partEnd;
        ++partEnd;
    }

    for (; partIt != partEnd; ++partIt)
    {
        QStringList item = partIt->split("/");
        const QString& itemFront = item.front();

        // Если появилась наша точка
        if (isPoint(itemFront) && beginRoute == routeParts.constEnd())
        {
            beginRoute = partIt;
            break;
        }

        // Если появился наш маршрут
        if (isRoute(itemFront) && beginRoute == routeParts.constEnd())
        {
            beginRoute = partIt;
            break;
        }
    }

    partIt = routeParts.constEnd();

    while (partIt != routeParts.constBegin())
    {
        --partIt;
        QStringList item = partIt->split("/");
        const QString& itemFront = item.front();

        if (isPoint(itemFront) && (endRoute == routeParts.constEnd()))
        {
            endRoute = partIt;
            break;
        }
        if (isRoute(itemFront) && (endRoute == routeParts.constEnd()))
        {
            endRoute = partIt;
            break;
        }
    }

    if (beginRoute == routeParts.constEnd() || endRoute == routeParts.constEnd())
    {
        errorCodes.append(AFTN_PARSER_ErrorInfo(ParserErrors::NeededRouteNotExists, route));
        return QString::null;
    }

    QStringList newRoute = routeParts.mid(routeParts.indexOf(*beginRoute), routeParts.indexOf(*endRoute)
                                          - routeParts.indexOf(*beginRoute) + 1);

    /* Pasha 2012.02.07 for route points like TAHIR/M078F330
    routeParts = newRoute;

    QStringList newNewRoute;

    foreach(const QString& part, newRoute)
        if (isPoint(part) || isSIDRoute(part) || isSTARRoute(part))
            newNewRoute.append(part);

    return newNewRoute.join(" ");*/
    return newRoute.join(" ");
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
