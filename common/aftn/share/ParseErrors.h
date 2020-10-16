#ifndef PARSEERRORS_H
#define PARSEERRORS_H

#include "enumstreaming.h"

#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtDebug>
/*!
 * \struct ErrorTr
 * \brief Класс переводов
 *
 * Фиктивный класс, для вынесения переводов в отдельную запись.
 * Вынесен из пространства имен \a ParserErrors, потому что lupdate некорректно обрабатывает конструкцию:
 * #ifdef AFTN_NAMESPACE
 * namespace Aftn
 * {
 * #endif
 */

class ErrorTr : public QObject
{
    Q_OBJECT
};

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct TelegramError
 * \brief Структура ошибки разбора
 */

struct TelegramError
{
    /*!
     * \brief Тип ошибки
     *
     * Возможные типы:
     * - ошибка при разборе АФТН телеграммы
     * - ошибка при разборе стандартного ОВД сообщения
     * - ошибка при разборе нестандартного ОВД сообщения
     * - ошибка при разборе метео сообщения
     * - ошибка при разборе нестандартного ограничения
     */
    enum Type {
        AFTN = 0
        , STANDARD = 1
        , EXTENDED = 2
        , METEO = 3
        , RESTRICTION = 4
    } type;
    /*!
     * \brief Код ошибки
     */
    quint16 code;
    QString elementError;
    TelegramError()
    {
        type = AFTN;
        code = 0;
    }
    TelegramError(Type t, int c)
    {
        type = t;
        code = c;
    }
    TelegramError(Type t, int c, QString e)
    {
        type = t;
        code = c;
        elementError = e;
    }

    inline bool operator== (const TelegramError &err) const
    {
        //return ((type == err.type) && (code == err.code));
        return ((type == err.type) && (code == err.code) && (elementError == err.elementError));
    }
};
QT_DECLARE_ENUM_STREAMING(TelegramError::Type)

inline uint qHash(const TelegramError &e)
{
    return e.type * 65536 + e.code;
}

inline QDataStream & operator<< (QDataStream &stream, const TelegramError &msg)
{
    stream << msg.type;
    stream << msg.code;
    stream << msg.elementError;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, TelegramError &msg)
{
    stream >> msg.type;
    stream >> msg.code;
    stream >> msg.elementError;
    return stream;
}

namespace ParserErrors
{

enum StandardError {
    // from aftnmessageprocessor
    UnknownError = -1,
    Success = 0,
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
    NotamFieldQError,
    NotamFieldAError,
    NotamFieldBError,
    NotamFieldCError,
    NotamFieldDError,
    NotamFieldEError,
    NotamNumberError,

    RplInvalidField4Format,
    RplInvalidValidTime,
    RplValidTimeTooOld,

    InvalidSts,
    InvalidReg,
    InvalidPer,
    InvalidOrgn,

    // manually added
    InvalidACID = 1000,
    InvalidAircraftType,
    InvalidADEP,
    InvalidADES,

    NoSuchFpl,
    NotOurAirport,
    StatusError,
    PlnNotFromZces,
    PpnNotFromZces,
    AckNotFromZces,
    RejNotFromZces,
    ChangesOnTplSent,
    ChangesOnFplSent,
    FplDepartured,
    FplArrived,

    NoSuchNotam,
    NoSuchAtf,

    SemanticControl,

    NoSuchRpl,

    SuchNotamNumberExists,

    SuchFplExists,
    TwoFplExists,

    SvcManual
};

enum AftnError {
    AftnNoError = 0,
    AftnNoZCZC,
    AftnNoNNNN,
    AftnErrorTransmissionIdentification,
    AftnErrorPriorityIndicator,
    AftnErrorReceiverAddress,
    AftnErrorFillingTime,
    AftnErrorSenderAddress,
    AftnErrorWrongPart,
    AftnErrorLastPart
};

enum NonStandardMessageError {
    UnhandledError
    , ExtendedInvalidMessage
    , UnknownMessage

    , ErrorMessageType

    , ErrorGplStartTime
    , ErrorGplFinishTime
    , InvalidGplTime

