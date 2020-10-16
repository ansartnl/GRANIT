#ifndef METEOPARSER_H
#define METEOPARSER_H

#include "ParseErrors.h"
#include "Share.h"

#include <QtCore/QDateTime>
#include <QtCore/QSet>
#include <QtCore/QVariantMap>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

// common meteo info

dusc(METEO_WMO_DATA_TYPE)
dusc(METEO_WMO_REGION)
dusc(METEO_WMO_BULLETIN_NUMBER)
dusc(METEO_WMO_DISSEMINATOR)
dusc(METEO_WMO_TRANSMITTION_DATE_TIME)

dusc(METEO_WMO_ERROR_TYPE)
dusc(METEO_WMO_ERROR_NUMBER)

dusc(METEO_MESSAGE_TEXT)
dusc(METEO_MESSAGE_TYPE)
dusc(METEO_ARRIVAL_TIME)
dusc(METEO_ZONE)

// METAR/SPECI

dusc(METEO_COR)
dusc(METEO_TIME)
dusc(METEO_NIL)
dusc(METEO_AUTO)

dusc(METEO_WIND_DIRECTION)
dusc(METEO_WIND_AVARAGE_SPEED)
dusc(METEO_WIND_GUSTS)
dusc(METEO_WIND_IS_ABOVE)
dusc(METEO_WIND_SPEED_TYPE)
dusc(METEO_WIND_VARIES_FROM)
dusc(METEO_WIND_VARIES_TO)

dusc(METEO_VISIBILITY)
dusc(METEO_VISIBILITY_DIRECTION)
dusc(METEO_VISIBILITY_MAXIMUM)
dusc(METEO_VISIBILITY_MAXIMUM_DIRECTION)

dusc(METEO_RUNWAY_VISIBILIY)            // QVariantMap / id + place
dusc(METEO_RUNWAY_ID)
dusc(METEO_RUNWAY_PLACE)
dusc(METEO_RUNWAY_VISIBILIY_TYPE)
dusc(METEO_RUNWAY_VISIBILIY_VALUE)
dusc(METEO_RUNWAY_VISIBILIY_MAX_TYPE)
dusc(METEO_RUNWAY_VISIBILIY_MAX_VALUE)
dusc(METEO_RUNWAY_VISIBILIY_TENDENCY)

dusc(METEO_WEATHER_EVENTS)              // QVariantList
dusc(METEO_WEATHER_EVENT)
dusc(METEO_WEATHER_EVENT_IN_VICINITY)
dusc(METEO_WEATHER_EVENT_STRENGTH)

dusc(METEO_CLOUDINESS)                  // QVariantMap / CLOUDINESS_TYPE, CLOUDS, VERTICAL_VISIBILITY
dusc(METEO_CLOUDINESS_TYPE)             // "SKC" or "NSC" or "NCD"
dusc(METEO_CLOUDS)                      // QVariantList
dusc(METEO_CLOUDS_COUNT)
dusc(METEO_CLOUDS_HEIGHT)
dusc(METEO_CLOUDS_TYPE)
dusc(METEO_VERTICAL_VISIBILITY)

dusc(METEO_CAVOK)

dusc(METEO_TEMPERATURE)
dusc(METEO_DEW_POINT)

dusc(METEO_PRESSURE_QNH_HP)
dusc(METEO_PRESSURE_QNH_MM)

dusc(METEO_RECENT_WEATHER)              // QVariantList

dusc(METEO_WIND_SHEAR)                  // QVariantMap / id + place
dusc(METEO_WIND_SHEAR_ALL_RUNWAYS)

dusc(METEO_SEA_TEMPERATURE)
dusc(METEO_SEA_STATE)

dusc(METEO_RUNWAY_STATE)                // QVariantMap / id
dusc(METEO_RUNWAY_SNOCLO)
dusc(METEO_RUNWAY_CLRD)
dusc(METEO_RUNWAY_COVERING_CONDITIONS)
dusc(METEO_RUNWAY_COVERING_RANGE)
dusc(METEO_RUNWAY_COVERING_THICK)
dusc(METEO_RUNWAY_COHESION)

dusc(METEO_TREND_TYPE)
dusc(METEO_TREND_AT)
dusc(METEO_TREND_FROM)
dusc(METEO_TREND_TILL)
dusc(METEO_TREND_WIND_DIRECTION)
dusc(METEO_TREND_WIND_AVARAGE_SPEED)
dusc(METEO_TREND_WIND_GUSTS)
dusc(METEO_TREND_WIND_IS_ABOVE)
dusc(METEO_TREND_WIND_TYPE)
dusc(METEO_TREND_VISIBILITY)
dusc(METEO_TREND_WEATHER_EVENTS)        // QVariantList
dusc(METEO_TREND_CLOUDINESS)            // QVariantMap / CLOUDINESS_TYPE, CLOUDS, VERTICAL_VISIBILITY

