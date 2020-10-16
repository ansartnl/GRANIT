#ifndef AFTNMESSAGEPARSER_H
#define AFTNMESSAGEPARSER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>

#include <QtSql/QSqlDatabase>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

#define MESSAGE_BEGIN_PARSER '('
#define MESSAGE_END_PARSER   ')'

#define AFTN_PARSER_SOURCE_MESSAGE                 "source_message"
#define AFTN_PARSER_SOURCE_MESSAGE_ID              "id"

#define AFTN_PARSER_MESSAGE_FPL_ID                 "fpl_id"

//3
#define AFTN_PARSER_MESSAGE_TYPE                   "type"
#define AFTN_PARSER_MESSAGE_ID                     "id"
#define AFTN_PARSER_MESSAGE_PARENT_ID              "parent_id"

//4 for RPL

#define AFTN_PARSER_MESSAGE_RPL_VALID_FROM         "rpl_valid_from"
#define AFTN_PARSER_MESSAGE_RPL_VALID_TILL         "rpl_valid_till"
#define AFTN_PARSER_MESSAGE_RPL_MONDAY             "rpl_monday"
#define AFTN_PARSER_MESSAGE_RPL_TUESDAY            "rpl_tuesday"
#define AFTN_PARSER_MESSAGE_RPL_WEDNESDAY          "rpl_wednesday"
#define AFTN_PARSER_MESSAGE_RPL_THURSDAY           "rpl_thursday"
#define AFTN_PARSER_MESSAGE_RPL_FRIDAY             "rpl_friday"
#define AFTN_PARSER_MESSAGE_RPL_SATURDAY           "rpl_saturday"
#define AFTN_PARSER_MESSAGE_RPL_SUNDAY             "rpl_sunday"

//5
#define AFTN_PARSER_MESSAGE_EMERGENCY_STAGE        "emergency_stage"
#define AFTN_PARSER_MESSAGE_EMERGENCY_SENDER       "emergency_sender"
#define AFTN_PARSER_MESSAGE_EMERGENCY_MESSAGE      "emergency_message"

//7
#define AFTN_PARSER_MESSAGE_AIRCRAFT_ID            "aircraft_id"
#define AFTN_PARSER_MESSAGE_VORL_A                 "vorl_a"
#define AFTN_PARSER_MESSAGE_VORL_ID                "vorl_id"

//8
#define AFTN_PARSER_MESSAGE_FLIGHT_RULES           "flight_rules"
#define AFTN_PARSER_MESSAGE_FLIGHT_TYPE            "flight_type"

//9
#define AFTN_PARSER_MESSAGE_AIRCRAFT_COUNT         "aircraft_count"
#define AFTN_PARSER_MESSAGE_AIRCRAFT_TYPE          "aircraft_type"
#define AFTN_PARSER_MESSAGE_TURBULENCE             "turbulence"

//10
#define AFTN_PARSER_MESSAGE_EQUIPMENT              "equipment"
#define AFTN_PARSER_MESSAGE_RADIO_DEVICES          "radio_devices"
#define AFTN_PARSER_MESSAGE_EQUIPMENT_MONITORING   "equipment_monitoring"

//13
#define AFTN_PARSER_MESSAGE_DEPARTURE_AERODROME    "departure_aerodrome"
#define AFTN_PARSER_MESSAGE_DEPARTURE_TIME         "departure_time"

//14
#define AFTN_PARSER_MESSAGE_BORDER_POINT           "border_point"
#define AFTN_PARSER_MESSAGE_BORDER_CROSS_TIME      "border_cross_time"
#define AFTN_PARSER_MESSAGE_BORDER_PERMISSIBLE_LEVEL "border_permissible_level"
#define AFTN_PARSER_MESSAGE_BORDER_LEVEL           "border_level"
#define AFTN_PARSER_MESSAGE_BORDER_TERMS           "border_terms"

//15
//Cruising speed or Mach aircraftNumber
#define AFTN_PARSER_MESSAGE_CRUISING_SPEED_OR_MACH_NUMBER  "cruising_level_or_mach_number"
#define AFTN_PARSER_MESSAGE_REQUESTED_CRUISING_LEVEL           "requested_cruising_level"
#define AFTN_PARSER_MESSAGE_ROUTE                  "route"