    , ErrorRtcResDateTime
    , InvalidRtcResDateTime
};

enum RestrictionError {
    UnhandledErrorRestriction = 0
    , UnknownMessageRestriction
    , ErrorTypeMessage
    , ErrorMessageIdRestriction
    , ErrorMessageTypeRestriction
    , ErrorMessageTimeRestriction
    , ErrorMessageFormRestriction
    , ErrorMessagePointsRestriction
    , ErrorMessageNameRestriction
};

enum MeteoError {
    NotMeteo
    , UnknownMeteoMessage
    , MissingEnding
    , BadEnding
    , ExtraField
    , UnknownFields
    , BadWmoDataType

    , InvalidTime
    , CancelNoNumber
    , CancelBadTime

    , MetarTypeField
    , MetarZoneField
    , MetarTimeField
    , MetarWindField
    , MetarVisibilityField
    , MetarCloudinessField
    , MetarTemperatureField
    , MetarPressureField
    , MetarTrendField
    , MetarWindShearField
    , MetarTrendTimeField

    , MetarBadTime
    , MetarBadWind
    , MetarBadRunwayCoveringRange
    , MetarBadRunwayCoveringThick
    , MetarBadRunwayCohesion
    , MetarBadTrend
    , MetarBadTrendTime

    , TafTypeField
    , TafZoneField
    , TafTimeField
    , TafValidTimeField
    , TafWindField
    , TafCnlField
    , TafVisibilityField
    , TafCloudinessField
    , TafTrendTimeField

    , TafBadValidTime
    , TafBadWind
    , TafBadTrend
    , TafBadTrendTime

    , SigmetHeaderField
    , SigmetTargetField
    , SigmetEventField
    , SigmetObservationField
    , SigmetLocationField
    , SigmetFlightLevelField
    , SigmetMovField
    , SigmetIntensityChangeField

    , SigmetBadValidTime

    , SigmetVolcanoField
    , SigmetAshCloudLocationField
    , SigmetVaForecastField

    , AirmetHeaderField
    , AirmetTargetField
    , AirmetEventField
    , AirmetObservationField
    , AirmetLocationField
    , AirmetFlightLevelField
    , AirmetMovField
    , AirmetIntensityChangeField

    , AirmetBadValidTime

    , GametHeaderField
    , GametTargetField
    , GametFlightLevelField
    , GametBody

    , GametBadValidTime

    , CancelNoSuchMeteo