dusc(METEO_RMK)

// TAF

dusc(METEO_VALID_FROM)
dusc(METEO_VALID_TILL)

dusc(METEO_TREND)
dusc(METEO_TREND_PROBABILITY)

dusc(METEO_MAX_TEMPERATURE)
dusc(METEO_MAX_TEMPERATURE_TIME)
dusc(METEO_MIN_TEMPERATURE)
dusc(METEO_MIN_TEMPERATURE_TIME)

dusc(METEO_CANCEL)

// SIGMET

dusc(METEO_CONTROL_ATS)
dusc(METEO_NUMBER)
dusc(METEO_SOURCE)

dusc(METEO_TARGET)
dusc(METEO_TARGET_SECTOR)
dusc(METEO_TARGET_NAME)
dusc(METEO_TARGET_FIR)

dusc(METEO_EVENT)
dusc(METEO_OBSERVATION_TYPE)
dusc(METEO_OBSERVATION_TIME)

// 0 - points, 1 - line, 2 - within, 3 - over fir, 4 - over whole fir
dusc(METEO_LOCATION_TYPE)
dusc(METEO_COMMON_DIRECTION)
dusc(METEO_LATTITUDE_DIRECTION)
dusc(METEO_LONGITUDE_DIRECTION)
dusc(METEO_LATTITUDE)
dusc(METEO_LONGITUDE)
dusc(METEO_POINTS)                      // QVariantList
dusc(METEO_LOCATION_OVER_FIR)

dusc(METEO_FLIGHT_LEVEL_TYPE)
dusc(METEO_FLIGHT_LEVEL_FIRST)
dusc(METEO_FLIGHT_LEVEL_SECOND)
dusc(METEO_FLIGHT_LEVEL_UNIT)

dusc(METEO_MOVE_TYPE)
dusc(METEO_MOVE_DIRECTION)
dusc(METEO_MOVE_SPEED)
dusc(METEO_MOVE_UNIT)

dusc(METEO_INTENSITY_CHANGE)

dusc(METEO_CANCEL_NUMBER)
dusc(METEO_CANCEL_VALID_FROM)
dusc(METEO_CANCEL_VALID_TILL)

dusc(METEO_VA_ERUPTION)
dusc(METEO_MT_NAME)
dusc(METEO_MT_POSITION)
dusc(METEO_MT_LATTITUDE)
dusc(METEO_MT_LONGITUDE)

dusc(METEO_ASH_CLOUD_LOCATION)          // QVariantList
dusc(METEO_ASH_CLOUD_LENGTH1)
dusc(METEO_ASH_CLOUD_LENGTH1TYPE)
dusc(METEO_ASH_CLOUD_LENGTH2)
dusc(METEO_ASH_CLOUD_LENGTH2TYPE)

dusc(METEO_VA_FORECAST)                 // QVariantMap
dusc(METEO_VA_FORECAST_TIME)

// AIRMET

dusc(METEO_CLOUDS_LOWER_LIMIT)
dusc(METEO_CLOUDS_UPPER_LIMIT)
dusc(METEO_CLOUDS_UPPER_LIMIT_IS_ABOVE)
dusc(METEO_CLOUDS_LIMIT_TYPE)

// GAMET

dusc(METEO_FLIGHT_LEVEL)

dusc(METEO_GAMET_SECTION1)
dusc(METEO_GAMET_SECTION2)

class MeteoParser
{
public:
    MeteoParser();

    typedef QSet < ParserErrors::MeteoError > MeteoErrors;

    static QVariantMap parse(const QString &data, MeteoErrors &error);
    QVariantMap parse(const QString &data);

    MeteoErrors errors() const;

    enum SigmetLocationType {
        POINTS = 0
        , LINE = 1
        , WITHIN = 2
        , OVER_FIR = 3
        , OVER_WHOLE_FIR = 4
    };

protected:
    bool tryParseMetar(const QString &data, QVariantMap &map);
    bool tryParseTaf(const QString &data, QVariantMap &map);
    bool tryParseSigmet(const QString &data, QVariantMap &map);
    bool tryParseAirmet(const QString &data, QVariantMap &map);
    bool tryParseGamet(const QString &data, QVariantMap &map);