//16 FPL CHG CNL DLA DEP ARR
// Destination aerodrome
#define AFTN_PARSER_MESSAGE_DESTINATION_AERODROME    "destination_aerodrome"
//Total estimated elapsed time
#define AFTN_PARSER_MESSAGE_ELAPSED_TIME             "elapsed_time"
//Dispersal field
#define AFTN_PARSER_MESSAGE_DISPERSAL_FIELD          "dispersal_field"
#define AFTN_PARSER_MESSAGE_DISPERSAL_FIELD_2        "dispersal_field_2"
#define AFTN_PARSER_MESSAGE_DISPERSAL_NAME           "dispersal_field_name"

//17 ARR
#define AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME        "arrival_aerodrome"
#define AFTN_PARSER_MESSAGE_ARRIVAL_TIME             "arrival_time"
#define AFTN_PARSER_MESSAGE_ARRIVAL_AERODROME_NAME   "arrival_aerodrome_name"

//18
#define AFTN_PARSER_MESSAGE_OTHER_INFO                     "other_info"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_DOF                 "dof"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_STS                 "sts"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_FIR_EET             "fir_eet"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_AIRPORT             "other_airport"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_START_TIME          "start_time"
#define AFTN_PARSER_MESSAGE_OTHER_INFO_FINISH_TIME         "finish_time"
#define AFTN_PARSER_MESSAGE_OTHER_BOARD_NUMBER             "board_number"

//19
#define AFTN_PARSER_MESSAGE_ADDITIONAL_INFO          "additional_info"

//20
#define AFTN_PARSER_MESSAGE_RESCUE_INFO              "rescue_info"

//21
#define AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME "refusal_radiolink_last_time"
#define AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_FREQUENCY   "refusal_radiolink_last_frequency"
#define AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_LOCATION  "refusal_radiolink_last_location"
#define AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_LAST_TIME_LOCATION "refusal_radiolink_last_time_location"
#define AFTN_PARSER_MESSAGE_REFUSAL_RADIOLINK_POSSIBLE_LINK "refusal_radiolink_possible_link"

//22
#define AFTN_PARSER_MESSAGE_CHANGE_FIELD_NUMBER      "change_field_number"
#define AFTN_PARSER_MESSAGE_CHANGE_FIELD_DATA        "change_field_data"

#define AFTN_PARSER_MESSAGE_CHANGED_FIELDS        "changed_fields"

// NOTAM
#define AFTN_PARSER_MESSAGE_BORDER_POINT           "border_point"
#define AFTN_PARSER_MESSAGE_BORDER_CROSS_TIME      "border_cross_time"
#define AFTN_PARSER_MESSAGE_BORDER_PERMISSIBLE_LEVEL "border_permissible_level"
#define AFTN_PARSER_MESSAGE_BORDER_LEVEL           "border_level"
#define AFTN_PARSER_MESSAGE_BORDER_TERMS           "border_terms"

#define AFTN_PARSER_MESSAGE_NOTAM_SERIES            "series"
#define AFTN_PARSER_MESSAGE_NOTAM_NUMBER           "number"
#define AFTN_PARSER_MESSAGE_NOTAM_YEAR               "year"
#define AFTN_PARSER_MESSAGE_NOTAM_SUFFIX            "suffix"
#define AFTN_PARSER_MESSAGE_NOTAM_REPLACE_SERIES    "replace_series"
#define AFTN_PARSER_MESSAGE_NOTAM_REPLACE_NUMBER    "replace_number"
#define AFTN_PARSER_MESSAGE_NOTAM_REPLACE_YEAR      "replace_year"
#define AFTN_PARSER_MESSAGE_NOTAM_FIR               "fir"
#define AFTN_PARSER_MESSAGE_NOTAM_ICAO              "icao"
#define AFTN_PARSER_MESSAGE_NOTAM_SUBJECT           "subject"
#define AFTN_PARSER_MESSAGE_NOTAM_CONDITION         "condition"
#define AFTN_PARSER_MESSAGE_NOTAM_TRAFFIC           "traffic"
#define AFTN_PARSER_MESSAGE_NOTAM_PURPOSE           "purpose"
#define AFTN_PARSER_MESSAGE_NOTAM_SCOPE             "scope"
#define AFTN_PARSER_MESSAGE_NOTAM_Q_LOWER_LIMIT     "qlowerlimit"
#define AFTN_PARSER_MESSAGE_NOTAM_Q_UPPER_LIMIT     "qupperlimit"
#define AFTN_PARSER_MESSAGE_NOTAM_LOCATION          "location"
#define AFTN_PARSER_MESSAGE_NOTAM_RADIUS            "radius"
#define AFTN_PARSER_MESSAGE_NOTAM_AERODROME         "aerodrome"
#define AFTN_PARSER_MESSAGE_NOTAM_VALID_FROM        "validfrom"
#define AFTN_PARSER_MESSAGE_NOTAM_VALID_UNTIL       "validuntil"
#define AFTN_PARSER_MESSAGE_NOTAM_VALID_UNTIL_TYPE  "validuntiltype"
#define AFTN_PARSER_MESSAGE_NOTAM_SCHEDULE          "schedule"
#define AFTN_PARSER_MESSAGE_NOTAM_DESCRIPTION       "description"
#define AFTN_PARSER_MESSAGE_NOTAM_LOWER_LIMIT       "lowerlimit"
#define AFTN_PARSER_MESSAGE_NOTAM_UPPER_LIMIT       "upperlimit"
#define AFTN_PARSER_MESSAGE_NOTAM_SEQUENCE_ID       "sequence_id"
#define AFTN_PARSER_MESSAGE_NOTAM_STATE             "state"