    , SigmetTropicalCycloneNotSupported = 9999
};

inline QString toString(TelegramError e);
inline QString toString(ParserErrors::StandardError e, QString _elementError);
inline QString toString(ParserErrors::AftnError e);
inline QString toString(ParserErrors::NonStandardMessageError e);
inline QString toString(ParserErrors::RestrictionError e);
inline QString toString(ParserErrors::MeteoError e);


inline QString toString(TelegramError e)
{
    switch (e.type)
    {
    case TelegramError::AFTN:
        return toString(ParserErrors::AftnError(e.code));
    case TelegramError::STANDARD:
        return toString(ParserErrors::StandardError(e.code),e.elementError);
    case TelegramError::EXTENDED:
        return toString(ParserErrors::NonStandardMessageError(e.code));
    case TelegramError::METEO:
        return toString(ParserErrors::MeteoError(e.code));
    case TelegramError::RESTRICTION:
        return toString(ParserErrors::RestrictionError(e.code));
    }
    return QString();
}

inline QString toString(ParserErrors::StandardError e, QString _elementError)
{
    switch (e)
    {
    case ParserErrors::Success:
        return QString();
    case ParserErrors::UnknownError:
        return ErrorTr::tr("Unknown error");
    case ParserErrors::InvalidMessage:
        return ErrorTr::tr("Invalid message");
    case ParserErrors::InvalidMessageFormat:
        return ErrorTr::tr("Invalid message format");
    case ParserErrors::InvalidField3Format:
        return ErrorTr::tr("Invalid field 3 format");
    case ParserErrors::InvalidField5Format:
        return ErrorTr::tr("Invalid field 5 format");
    case ParserErrors::InvalidField7Format:
        return ErrorTr::tr("Invalid field 7 format");
    case ParserErrors::InvalidField8Format:
        return ErrorTr::tr("Invalid field 8 format");
    case ParserErrors::InvalidField9Format:
        return ErrorTr::tr("Invalid field 9 format");
    case ParserErrors::InvalidField10Format:
        return ErrorTr::tr("Invalid field 10 format");
    case ParserErrors::InvalidField13Format:
        return ErrorTr::tr("Invalid field 13 format");
    case ParserErrors::InvalidField14Format:
        return ErrorTr::tr("Invalid field 14 format");
    case ParserErrors::InvalidField15Format:
        return ErrorTr::tr("Invalid field 15 format");
    case ParserErrors::InvalidField16Format:
        return ErrorTr::tr("Invalid field 16 format");
    case ParserErrors::InvalidField17Format:
        return ErrorTr::tr("Invalid field 17 format");
    case ParserErrors::InvalidField18Format:
        return ErrorTr::tr("Invalid field 18 format");
    case ParserErrors::InvalidField19Format:
        return ErrorTr::tr("Invalid field 19 format");
    case ParserErrors::InvalidField20Format:
        return ErrorTr::tr("Invalid field 20 format");
    case ParserErrors::InvalidField21Format:
        return ErrorTr::tr("Invalid field 21 format");
    case ParserErrors::InvalidField22Format:
        return ErrorTr::tr("Invalid field 22 format");
    case ParserErrors::TooManyFields:
        return ErrorTr::tr("Too many fields");
    case ParserErrors::InvalidFlightRules:
        return ErrorTr::tr("Invalid flight rules");
    case ParserErrors::InvalidFlightType:
        return ErrorTr::tr("Invalid flight type");
    case ParserErrors::InvalidTurbulenceCategory:
        return ErrorTr::tr("Invalid turbulence category");
    case ParserErrors::InvalidRadioDevice:
        return ErrorTr::tr("Invalid radio device");
    case ParserErrors::InvalidEquipment:
        return ErrorTr::tr("Invalid equipment");
    case ParserErrors::InvalidBorderCrossTime:
        return ErrorTr::tr("Invalid border cross time");
    case ParserErrors::InvalidBorderCrossTerm:
        return ErrorTr::tr("Invalid border cross term");
    case ParserErrors::InvalidRoute:
        return (ErrorTr::tr("Invalid route") + " : " + _elementError);
    case ParserErrors::InvalidRouteItem:
        return ErrorTr::tr("Invalid route item");
    case ParserErrors::InvalidRouteItemSequence:
        return ErrorTr::tr("Invalid route item sequence");
    case ParserErrors::NeededRouteNotExists:
        return (ErrorTr::tr("Needed route does not exist") + " : " +  _elementError);
    case ParserErrors::InvalidRoutePointPair:
        return ErrorTr::tr("Invalid route point pair");
    case ParserErrors::InvalidElapsedTime:
        return ErrorTr::tr("Invalid elapsed time");
    case ParserErrors::InvalidArrivalTime:
        return ErrorTr::tr("Invalid arrival time");
    case ParserErrors::InvalidDateOfFLight:
        return ErrorTr::tr("Invalid date of flight");
    case ParserErrors::InvalidFirEet:
        return ErrorTr::tr("Invalid Fir Eet");
    case ParserErrors::InvalidDepartureTime:
        return ErrorTr::tr("Invalid Departure Time");
    case ParserErrors::NotamTimeExpired:
        return ErrorTr::tr("Notam time expired");
    case ParserErrors::NotamFieldQRequired:
        return ErrorTr::tr("Notam field Q required");
    case ParserErrors::NotamFieldARequired:
        return ErrorTr::tr("Notam field A required");
    case ParserErrors::NotamFieldBRequired:
        return ErrorTr::tr("Notam field B required");
    case ParserErrors::NotamFieldCRequired:
        return ErrorTr::tr("Notam field C required");
    case ParserErrors::NotamFieldDRequired:
        return ErrorTr::tr("Notam field D required");
    case ParserErrors::NotamFieldERequired:
        return ErrorTr::tr("Notam field E required");
    case ParserErrors::NotamFieldQError:
        return ErrorTr::tr("Notam field Q error");
    case ParserErrors::NotamFieldAError:
        return ErrorTr::tr("Notam field A error");
    case ParserErrors::NotamFieldBError:
        return ErrorTr::tr("Notam field B error");
    case ParserErrors::NotamFieldCError:
        return ErrorTr::tr("Notam field C error");
    case ParserErrors::NotamFieldDError:
        return ErrorTr::tr("Notam field D error");
    case ParserErrors::NotamFieldEError:
        return ErrorTr::tr("Notam field E error");
    case ParserErrors::NotamNumberError:
        return ErrorTr::tr("Notam number error");
    case ParserErrors::RplInvalidField4Format:
        return ErrorTr::tr("RPL Invalid field 4 format");
    case ParserErrors::RplInvalidValidTime:
        return ErrorTr::tr("RPL bad valid time");
    case ParserErrors::RplValidTimeTooOld:
        return ErrorTr::tr("RPL is too old");
    case ParserErrors::InvalidSts:
        return ErrorTr::tr("Invalid STS");
    case ParserErrors::InvalidReg:
        return ErrorTr::tr("Invalid REG");
    case ParserErrors::InvalidPer:
        return ErrorTr::tr("Invalif PER");
    case ParserErrors::InvalidOrgn:
        return ErrorTr::tr("Invalid ORGN");
    case InvalidACID:
        return ErrorTr::tr("Invalid ACID");
    case InvalidAircraftType:
        return ErrorTr::tr("Invalid aircraft type");
    case InvalidADEP:
        return ErrorTr::tr("Invalid ADEP");
    case InvalidADES:
        return ErrorTr::tr("Invalid ADES");
    case NoSuchFpl:
        return ErrorTr::tr("No fpl, associated to this message");
    case NotOurAirport:
        return ErrorTr::tr("Fpl with ADEP, ADES and alternatives - not our airport");
    case StatusError:
        return ErrorTr::tr("Message breaks fpl status");
    case PlnNotFromZces:
        return ErrorTr::tr("PLN not from ZCES");
    case PpnNotFromZces:
        return ErrorTr::tr("PPN not from ZCES");
    case AckNotFromZces:
        return ErrorTr::tr("ACK not from ZCES");
    case RejNotFromZces:
        return ErrorTr::tr("REJ not from ZCES");
    case ChangesOnTplSent:
        return ErrorTr::tr("Changes came, when plan is checked");
    case ChangesOnFplSent:
        return ErrorTr::tr("Changes came, when plan is on approval");
    case FplDepartured:
        return ErrorTr::tr("Airplane has already departured");
    case FplArrived:
        return ErrorTr::tr("Airplane has already arrived");
    case NoSuchNotam:
        return ErrorTr::tr("No such notam");
    case NoSuchAtf:
        return ErrorTr::tr("No such ATF message");
    case SemanticControl:
        return ErrorTr::tr("Semantic control error");
    case NoSuchRpl:
        return ErrorTr::tr("No rpl, associated to this message");
    case SuchNotamNumberExists:
        return ErrorTr::tr("Such number of notam exists");
    case SuchFplExists:
        return ErrorTr::tr("Such FPL exists");
    case TwoFplExists:
        return ErrorTr::tr("More than one FPL exists");
    case SvcManual:
        return ErrorTr::tr("Could not process service message automatically");

    }
    return ErrorTr::tr("Unknown error");
}

inline QString toString(ParserErrors::AftnError e)
{
    switch (e)
    {
    case AftnNoError:
        return ErrorTr::tr("No error");
    case AftnNoZCZC:
        return ErrorTr::tr("No ZCZC");
    case AftnNoNNNN:
        return ErrorTr::tr("No NNNN");
    case AftnErrorTransmissionIdentification:
        return ErrorTr::tr("Error in transmission identification field");
    case AftnErrorPriorityIndicator:
        return ErrorTr::tr("Error in priority indicator field");
    case AftnErrorReceiverAddress:
        return ErrorTr::tr("Error in receiver address field");
    case AftnErrorFillingTime:
        return ErrorTr::tr("Error in filling time field");
    case AftnErrorSenderAddress:
        return ErrorTr::tr("Error in sender address field");
    case AftnErrorWrongPart:
        return ErrorTr::tr("Other part number expected");
    case AftnErrorLastPart:
        return ErrorTr::tr("Error in last part numeration");
    }
    return QString();
}

inline QString toString(ParserErrors::NonStandardMessageError e)
{
    switch (e)
    {
    case UnhandledError:
        return ErrorTr::tr("Unhandled Error");
    case ExtendedInvalidMessage:
        return ErrorTr::tr("Invalid Message");
    case UnknownMessage:
        return ErrorTr::tr("Unknown Message");
    case ErrorMessageType:
        return ErrorTr::tr("Error in message type");
    case ErrorGplStartTime:
        return ErrorTr::tr("Error in GPL start time");
    case ErrorGplFinishTime:
        return ErrorTr::tr("Error in GPL finish time");
    case InvalidGplTime:
        return ErrorTr::tr("Invalid GPL time");
    case ErrorRtcResDateTime:
        return ErrorTr::tr("Error in RTC/RES date or time");
    case InvalidRtcResDateTime:
        return ErrorTr::tr("Invalid RTC/RES date or time");
    }
    return QString();
}

inline QString toString(ParserErrors::RestrictionError e)
{
    switch (e)
    {
    case UnhandledErrorRestriction:
        return ErrorTr::tr("Unhandled Error");
    case UnknownMessageRestriction:
        return ErrorTr::tr("Unknown Message");
    case ErrorTypeMessage:
        return ErrorTr::tr("Error in message type");
    case ErrorMessageIdRestriction:
        return ErrorTr::tr("Error in identifier");
    case ErrorMessageTypeRestriction:
        return ErrorTr::tr("Error in type of restriction");
    case ErrorMessageTimeRestriction:
        return ErrorTr::tr("Error in time of active");
    case ErrorMessageFormRestriction:
        return ErrorTr::tr("Error in form of restriction");
    case ErrorMessagePointsRestriction:
        return ErrorTr::tr("Error in points of restriction");
    case ErrorMessageNameRestriction:
        return ErrorTr::tr("Error in name of restriction");
    }
    return QString();
}

inline QString toString(ParserErrors::MeteoError e)
{
    switch (e)
    {
    case NotMeteo:
        return ErrorTr::tr("Not a meteo message");
    case UnknownMeteoMessage:
        return ErrorTr::tr("Unknown meteo message type");
    case MissingEnding:
        return ErrorTr::tr("Message ending is missing");
    case BadEnding:
        return ErrorTr::tr("There are simbols after message ending");
    case ExtraField:
        return ErrorTr::tr("Message has extra unwanted fields");
    case UnknownFields:
        return ErrorTr::tr("Parser reached unknown fields");
    case BadWmoDataType:
        return ErrorTr::tr("WMO header data type does not match message type");
    case InvalidTime:
        return ErrorTr::tr("Invalid time");
    case CancelNoNumber:
        return ErrorTr::tr("Cancelling message: no such number");
    case CancelBadTime:
        return ErrorTr::tr("Cancelling message: valid time for this number do not match");
    case MetarTypeField:
        return ErrorTr::tr("Error in metar type field");
    case MetarZoneField:
        return ErrorTr::tr("Error in metar zone field");
    case MetarTimeField:
        return ErrorTr::tr("Error in metar time field");
    case MetarWindField:
        return ErrorTr::tr("Error in metar wind field");
    case MetarVisibilityField:
        return ErrorTr::tr("Error in metar visibility or CAVOK field");
    case MetarCloudinessField:
        return ErrorTr::tr("Error in metar cloudiness field");
    case MetarTemperatureField:
        return ErrorTr::tr("Error in metar temperature field");
    case MetarPressureField:
        return ErrorTr::tr("Error in metar pressure field");
    case MetarTrendField:
        return ErrorTr::tr("Error in metar trend field");
    case MetarWindShearField:
        return ErrorTr::tr("Error in metar wind shear field");
    case MetarTrendTimeField:
        return ErrorTr::tr("Error in metar trend time field");
    case MetarBadTime:
        return ErrorTr::tr("Metar: time contains errors");
    case MetarBadWind:
        return ErrorTr::tr("Metar: wind field contains errors");
    case MetarBadRunwayCoveringRange:
        return ErrorTr::tr("Metar: one of runway's covering range is incorrect");
    case MetarBadRunwayCoveringThick:
        return ErrorTr::tr("Metar: one of runway's covering thick is incorrect");
    case MetarBadRunwayCohesion:
        return ErrorTr::tr("Metar: one of runway's cohesion is incorrect");
    case MetarBadTrend:
        return ErrorTr::tr("Metar: trend contains errors");
    case MetarBadTrendTime:
        return ErrorTr::tr("Metar: trend time contains errors");
    case TafTypeField:
        return ErrorTr::tr("Error in taf type field");
    case TafZoneField:
        return ErrorTr::tr("Error in taf zone field");
    case TafTimeField:
        return ErrorTr::tr("Error in taf time field");
    case TafValidTimeField:
        return ErrorTr::tr("Error in taf valid time field");
    case TafWindField:
        return ErrorTr::tr("Error in taf wind field");
    case TafCnlField:
        return ErrorTr::tr("Error in taf cancel field");
    case TafVisibilityField:
        return ErrorTr::tr("Error in taf visibility or CAVOK field");
    case TafCloudinessField:
        return ErrorTr::tr("Error in taf cloudiness field");
    case TafTrendTimeField:
        return ErrorTr::tr("Error in taf trend time field");
    case TafBadValidTime:
        return ErrorTr::tr("Taf: bad valid time");
    case TafBadWind:
        return ErrorTr::tr("Taf: wind field contains errors");
    case TafBadTrend:
        return ErrorTr::tr("Taf: trend contains errors");
    case TafBadTrendTime:
        return ErrorTr::tr("Taf: bad trend time");
    case SigmetHeaderField:
        return ErrorTr::tr("Error in sigmet header field");
    case SigmetTargetField:
        return ErrorTr::tr("Error in sigmet target field");
    case SigmetEventField:
        return ErrorTr::tr("Error in sigmet event or cancel field");
    case SigmetObservationField:
        return ErrorTr::tr("Error in sigmet observation field");
    case SigmetLocationField:
        return ErrorTr::tr("Error in sigmet location field");
    case SigmetFlightLevelField:
        return ErrorTr::tr("Error in sigmet flight level field");
    case SigmetMovField:
        return ErrorTr::tr("Error in sigmet mov field");
    case SigmetIntensityChangeField:
        return ErrorTr::tr("Error in sigmet intensity change field");
    case SigmetBadValidTime:
        return ErrorTr::tr("Sigmet: bad valid time");
    case SigmetVolcanoField:
        return ErrorTr::tr("Error in sigmet volcano field");
    case SigmetAshCloudLocationField:
        return ErrorTr::tr("Error in sigmet ash cloud location field");
    case SigmetVaForecastField:
        return ErrorTr::tr("Error in sigmet volcanic ash forecast field");
    case AirmetHeaderField:
        return ErrorTr::tr("Error in airmet header field");
    case AirmetTargetField:
        return ErrorTr::tr("Error in airmet target field");
    case AirmetEventField:
        return ErrorTr::tr("Error in airmet event or cancel field");
    case AirmetObservationField:
        return ErrorTr::tr("Error in airmet observation field");
    case AirmetLocationField:
        return ErrorTr::tr("Error in airmet location field");
    case AirmetFlightLevelField:
        return ErrorTr::tr("Error in airmet flight level field");
    case AirmetMovField:
        return ErrorTr::tr("Error in airmet mov field");
    case AirmetIntensityChangeField:
        return ErrorTr::tr("Error in airmet intensity change field");
    case AirmetBadValidTime:
        return ErrorTr::tr("Airmet: bad valid time");
    case GametHeaderField:
        return ErrorTr::tr("Error in gamet header field");
    case GametTargetField:
        return ErrorTr::tr("Error in gamet target field");
    case GametFlightLevelField:
        return ErrorTr::tr("Error in gamet flight level field");
    case GametBody:
        return ErrorTr::tr("Error in gamet body");
    case GametBadValidTime:
        return ErrorTr::tr("Gamet: bad valid time");
    case CancelNoSuchMeteo:
        return ErrorTr::tr("No meteo message, associated with this cancelling message");
    case SigmetTropicalCycloneNotSupported:
        return ErrorTr::tr("Tropical cyclone SIGMET is not supported");
    }
    return QString();
}

} // namespace ParserErrors

class TelegramErrorList : public QList < TelegramError >
{
public:
    TelegramErrorList()
    {}

    inline QString toString(const QString &prefix = "- ") const
    {
        QStringList errorList;
        foreach (const TelegramError &err, *this)
            errorList.append(prefix + ParserErrors::toString(err));
        return errorList.join("\n");
    }
};

class TelegramErrorSet : public QSet < TelegramError >
{
public:
    TelegramErrorSet()
    {}
    TelegramErrorSet(const QSet < TelegramError > &err) :
        QSet < TelegramError >(err)
    {}

    inline QString toString(const QString &prefix = "- ") const
    {
        QStringList errorList;
        foreach (const TelegramError &err, *this)
            errorList.append(prefix + ParserErrors::toString(err));
        return errorList.join("\n");
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PARSEERRORS_H