    enum MetarField {
        METAR_START = 0
        , METAR_TYPE
        , METAR_COR
        , METAR_ZONE
        , METAR_TIME
        , METAR_NIL
        , METAR_AUTO
        , METAR_WIND
        , METAR_WIND_VARIATIONS
        , METAR_VISIBILITY
        , METAR_MAX_VISIBILITY
        , METAR_RUNWAY_VISIBILITY
        , METAR_WEATHER_EVENT
        , METAR_CLOUDINESS_TYPE
        , METAR_CLOUDS
        , METAR_VERTICAL_VISIBILITY
        , METAR_CAVOK
        , METAR_TEMPERATURE
        , METAR_PRESSURE
        , METAR_RECENT_WEATHER
        , METAR_WS
        , METAR_ALL
        , METAR_RWY
        , METAR_WIND_SHEAR
        , METAR_SEA_STATE
        , METAR_RUNWAY_STATE
        , METAR_TREND
        , METAR_TREND_TIME
        , METAR_TREND_WIND
        , METAR_TREND_VISIBILITY
        , METAR_TREND_WEATHER_EVENT
        , METAR_TREND_CLOUDINESS_TYPE
        , METAR_TREND_CLOUDS
        , METAR_TREND_VERTICAL_VISIBILITY
        , METAR_RMK
    } lastMetarField;

    enum TafField {
        TAF_START = 0
        , TAF_TYPE
        , TAF_COR
        , TAF_ZONE
        , TAF_TIME
        , TAF_VALID_TIME
        , TAF_NIL
        , TAF_CNL
        , TAF_WIND
        , TAF_VISIBILITY
        , TAF_WEATHER_EVENT
        , TAF_CLOUDINESS_TYPE
        , TAF_CLOUDS
        , TAF_VERTICAL_VISIBILITY
        , TAF_CAVOK
        , TAF_TREND_PROBABILITY
        , TAF_TREND_TYPE
        , TAF_TREND_TIME
        , TAF_TREND_WIND
        , TAF_TREND_VISIBILITY
        , TAF_TREND_WEATHER_EVENT
        , TAF_TREND_CLOUDINESS_TYPE
        , TAF_TREND_CLOUDS
        , TAF_TREND_VERTICAL_VISIBILITY
        , TAF_TREND_CAVOK
        , TAF_MAX_TEMPERATURE
        , TAF_MIN_TEMPERATURE
    } lastTafField;

    enum SigmetField {
        SIGMET_START = 0
        , SIGMET_HEADER
        , SIGMET_TARGET
        , SIGMET_EVENT
        , SIGMET_OBSERVATION
        , SIGMET_LOCATION
        , SIGMET_FLIGHT_LEVEL
        , SIGMET_MOV
        , SIGMET_INTENSITY_CHANGE
        , SIGMET_CANCEL
    } lastSigmetField;

    enum SigmetVaField {
        SIGMET_VA_START = 0
        , SIGMET_VA_HEADER
        , SIGMET_VA_TARGET
        , SIGMET_VA_VOLCANO
        , SIGMET_VA_OBSERVATION
        , SIGMET_VA_LOCATION
        , SIGMET_VA_MOV
        , SIGMET_VA_FORECAST
        , SIGMET_VA_CANCEL
    } lastSigmetVaField;

    enum AirmetField {
        AIRMET_START = 0
        , AIRMET_HEADER
        , AIRMET_TARGET
        , AIRMET_EVENT
        , AIRMET_OBSERVATION
        , AIRMET_LOCATION
        , AIRMET_FLIGHT_LEVEL
        , AIRMET_MOV
        , AIRMET_INTENSITY_CHANGE
        , AIRMET_CANCEL
    } lastAirmetField;

    enum GametField {
        GAMET_START = 0
        , GAMET_HEADER
        , GAMET_TARGET
        , GAMET_FLIGHT_LEVEL
        , GAMET_BODY
    } lastGametField;

    bool parseMetar(const QString &data, QVariantMap &map);
    void validateMetar(const QVariantMap &map);

    bool parseTaf(const QString &data, QVariantMap &map);
    void validateTaf(const QVariantMap &map);

    int parseAnySigmet(const QString &data, QVariantMap &map);
    void validateAnySigmet(const QVariantMap &map);

    int parseSigmet(const QString &data, QVariantMap &map);
    void validateSigmet(const QVariantMap &map);

    int parseSigmetTc(const QString &data, QVariantMap &map);
    void validateSigmetTc(const QVariantMap &map);

    int parseSigmetVa(const QString &data, QVariantMap &map);
    void validateSigmetVa(const QVariantMap &map);