//#define TBL_ARC_TEXT        "text"

#define TBL_ARC_AFTN_PARSER         "arc_aftn"
#define TBL_ARC_AFTN_PARSER_ID      "id"
#define TBL_ARC_AFTN_PARSER_CREATED "created"
#define TBL_ARC_AFTN_PARSER_TEXT    "text"
#define TBL_ARC_AFTN_PARSER_HEADER  "header"
#define TBL_ARC_AFTN_PARSER_INSERT  "INSERT INTO " TBL_ARC_AFTN_PARSER " (" TBL_ARC_AFTN_PARSER_TEXT"," TBL_ARC_AFTN_PARSER_HEADER ") VALUES (:"TBL_ARC_AFTN_PARSER_TEXT", :"TBL_ARC_AFTN_PARSER_HEADER ")"

#define TBL_MAN_AFTN_PARSER        "man_aftn"
#define TBL_MAN_AFTN_PARSER_INSERT "INSERT INTO " TBL_MAN_AFTN_PARSER " (text, header) VALUES (:text, :header)"

#define AFTN_PARSER_TBL_FPL             "fpl"
#define AFTN_PARSER_TBL_FFPL            "ffpl"
#define AFTN_PARSER_TBL_FPL_ID          "id"
#define AFTN_PARSER_TBL_FPL_FIR_NAME    "fir_name"
#define AFTN_PARSER_TBL_FPL_AIRCRAFT_ID "aircraft_id"
#define AFTN_PARSER_TBL_FPL_FRULES      "frules"
#define AFTN_PARSER_TBL_FPL_TFLIGHT     "tflight"
#define AFTN_PARSER_TBL_FPL_NUM         "num"
#define AFTN_PARSER_TBL_FPL_TAIRCRAFT   "taircraft"
#define AFTN_PARSER_TBL_FPL_TURBULENCE_CATEGORY "wtc"
#define AFTN_PARSER_TBL_FPL_EQUIPMENT   "equipment"
#define AFTN_PARSER_TBL_FPL_ADEP        "adep"
#define AFTN_PARSER_TBL_FPL_TIME        "time"
#define AFTN_PARSER_TBL_FPL_SPEED       "speed"
#define AFTN_PARSER_TBL_FPL_FLEVEL      "flevel"
#define AFTN_PARSER_TBL_FPL_ROUTE       "route"
#define AFTN_PARSER_TBL_FPL_ADES        "ades"
#define AFTN_PARSER_TBL_FPL_EET         "eet"
#define AFTN_PARSER_TBL_FPL_ALT1        "alt1"
#define AFTN_PARSER_TBL_FPL_ALT2        "alt2"
#define AFTN_PARSER_TBL_FPL_OTHER       "other"
#define AFTN_PARSER_TBL_FPL_TRTIME      "trtime"
#define AFTN_PARSER_TBL_FPL_EXFPL       "exfpl"
#define AFTN_PARSER_TBL_FPL_VISIBILITY  "visibility"
#define AFTN_PARSER_TBL_FPL_MESSAGE_ID  "message_id"
#define AFTN_PARSER_TBL_FPL_ENTRY_FIR_EET "entry_fir_eet"