    int parseAirmet(const QString &data, QVariantMap &map);
    void validateAirmet(const QVariantMap &map);

    int parseGamet(const QString &data, QVariantMap &map);
    void validateGamet(const QVariantMap &map);

    bool parseMetarType(const QString &data, QVariantMap &map);
    bool parseCor(const QString &data, QVariantMap &map);
    bool parseZone(const QString &data, QVariantMap &map);
    bool parseTime(const QString &data, QVariantMap &map);
    bool parseNil(const QString &data, QVariantMap &map);
    bool parseAuto(const QString &data, QVariantMap &map);
    bool parseWind(const QString &data, QVariantMap &map);
    bool parseWindVariations(const QString &data, QVariantMap &map);
    bool parseVisibility(const QString &data, QVariantMap &map);
    bool parseMaxVisibility(const QString &data, QVariantMap &map);
    bool parseRunwayVisibility(const QString &data, QVariantMap &map);
    bool parseWeatherEvent(const QString &data, QVariantList &list);
    bool parseCloudinessType(const QString &data, QVariantMap &map);
    bool parseClouds(const QString &data, QVariantList &list);
    bool parseVerticalVisibility(const QString &data, QVariantMap &map);
    bool parseCavok(const QString &data, QVariantMap &map);
    bool parseTemperature(const QString &data, QVariantMap &map);
    bool parsePressure(const QString &data, QVariantMap &map);
    bool parseRecentWeather(const QString &data, QVariantList &list);
    bool parseWs(const QString &data);
    bool parseAll(const QString &data);
    bool parseRwy(const QString &data);
    bool parseWindShear(const QString &data, QVariantMap &map);
    bool parseSeaState(const QString &data, QVariantMap &map);
    bool parseRunwayState(const QString &data, QVariantMap &map);
    bool parseTrend(const QString &data, QVariantMap &map);
    bool parseTrendTime(const QString &data, QVariantMap &map);
    bool parseTrendWind(const QString &data, QVariantMap &map);
    bool parseTrendVisibility(const QString &data, QVariantMap &map);
    bool parseTrendWeatherEvent(const QString &data, QVariantList &list);
    bool parseRmk(const QString &data);

    bool parseCnl(const QString &data, QVariantMap &map);

    bool parseTafType(const QString &data, QVariantMap &map);
    bool parseTafTime(const QString &data, QVariantMap &map);
    bool parseTafWeatherEvent(const QString &data, QVariantList &list);
    bool parseTafTrend(const QString &data, QVariantMap &map);
    bool parseTafTrendProbability(const QString &data, QVariantMap &map);
    bool parseTafTrendTime(const QString &data, QVariantMap &map);
    bool parseTafMaxTemperature(const QString &data, QVariantMap &map);
    bool parseTafMinTemperature(const QString &data, QVariantMap &map);

    int parseSigmetHeader(const QString &data, QVariantMap &map);
    int parseSigmetTarget(const QString &data, QVariantMap &map);
    int parseSigmetEvent(const QString &data, QVariantMap &map);
    int parseSigmetObservation(const QString &data, QVariantMap &map);
    int parseSigmetLocation(const QString &data, QVariantMap &map);
    int parseSigmetFlightLevel(const QString &data, QVariantMap &map);
    int parseSigmetMov(const QString &data, QVariantMap &map);
    int parseSigmetIntensityChange(const QString &data, QVariantMap &map);
    int parseSigmetCancel(const QString &data, QVariantMap &map);

    int parseSigmetVolcano(const QString &data, QVariantMap &map);
    int parseSigmetAshCloudLocation(const QString &data, QVariantMap &map);
    int parseSigmetVaForecast(const QString &data, QVariantMap &map);

    int parseAirmetHeader(const QString &data, QVariantMap &map);
    int parseAirmetEvent(const QString &data, QVariantMap &map);
    int parseAirmetCancel(const QString &data, QVariantMap &map);

    int parseGametHeader(const QString &data, QVariantMap &map);
    int parseGametFlightLevel(const QString &data, QVariantMap &map);
    int parseGametBody(const QString &data, QVariantMap &map);

protected:
    QDateTime now;
    MeteoErrors mErrors;

    QVariantMap runwayVisibility;
    QVariantList weatherEvents;
    QVariantMap cloudiness;
    QVariantList clouds;
    QVariantList recentWeather;
    QVariantMap windShear;
    QVariantMap runwayState;
    QVariantList trendWeatherEvents;
    QVariantMap trendCloudiness;
    QVariantList trendClouds;
    QVariantMap trend;
    QVariantList trends;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // METEOPARSER_H