#define AFTN_PARSER_TBL_FPL_OTHER_DOF   "DOF"

#define AFTN_PARSER_TBL_RPL             "rpl"
#define AFTN_PARSER_TBL_RPL_ID          "id"
#define AFTN_PARSER_TBL_RPL_FIR_NAME    "fir_name"
#define AFTN_PARSER_TBL_RPL_AIRCRAFT_ID "aircraft_id"
#define AFTN_PARSER_TBL_RPL_FRULES      "frules"
#define AFTN_PARSER_TBL_RPL_TFLIGHT     "tflight"
#define AFTN_PARSER_TBL_RPL_NUM         "num"
#define AFTN_PARSER_TBL_RPL_TAIRCRAFT   "taircraft"
#define AFTN_PARSER_TBL_RPL_TURBULENCE_CATEGORY  "wtc"
#define AFTN_PARSER_TBL_RPL_EQUIPMENT   "equipment"
#define AFTN_PARSER_TBL_RPL_ADEP        "adep"
#define AFTN_PARSER_TBL_RPL_TIME        "time"
#define AFTN_PARSER_TBL_RPL_SPEED       "speed"
#define AFTN_PARSER_TBL_RPL_FLEVEL      "flevel"
#define AFTN_PARSER_TBL_RPL_ROUTE       "route"
#define AFTN_PARSER_TBL_RPL_ADES        "ades"
#define AFTN_PARSER_TBL_RPL_EET         "eet"
#define AFTN_PARSER_TBL_RPL_ALT1        "alt1"
#define AFTN_PARSER_TBL_RPL_ALT2        "alt2"
#define AFTN_PARSER_TBL_RPL_OTHER       "other"
#define AFTN_PARSER_TBL_RPL_TRTIME      "trtime"
#define AFTN_PARSER_TBL_RPL_DAYS          "days"
#define AFTN_PARSER_TBL_RPL_ENTRY_FIR_EET "entry_fir_eet"
#define AFTN_PARSER_TBL_RPL_VALID_FROM    "valid_from"
#define AFTN_PARSER_TBL_RPL_VALID_TO      "valid_to"

#define AFTN_PARSER_TBL_NOTAM                 "notam"
#define AFTN_PARSER_TBL_NOTAM_ID              "id"
#define AFTN_PARSER_TBL_NOTAM_SERIES          "series"
#define AFTN_PARSER_TBL_NOTAM_SNUMBER         "snumber"
#define AFTN_PARSER_TBL_NOTAM_YEAR            "year"
#define AFTN_PARSER_TBL_NOTAM_NATURE          "nature"
#define AFTN_PARSER_TBL_NOTAM_REPLACE_SERIES  "replace_series"
#define AFTN_PARSER_TBL_NOTAM_REPLACE_SNUMBER "replace_snumber"
#define AFTN_PARSER_TBL_NOTAM_REPLACE_YEAR    "replace_year"
#define AFTN_PARSER_TBL_NOTAM_FIR             "fir"
#define AFTN_PARSER_TBL_NOTAM_SUBJECT         "subject"
#define AFTN_PARSER_TBL_NOTAM_CONDITION       "condition"
#define AFTN_PARSER_TBL_NOTAM_TRAFFIC         "traffic"
#define AFTN_PARSER_TBL_NOTAM_PURPOSE         "purpose"
#define AFTN_PARSER_TBL_NOTAM_SCOPE           "scope"
#define AFTN_PARSER_TBL_NOTAM_Q_LOWER_LIMIT   "q_lower_limit"
#define AFTN_PARSER_TBL_NOTAM_Q_UPPER_LIMIT   "q_upper_limit"
#define AFTN_PARSER_TBL_NOTAM_LOCATION        "location"
#define AFTN_PARSER_TBL_NOTAM_RADIUS          "radius"
#define AFTN_PARSER_TBL_NOTAM_AERODROME       "aerodrome"
#define AFTN_PARSER_TBL_NOTAM_VALID_FROM      "valid_from"
#define AFTN_PARSER_TBL_NOTAM_VALID_UNTIL     "valid_until"
#define AFTN_PARSER_TBL_NOTAM_VALID_UNTIL_T   "valid_until_t"
#define AFTN_PARSER_TBL_NOTAM_SCHEDULE        "schedule"
#define AFTN_PARSER_TBL_NOTAM_DESCRIPTION     "description"
#define AFTN_PARSER_TBL_NOTAM_LOWER_LIMIT     "lower_limit"
#define AFTN_PARSER_TBL_NOTAM_UPPER_LIMIT     "upper_limit"
#define AFTN_PARSER_TBL_NOTAM_SEQUENCE_ID     "sequence_id"
#define AFTN_PARSER_TBL_NOTAM_STATE           "state"

#define AFTN_PARSER_TBL_FPL_ON_FUTURE          "fpl_on_future"

#define AFTN_PARSER_TBL_HIST_FPL                    "hist_fpl"
#define AFTN_PARSER_TBL_HIST_FPL_ID                 "id"
#define AFTN_PARSER_TBL_HIST_FPL_FIR_NAME           "fir_name"
#define AFTN_PARSER_TBL_HIST_FPL_AIRCRAFT_ID        "aircraft_id"
#define AFTN_PARSER_TBL_HIST_FPL_TAIRCRAFT          "taircraft"
#define AFTN_PARSER_TBL_HIST_FPL_FRULES             "frules"
#define AFTN_PARSER_TBL_HIST_FPL_TFLIGHT            "tflight"
#define AFTN_PARSER_TBL_HIST_FPL_NUM                "num"
#define AFTN_PARSER_TBL_HIST_FPL_WTC                "wtc"
#define AFTN_PARSER_TBL_HIST_FPL_EQUIPMENT          "equipment"
#define AFTN_PARSER_TBL_HIST_FPL_ADEP               "adep"
#define AFTN_PARSER_TBL_HIST_FPL_TIME               "time"
#define AFTN_PARSER_TBL_HIST_FPL_SPEED              "speed"
#define AFTN_PARSER_TBL_HIST_FPL_FLEVEL             "flevel"
#define AFTN_PARSER_TBL_HIST_FPL_ROUTE              "route"
#define AFTN_PARSER_TBL_HIST_FPL_ADES               "ades"
#define AFTN_PARSER_TBL_HIST_FPL_EET                "eet"
#define AFTN_PARSER_TBL_HIST_FPL_ALT1               "alt1"
#define AFTN_PARSER_TBL_HIST_FPL_ALT2               "alt2"
#define AFTN_PARSER_TBL_HIST_FPL_OTHER              "other"
#define AFTN_PARSER_TBL_HIST_FPL_TRTIME             "trtime"
#define AFTN_PARSER_TBL_HIST_FPL_EXFPL              "exfpl"
#define AFTN_PARSER_TBL_HIST_FPL_VISIBILITY         "visibility"
#define AFTN_PARSER_TBL_HIST_FPL_HIST_ID            "hist_id"
#define AFTN_PARSER_TBL_HIST_FPL_MODIFIED_BY        "modified_by"
#define AFTN_PARSER_TBL_HIST_FPL_MODIFIED_AT        "modified_at"
#define AFTN_PARSER_TBL_HIST_FPL_MODIFICATION_TYPE  "modification_type"
#define AFTN_PARSER_TBL_HIST_FPL_ACTUAL             "actual"
#define AFTN_PARSER_TBL_HIST_FPL_MESSAGE_ID         "message_id"
#define AFTN_PARSER_TBL_HIST_FPL_ENTRY_FIR_EET        "entry_fir_eet"

#define AFTN_PARSER_TBL_FPL_INSERT_FORMAT \
    "INSERT INTO "                \
    "%1 "                     \
    "("                       \
    AFTN_PARSER_TBL_FPL_FIR_NAME    ", "  \
    AFTN_PARSER_TBL_FPL_AIRCRAFT_ID ", "  \
    AFTN_PARSER_TBL_FPL_FRULES      ", "  \
    AFTN_PARSER_TBL_FPL_TFLIGHT     ", "  \
    AFTN_PARSER_ TBL_FPL_NUM         ", "  \
    AFTN_PARSER_TBL_FPL_TAIRCRAFT   ", "  \
    AFTN_PARSER_TBL_FPL_TURBULENCE_CATEGORY ", "  \
    AFTN_PARSER_TBL_FPL_EQUIPMENT   ", "  \
    AFTN_PARSER_TBL_FPL_ADEP        ", "  \
    AFTN_PARSER_TBL_FPL_TIME        ", "  \
    AFTN_PARSER_TBL_FPL_SPEED       ", "  \
    AFTN_PARSER_TBL_FPL_FLEVEL      ", "  \
    AFTN_PARSER_TBL_FPL_ROUTE       ", "  \
    AFTN_PARSER_TBL_FPL_ADES        ", "  \
    AFTN_PARSER_TBL_FPL_EET         ", "  \
    AFTN_PARSER_TBL_FPL_ALT1        ", "  \
    AFTN_PARSER_TBL_FPL_ALT2        ", "  \
    AFTN_PARSER_TBL_FPL_OTHER       ", "  \
    AFTN_PARSER_TBL_FPL_MESSAGE_ID  ", "  \
    AFTN_PARSER_TBL_FPL_ENTRY_FIR_EET " "   \
    ") "      \
    "VALUES " \
    "("       \
    "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?" \
    ")"

#define AFTN_PARSER_TBL_FPL_UPDATE_FORMAT    \
    "UPDATE "                    \
    "%1 "                        \
    "SET "                       \
    AFTN_PARSER_TBL_FPL_FIR_NAME    " = ?, " \
    AFTN_PARSER_TBL_FPL_AIRCRAFT_ID " = ?, " \
    AFTN_PARSER_TBL_FPL_FRULES      " = ?, " \
    AFTN_PARSER_TBL_FPL_TFLIGHT     " = ?, " \
    AFTN_PARSER_TBL_FPL_NUM         " = ?, " \
    AFTN_PARSER_TBL_FPL_TAIRCRAFT   " = ?, " \
    AFTN_PARSER_TBL_FPL_TURBULENCE_CATEGORY         " = ?, " \
    AFTN_PARSER_TBL_FPL_EQUIPMENT   " = ?, " \
    AFTN_PARSER_TBL_FPL_ADEP        " = ?, " \
    AFTN_PARSER_TBL_FPL_TIME        " = ?, " \
    AFTN_PARSER_TBL_FPL_SPEED       " = ?, " \
    AFTN_PARSER_TBL_FPL_FLEVEL      " = ?, " \
    AFTN_PARSER_TBL_FPL_ROUTE       " = ?, " \
    AFTN_PARSER_TBL_FPL_ADES        " = ?, " \
    AFTN_PARSER_TBL_FPL_EET         " = ?, " \
    AFTN_PARSER_TBL_FPL_ALT1        " = ?, " \
    AFTN_PARSER_TBL_FPL_ALT2        " = ?, " \
    AFTN_PARSER_TBL_FPL_OTHER       " = ?, " \
    AFTN_PARSER_TBL_FPL_MESSAGE_ID  " = ?, " \
    AFTN_PARSER_TBL_FPL_ENTRY_FIR_EET " = ? "

//! DATETIME format
#define SQL_TIMESTAMP_TO_CHAR(x)    "to_char("x", \'DD.MM.YYYY HH24:MI:SS\')"
#define DATE_FORMAT                 "dd.MM.yyyy"
#define TIME_FORMAT                 "hh:mm:ss"
#define DATETIME_FORMAT             DATE_FORMAT" "TIME_FORMAT

class AftnMessageParserPrivate;

typedef QPair<int, QString> AFTN_PARSER_ErrorInfo;
typedef QList<AFTN_PARSER_ErrorInfo> AftnMessageErrors;

enum Standard {
    FPL_2007 = 0
    , FPL_2012 = 1
};

extern Standard standard;

class AftnMessageParser : public QObject
{
    Q_OBJECT

protected:
    Q_DECLARE_PRIVATE(AftnMessageParser)

    const QScopedPointer<AftnMessageParserPrivate> d_ptr;

public:
    AftnMessageParser(const QSqlDatabase &database = QSqlDatabase(),
                         QObject *parent = 0);
    virtual ~AftnMessageParser();

    typedef QList<AFTN_PARSER_ErrorInfo> AftnMessageErrors;

    /**
     * @brief Try to parse AFTN message
     *
     * @fn processMessage
     * @param message source message
     * @param errorCodes list of errors
     * @return QVariantMap map, with results
     */
    QVariantMap processMessage(const QString& message,
                               AftnMessageErrors& errorCodes);
    /**
     * @brief Error codes, returning when parsing
     *
     * @enum ProcessResult    */

    enum ProcessResult
    {
        UnknownError = -1, Success = 0,
        //InvalidMessage = 0x00000001,
        //InvalidMessageFormat = 0x00000002,
        InvalidMessage = 1,
        InvalidMessageFormat = 2,

        InvalidField3Format,
        InvalidField5Format,
        InvalidField7Format,
        InvalidField8Format,
        InvalidField9Format,
        InvalidField10Format,
        InvalidField13Format,
        InvalidField14Format,
        InvalidField15Format,
        InvalidField16Format,
        InvalidField17Format,
        InvalidField18Format,
        InvalidField19Format,
        InvalidField20Format,
        InvalidField21Format,
        InvalidField22Format,

        TooManyFields,

        InvalidFlightRules,
        InvalidFlightType,
        InvalidTurbulenceCategory,
        InvalidRadioDevice,
        InvalidEquipment,
        InvalidBorderCrossTime,
        InvalidBorderCrossTerm,
        InvalidRoute,
        InvalidRouteItem,
        InvalidRouteItemSequence,
        NeededRouteNotExists,
        InvalidRoutePointPair,
        InvalidElapsedTime,
        InvalidArrivalTime,
        InvalidDateOfFLight,
        InvalidFirEet,
        InvalidDepartureTime,

        NotamTimeExpired,
        NotamFieldQRequired,
        NotamFieldARequired,
        NotamFieldBRequired,
        NotamFieldCRequired,
        NotamFieldDRequired,
        NotamFieldERequired,

        RplInvalidField4Format,
        RplInvalidValidTime,
        RplValidTimeTooOld,

        GeneralSqlError,
        SqlErrorInsertFpl,
        SqlErrorInsertFplSFplFound,
        SqlErrorInsertChg,
        SqlErrorChgFplNotFound,
        SqlErrorInsertCnl,
        SqlErrorCnlFplNotFound,
        SqlErrorInsertDla,
        SqlErrorDlaFplNotFound,
        SqlErrorInsertDep,
        SqlErrorDepFplNotFound,
        SqlErrorInsertArr,
        SqlErrorArrFplNotFound,
        SqlErrorInsertNotam,
        SqlErrorNotamForReplacingNotExists
    };

    QString errorString(const QPair<int, QString>& error) const;
    QStringList errorStrings(const AftnMessageErrors& errors) const;

    static void processType3(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType4Rpl(const QString& field, QVariantMap& map,
                         AftnMessageErrors& errorCodes);
    static void processType5(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType7(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType8(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType9(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType9R(const QString& field, QVariantMap& map,
                      AftnMessageErrors& errorCodes);
    static void processType10(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType10v12(const QString& field, QVariantMap& map,
                          AftnMessageErrors& errorCodes);
    static void processType13(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType14(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType15(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType16(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType16Arr(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType17(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType17Fla(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType18(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType19(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType20(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType21(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processType22(const QString& field, QVariantMap& map,
                       AftnMessageErrors& errorCodes);
    static void processTooManyFields(const QString& field, QVariantMap& map,
                              AftnMessageErrors& errorCodes);

    static void validateType3(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType4Rpl(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType5(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType7(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType8(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType9(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType9R(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType10(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType10v12(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType13(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType14(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType15(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType16(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType16Arr(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType17(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType18(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType18v12(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType19(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType20(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType21(QVariantMap& map, AftnMessageErrors& errorCodes);
    static void validateType22(QVariantMap& map, AftnMessageErrors& errorCodes);

    static void validateTooManyFields(QVariantMap& map, AftnMessageErrors& errorCodes);

    QStringList slice(QStringList src, int start, int end = -1);
    static QString cutFIRRoute(const QString& route, AftnMessageErrors& errorCodes);

private:
    static bool isSIDRoute(const QString& identifier);
    static bool isSTARRoute(const QString& identifier);

    static bool isPoint(const QString& identifier);
    static bool isRoute(const QString& identifier);

    qint64 firId(const QString& identifier);

    QList<int> errorLines;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // AFTNMESSAGEPARSER_H
