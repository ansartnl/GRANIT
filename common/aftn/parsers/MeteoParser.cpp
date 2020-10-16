#include "MeteoParser.h"

#include "TelegramTypes.h"
#include "Transliteration.h"

#include <QtCore/QSet>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

MeteoParser::MeteoParser()
{
}

QVariantMap MeteoParser::parse(const QString &data, MeteoParser::MeteoErrors &error)
{
    MeteoParser parser;

    QVariantMap ret = parser.parse(data);
    error = parser.errors();

    return ret;
}

QVariantMap MeteoParser::parse(const QString &data)
{
    QVariantMap ret;

    now = QDateTime::currentDateTimeUtc();
    mErrors.clear();

    ret.insert(METEO_ARRIVAL_TIME, now);
    ret.insert(METEO_MESSAGE_TEXT, data);

    QRegExp rexp("^([A-Z|А-Я]{2})([A-Z|А-Я]{2})(\\d{2})"
                 "\\s([A-Z|А-Я]{4})"
                 "\\s(\\d{2})(\\d{2})(\\d{2})"
                 "(\\s(AA|CC)([A-Z]))?"
                 "\\s*\\r?\\n");
    if (rexp.indexIn(data) == -1)
    {
        mErrors << ParserErrors::NotMeteo;
        return ret;
    }

    ret.insert(METEO_WMO_DATA_TYPE, rexp.cap(1));
    ret.insert(METEO_WMO_REGION, rexp.cap(2));
    ret.insert(METEO_WMO_BULLETIN_NUMBER, rexp.cap(3).toInt());
    ret.insert(METEO_WMO_DISSEMINATOR, rexp.cap(4));
    QDateTime transmittionTime = QDateTime(
                QDate(now.date().year(), now.date().month(), rexp.cap(5).toInt())
                , QTime(rexp.cap(6).toInt(), rexp.cap(7).toInt()), Qt::UTC);
    ret.insert(METEO_WMO_TRANSMITTION_DATE_TIME, transmittionTime);

    if (!rexp.cap(8).isEmpty())
    {
        ret.insert(METEO_WMO_ERROR_TYPE, rexp.cap(9));
        ret.insert(METEO_WMO_ERROR_NUMBER, rexp.cap(10));
    }

    if (tryParseMetar(data.mid(rexp.matchedLength()), ret))
        return ret;
    if (tryParseTaf(data.mid(rexp.matchedLength()), ret))
        return ret;
    if (tryParseSigmet(data.mid(rexp.matchedLength()), ret))
        return ret;
    if (tryParseAirmet(data.mid(rexp.matchedLength()), ret))
        return ret;
    if (tryParseGamet(data.mid(rexp.matchedLength()), ret))
        return ret;

    mErrors << ParserErrors::UnknownMeteoMessage;
    return ret;
}

MeteoParser::MeteoErrors MeteoParser::errors() const
{
    return mErrors;
}

bool MeteoParser::tryParseMetar(const QString &data, QVariantMap &map)
{
    if (!data.startsWith("METAR ") && !data.startsWith("SPECI "))
        return false;

    if (!data.contains("="))
        mErrors << ParserErrors::MissingEnding;
    if (!data.endsWith("="))
        mErrors << ParserErrors::BadEnding;

    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if (data.startsWith("METAR ") && (dataType != "SA"))
        mErrors << ParserErrors::BadWmoDataType;
    if (data.startsWith("SPECI ") && (dataType != "SP"))
        mErrors << ParserErrors::BadWmoDataType;

    QStringList fields = data.mid(0, data.indexOf("=")).split(QRegExp("\\s+"), QString::SkipEmptyParts);

    runwayVisibility.clear();
    weatherEvents.clear();
    cloudiness.clear();
    clouds.clear();
    recentWeather.clear();
    windShear.clear();
    runwayState.clear();
    trendWeatherEvents.clear();
    trendCloudiness.clear();
    trendClouds.clear();

    lastMetarField = METAR_START;
    bool finish = false;
    while (!finish && !fields.isEmpty())
    {
        if (!parseMetar(fields[0], map))
            finish = true;
        fields.removeAt(0);
    }

    if (!runwayVisibility.isEmpty())
        map.insert(METEO_RUNWAY_VISIBILIY, runwayVisibility);
    if (!weatherEvents.isEmpty())
        map.insert(METEO_WEATHER_EVENTS, weatherEvents);
    if (!clouds.isEmpty())
        cloudiness.insert(METEO_CLOUDS, clouds);
    if (!cloudiness.isEmpty())
        map.insert(METEO_CLOUDINESS, cloudiness);
    if (!recentWeather.isEmpty())
        map.insert(METEO_RECENT_WEATHER, recentWeather);
    if (!windShear.isEmpty())
        map.insert(METEO_WIND_SHEAR, windShear);
    if (!runwayState.isEmpty())
        map.insert(METEO_RUNWAY_STATE, runwayState);
    if (!trendWeatherEvents.isEmpty())
        map.insert(METEO_TREND_WEATHER_EVENTS, trendWeatherEvents);
    if (!trendClouds.isEmpty())
        trendCloudiness.insert(METEO_CLOUDS, trendClouds);
    if (!trendCloudiness.isEmpty())
        map.insert(METEO_TREND_CLOUDINESS, trendCloudiness);

    if (!fields.isEmpty())
        map.insert(METEO_RMK, fields.join(" "));

    validateMetar(map);

    return true;
}

bool MeteoParser::tryParseTaf(const QString &data, QVariantMap &map)
{
    if (!data.startsWith("TAF"))
        return false;

    if (!data.contains("="))
        mErrors << ParserErrors::MissingEnding;
    if (!data.endsWith("="))
        mErrors << ParserErrors::BadEnding;

    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if ((dataType != "FC") && (dataType != "FT"))
        mErrors << ParserErrors::BadWmoDataType;

    QStringList fields = data.mid(0, data.indexOf("=")).split(QRegExp("\\s+"), QString::SkipEmptyParts);

    weatherEvents.clear();
    cloudiness.clear();
    clouds.clear();
    trendWeatherEvents.clear();
    trendCloudiness.clear();
    trendClouds.clear();
    trend.clear();
    trends.clear();

    lastTafField = TAF_START;
    bool finish = false;
    while (!finish && !fields.isEmpty())
    {
        if (!parseTaf(fields[0], map))
            finish = true;
        fields.removeAt(0);
    }

    if (!weatherEvents.isEmpty())
        map.insert(METEO_WEATHER_EVENTS, weatherEvents);
    if (!clouds.isEmpty())
        cloudiness.insert(METEO_CLOUDS, clouds);
    if (!cloudiness.isEmpty())
        map.insert(METEO_CLOUDINESS, cloudiness);
    if (!trendWeatherEvents.isEmpty())
        trend.insert(METEO_TREND_WEATHER_EVENTS, trendWeatherEvents);
    if (!trendClouds.isEmpty())
        trendCloudiness.insert(METEO_CLOUDS, trendClouds);
    if (!trendCloudiness.isEmpty())
        trend.insert(METEO_TREND_CLOUDINESS, trendCloudiness);
    if (!trend.isEmpty())
        trends << trend;
    if (!trends.isEmpty())
        map.insert(METEO_TREND, trends);

    if (!fields.isEmpty())
        mErrors << ParserErrors::UnknownFields;

    validateTaf(map);

    return true;
}

bool MeteoParser::tryParseSigmet(const QString &data, QVariantMap &map)
{
    QRegExp sigmetRexp("^([A-Z]{4})\\sSIGMET");

    if (sigmetRexp.indexIn(data) == -1)
        return false;

    if (!data.contains("="))
        mErrors << ParserErrors::MissingEnding;
    if (!data.endsWith("="))
        mErrors << ParserErrors::BadEnding;

    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if ((dataType != "WS") && (dataType != "WC") && (dataType != "WV"))
        mErrors << ParserErrors::BadWmoDataType;

    QString fields = data.mid(0, data.indexOf("="));

    lastSigmetField = SIGMET_START;
    lastSigmetVaField = SIGMET_VA_START;
    bool finish = false;
    int size = 0;
    while (!finish && !fields.isEmpty())
    {
        if ((size = parseAnySigmet(fields, map)) == -1)
            finish = true;
        fields.remove(0, size);
    }

    map.insert(METEO_ZONE, map.value(METEO_TARGET_SECTOR).toString());

    if (!fields.isEmpty())
        mErrors << ParserErrors::UnknownFields;

    validateAnySigmet(map);

    return true;
}

bool MeteoParser::tryParseAirmet(const QString &data, QVariantMap &map)
{
    QRegExp sigmetRexp("^([A-Z]{4})\\sAIRMET");

    if (sigmetRexp.indexIn(data) == -1)
        return false;

    if (!data.contains("="))
        mErrors << ParserErrors::MissingEnding;
    if (!data.endsWith("="))
        mErrors << ParserErrors::BadEnding;

    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if (dataType != "WA")
        mErrors << ParserErrors::BadWmoDataType;

    QString fields = data.mid(0, data.indexOf("="));

    lastAirmetField = AIRMET_START;
    bool finish = false;
    int size = 0;
    while (!finish && !fields.isEmpty())
    {
        if ((size = parseAirmet(fields, map)) == -1)
            finish = true;
        fields.remove(0, size);
    }

    map.insert(METEO_ZONE, map.value(METEO_TARGET_SECTOR).toString());

    if (!fields.isEmpty())
        mErrors << ParserErrors::UnknownFields;

    validateAirmet(map);

    return true;
}

bool MeteoParser::tryParseGamet(const QString &data, QVariantMap &map)
{
    QRegExp sigmetRexp("^([A-Z]{4})\\sGAMET");

    if (sigmetRexp.indexIn(data) == -1)
        return false;

    if (!data.contains("="))
        mErrors << ParserErrors::MissingEnding;
    if (!data.endsWith("="))
        mErrors << ParserErrors::BadEnding;

    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if (dataType != "FA")
        mErrors << ParserErrors::BadWmoDataType;

    QString fields = data.mid(0, data.indexOf("="));

    lastGametField = GAMET_START;
    bool finish = false;
    int size = 0;
    while (!finish && !fields.isEmpty())
    {
        if ((size = parseGamet(fields, map)) == -1)
            finish = true;
        fields.remove(0, size);
    }

    map.insert(METEO_ZONE, map.value(METEO_TARGET_SECTOR).toString());

    if (!fields.isEmpty())
        mErrors << ParserErrors::UnknownFields;

    validateGamet(map);

    return true;
}

bool MeteoParser::parseMetar(const QString &data, QVariantMap &map)
{
    bool first = true;

    switch (lastMetarField)
    {
    case METAR_START:
        if (parseMetarType(data, map))
        {
            lastMetarField = METAR_TYPE;
            return true;
        }
        mErrors << ParserErrors::MetarTypeField;
        first = false;
    case METAR_TYPE:
        if (parseCor(data, map))
        {
            lastMetarField = METAR_COR;
            return true;
        }
    case METAR_COR:
        if (parseZone(data, map))
        {
            lastMetarField = METAR_ZONE;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarZoneField))
            {
                mErrors << ParserErrors::MetarZoneField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_ZONE:
        if (parseTime(data, map))
        {
            lastMetarField = METAR_TIME;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarTimeField))
            {
                mErrors << ParserErrors::MetarTimeField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_TIME:
        if (parseNil(data, map))
        {
            lastMetarField = METAR_NIL;
            return true;
        }
    case METAR_NIL:
        if (parseAuto(data, map))
        {
            lastMetarField = METAR_AUTO;
            return true;
        }
    case METAR_AUTO:
        if (parseWind(data, map))
        {
            lastMetarField = METAR_WIND;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarWindField))
            {
                mErrors << ParserErrors::MetarWindField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_WIND:
        if (parseWindVariations(data, map))
        {
            lastMetarField = METAR_WIND_VARIATIONS;
            return true;
        }
    case METAR_WIND_VARIATIONS:
        if (parseCavok(data, map))
        {
            lastMetarField = METAR_CAVOK;
            return true;
        }
        if (parseVisibility(data, map))
        {
            lastMetarField = METAR_VISIBILITY;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarVisibilityField))
            {
                mErrors << ParserErrors::MetarVisibilityField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_VISIBILITY:
        if (parseMaxVisibility(data, map))
        {
            lastMetarField = METAR_MAX_VISIBILITY;
            return true;
        }
    case METAR_MAX_VISIBILITY:
    case METAR_RUNWAY_VISIBILITY:
        if (parseRunwayVisibility(data, runwayVisibility))
        {
            lastMetarField = METAR_RUNWAY_VISIBILITY;
            return true;
        }
    case METAR_WEATHER_EVENT:
        if (parseWeatherEvent(data, weatherEvents))
        {
            lastMetarField = METAR_WEATHER_EVENT;
            return true;
        }
        if (parseCloudinessType(data, cloudiness))
        {
            lastMetarField = METAR_CLOUDINESS_TYPE;
            return true;
        }
        if (parseClouds(data, clouds))
        {
            lastMetarField = METAR_CLOUDS;
            return true;
        }
        if (parseVerticalVisibility(data, cloudiness))
        {
            lastMetarField = METAR_VERTICAL_VISIBILITY;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarCloudinessField)
                    && map.contains(METEO_VISIBILITY)) // проверка на то, не ошибочный CAVOK ли это
            {
                mErrors << ParserErrors::MetarCloudinessField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_CLOUDS:
        if (parseClouds(data, clouds))
        {
            lastMetarField = METAR_CLOUDS;
            return true;
        }
    case METAR_CLOUDINESS_TYPE:
    case METAR_VERTICAL_VISIBILITY:
    case METAR_CAVOK:
        if (parseTemperature(data, map))
        {
            lastMetarField = METAR_TEMPERATURE;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarTemperatureField))
            {
                mErrors << ParserErrors::MetarTemperatureField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_TEMPERATURE:
        if (parsePressure(data, map))
        {
            lastMetarField = METAR_PRESSURE;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarPressureField))
            {
                mErrors << ParserErrors::MetarPressureField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_PRESSURE:
    case METAR_RECENT_WEATHER:
        if (parseRecentWeather(data, recentWeather))
        {
            lastMetarField = METAR_RECENT_WEATHER;
            return true;
        }
        if (parseWs(data))
        {
            lastMetarField = METAR_WS;
            return true;
        }
        if (parseSeaState(data, map))
        {
            lastMetarField = METAR_SEA_STATE;
            return true;
        }
        if (parseRunwayState(data, runwayState))
        {
            lastMetarField = METAR_RUNWAY_STATE;
            return true;
        }
        if (parseTrend(data, map))
        {
            lastMetarField = METAR_TREND;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarTrendField))
            {
                mErrors << ParserErrors::MetarTrendField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_WS:
        if (parseAll(data))
        {
            lastMetarField = METAR_ALL;
            return true;
        }
        if (parseWindShear(data, windShear))
        {
            lastMetarField = METAR_WIND_SHEAR;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarWindShearField))
            {
                mErrors << ParserErrors::MetarWindShearField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_ALL:
        if (parseRwy(data))
        {
            map.insert(METEO_WIND_SHEAR_ALL_RUNWAYS, true);
            lastMetarField = METAR_RWY;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarWindShearField))
            {
                mErrors << ParserErrors::MetarWindShearField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_WIND_SHEAR:
        if (parseWs(data))
        {
            lastMetarField = METAR_WS;
            return true;
        }
    case METAR_RWY:
        if (parseSeaState(data, map))
        {
            lastMetarField = METAR_SEA_STATE;
            return true;
        }
    case METAR_SEA_STATE:
    case METAR_RUNWAY_STATE:
        if (parseRunwayState(data, runwayState))
        {
            lastMetarField = METAR_RUNWAY_STATE;
            return true;
        }
        if (parseTrend(data, map))
        {
            lastMetarField = METAR_TREND;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarTrendField))
            {
                mErrors << ParserErrors::MetarTrendField;
                first = false;
            }
        } else
        {
            return true;
        }
    case METAR_TREND:
    {
        QString trend = map.value(METEO_TREND_TYPE).toString();
        if (trend == "NOSIG")
        {
            if (parseRmk(data))
            {
                lastMetarField = METAR_RMK;
                return true;
            }
            if (first)
            {
                if (!mErrors.contains(ParserErrors::ExtraField))
                {
                    mErrors << ParserErrors::ExtraField;
                    first = false;
                }
            } else
            {
                return true;
            }
        } else
        {
            if (parseTrendTime(data, map))
            {
                lastMetarField = METAR_TIME;
                return true;
            }
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::MetarTrendField))
            {
                mErrors << ParserErrors::MetarTrendField;
                first = false;
            }
        } else
        {
            return true;
        }
    }
    case METAR_TREND_TIME:
        if (map.contains(METEO_TREND_FROM) && !map.contains(METEO_TREND_TILL))
        {
            if (parseTrendTime(data, map))
            {
                lastMetarField = METAR_TIME;
                return true;
            }
        }
        if (parseTrendWind(data, map))
        {
            lastMetarField = METAR_TREND_WIND;
            return true;
        }
    case METAR_TREND_WIND:
        if (parseTrendVisibility(data, map))
        {
            lastMetarField = METAR_TREND_VISIBILITY;
            return true;
        }
    case METAR_TREND_VISIBILITY:
    case METAR_TREND_WEATHER_EVENT:
        if (parseTrendWeatherEvent(data, trendWeatherEvents))
        {
            lastMetarField = METAR_TREND_WEATHER_EVENT;
            return true;
        }
        if (parseCloudinessType(data, trendCloudiness))
        {
            lastMetarField = METAR_TREND_CLOUDINESS_TYPE;
            return true;
        }
        if (parseVerticalVisibility(data, trendCloudiness))
        {
            lastMetarField = METAR_TREND_CLOUDS;
            return true;
        }
    case METAR_TREND_CLOUDS:
        if (parseClouds(data, trendClouds))
        {
            lastMetarField = METAR_TREND_CLOUDS;
            return true;
        }
    case METAR_TREND_CLOUDINESS_TYPE:
    case METAR_TREND_VERTICAL_VISIBILITY:
        if (parseRmk(data))
        {
            lastMetarField = METAR_RMK;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::ExtraField))
            {
                mErrors << ParserErrors::ExtraField;
                return false;
            }
        } else
        {
            return true;
        }
    case METAR_RMK:
        mErrors << ParserErrors::ExtraField;
        return false;
    }
    mErrors << ParserErrors::UnknownFields;
    return false;
}

void MeteoParser::validateMetar(const QVariantMap &map)
{
    // time
    if (!map.value(METEO_TIME).toDateTime().isValid())
        mErrors << ParserErrors::MetarBadTime;

    // wind
    if (map.value(METEO_WIND_DIRECTION).toString() != "VRB")
    {
        if (!map.value(METEO_WIND_DIRECTION).toInt() && map.value(METEO_WIND_AVARAGE_SPEED).toInt())
            mErrors << ParserErrors::MetarBadWind;
    }

    // runway state
    if (map.contains(METEO_RUNWAY_STATE))
    {
        QSet < int > ranges;
        ranges.insert(1);
        ranges.insert(2);
        ranges.insert(5);
        ranges.insert(9);

        QVariantMap runwayStates = map.value(METEO_RUNWAY_STATE).value < QVariantMap >();
        foreach (const QVariant &rs, runwayStates)
        {
            QVariantMap rsMap = rs.value < QVariantMap >();
            if (rsMap.contains(METEO_RUNWAY_COVERING_RANGE))
            {
                if (!ranges.contains(rsMap.value(METEO_RUNWAY_COVERING_RANGE).toInt()))
                    mErrors << ParserErrors::MetarBadRunwayCoveringRange;
            }
            if (rsMap.contains(METEO_RUNWAY_COVERING_THICK))
            {
                if (rsMap.value(METEO_RUNWAY_COVERING_THICK).toInt() == 91)
                    mErrors << ParserErrors::MetarBadRunwayCoveringThick;
            }
            if (rsMap.contains(METEO_RUNWAY_COHESION))
            {
                int cohesion = rsMap.value(METEO_RUNWAY_COHESION).toInt();
                if ((cohesion > 95) && (cohesion < 99))
                    mErrors << ParserErrors::MetarBadRunwayCohesion;
            }
        }
    }

    // trend
    if (map.value(METEO_TREND_TYPE).toString() != "NOSIG")
    {
        if (!map.contains(METEO_TREND_WIND_DIRECTION)
                && !map.contains(METEO_TREND_VISIBILITY)
                && !map.contains(METEO_TREND_WEATHER_EVENTS)
                && !map.contains(METEO_TREND_CLOUDINESS))
            mErrors << ParserErrors::MetarBadTrend;
        if (map.contains(METEO_TREND_FROM) && !map.value(METEO_TREND_FROM).toDateTime().isValid())
            mErrors << ParserErrors::MetarBadTrendTime;
        if (map.contains(METEO_TREND_TILL) && !map.value(METEO_TREND_TILL).toDateTime().isValid())
            mErrors << ParserErrors::MetarBadTrendTime;
        if (map.contains(METEO_TREND_FROM)
                && map.contains(METEO_TREND_TILL)
                && map.value(METEO_TREND_FROM).toDateTime().isValid()
                && map.value(METEO_TREND_TILL).toDateTime().isValid())
        {
            if (map.value(METEO_TREND_FROM).toTime() > map.value(METEO_TREND_TILL).toTime())
                mErrors << ParserErrors::MetarBadTrendTime;
        }
    }
}

bool MeteoParser::parseTaf(const QString &data, QVariantMap &map)
{
    bool first = true;

    switch (lastTafField)
    {
    case TAF_START:
        if (parseTafType(data, map))
        {
            lastTafField = TAF_TYPE;
            return true;
        }
        mErrors << ParserErrors::TafTypeField;
        first = false;
    case TAF_TYPE:
        if (parseCor(data, map))
        {
            lastTafField = TAF_COR;
            return true;
        }
    case TAF_COR:
        if (parseZone(data, map))
        {
            lastTafField = TAF_ZONE;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafZoneField))
            {
                mErrors << ParserErrors::TafZoneField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_ZONE:
        if (parseTime(data, map))
        {
            lastTafField = TAF_TIME;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafTimeField))
            {
                mErrors << ParserErrors::TafTimeField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_TIME:
        if (parseTafTime(data, map))
        {
            lastTafField = TAF_VALID_TIME;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafValidTimeField))
            {
                mErrors << ParserErrors::TafValidTimeField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_VALID_TIME:
        if (parseWind(data, map))
        {
            lastTafField = TAF_WIND;
            return true;
        }
        if (parseNil(data, map))
        {
            lastTafField = TAF_NIL;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafWindField))
            {
                mErrors << ParserErrors::TafWindField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_WIND:
        if (parseCavok(data, map))
        {
            lastTafField = TAF_CAVOK;
            return true;
        }
        if (parseVisibility(data, map))
        {
            lastTafField = TAF_VISIBILITY;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafVisibilityField))
            {
                mErrors << ParserErrors::TafVisibilityField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_VISIBILITY:
    case TAF_WEATHER_EVENT:
        if (parseTafWeatherEvent(data, weatherEvents))
        {
            lastTafField = TAF_WEATHER_EVENT;
            return true;
        }
        if (parseCloudinessType(data, cloudiness))
        {
            lastTafField = TAF_CLOUDINESS_TYPE;
            return true;
        }
        if (parseClouds(data, clouds))
        {
            lastTafField = TAF_CLOUDS;
            return true;
        }
        if (parseVerticalVisibility(data, cloudiness))
        {
            lastTafField = TAF_VERTICAL_VISIBILITY;
            return true;
        }
        if (first)
        {
            if (!mErrors.contains(ParserErrors::TafCloudinessField)
                    && map.contains(METEO_VISIBILITY)) // проверка на то, не ошибочный CAVOK ли это
            {
                mErrors << ParserErrors::TafCloudinessField;
                first = false;
            }
        } else
        {
            return true;
        }
    case TAF_CLOUDS:
        if (parseClouds(data, clouds))
        {
            lastTafField = TAF_CLOUDS;
            return true;
        }
    case TAF_CLOUDINESS_TYPE:
    case TAF_VERTICAL_VISIBILITY:
    case TAF_CAVOK:
        if (parseTafTrendProbability(data, trend))
        {
            lastTafField = TAF_TREND_PROBABILITY;
            return true;
        }
    case TAF_TREND_PROBABILITY:
        if (parseTafTrend(data, trend))
        {
            lastTafField = TAF_TREND_TYPE;
            return true;
        }
        if (parseTafMaxTemperature(data, map))
        {
            lastTafField = TAF_MAX_TEMPERATURE;
            return true;
        }
        if (parseTafMinTemperature(data, map))
        {
            lastTafField = TAF_MIN_TEMPERATURE;
            return true;
        }
        return !first;
    case TAF_TREND_TYPE:
    {
        QString trendType = trend.value(METEO_TREND_TYPE).toString();
        if ((trendType == "BECMG") || (trendType == "TEMPO"))
        {
            if (parseTafTrendTime(data, trend))
            {
                lastTafField = TAF_TREND_TIME;
                return true;
            }
            if (first)
            {
                if (!mErrors.contains(ParserErrors::TafTrendTimeField))
                {
                    mErrors << ParserErrors::TafTrendTimeField;
                    first = false;
                }
            } else
            {
                return true;
            }
        }
    }
    case TAF_TREND_TIME:
        if (parseCavok(data, trend))
        {
            lastTafField = TAF_TREND_CAVOK;
            return true;
        }
        if (parseTrendWind(data, trend))
        {
            lastTafField = TAF_TREND_WIND;
            return true;
        }
    case TAF_TREND_WIND:
        if (parseTrendVisibility(data, trend))
        {
            lastTafField = TAF_TREND_VISIBILITY;
            return true;
        }
    case TAF_TREND_VISIBILITY:
    case TAF_TREND_WEATHER_EVENT:
        if (parseTrendWeatherEvent(data, trendWeatherEvents))
        {
            lastTafField = TAF_TREND_WEATHER_EVENT;
            return true;
        }
        if (parseCloudinessType(data, trendCloudiness))
        {
            lastTafField = TAF_TREND_CLOUDINESS_TYPE;
            return true;
        }
        if (parseVerticalVisibility(data, trendCloudiness))
        {
            lastTafField = TAF_TREND_CLOUDS;
            return true;
        }
    case TAF_TREND_CLOUDS:
        if (parseClouds(data, trendClouds))
        {
            lastTafField = TAF_TREND_CLOUDS;
            return true;
        }
    case TAF_TREND_CLOUDINESS_TYPE:
    case TAF_TREND_VERTICAL_VISIBILITY:
    case TAF_TREND_CAVOK:
        if (parseTafTrendProbability(data, trend))
        {
            lastTafField = TAF_TREND_PROBABILITY;
            return true;
        }
        if (parseTafTrend(data, trend))
        {
            lastTafField = TAF_TREND_TYPE;
            return true;
        }
        if (parseTafMaxTemperature(data, map))
        {
            lastTafField = TAF_MAX_TEMPERATURE;
            return true;
        }
    case TAF_MAX_TEMPERATURE:
        if (parseTafMinTemperature(data, map))
        {
            lastTafField = TAF_MIN_TEMPERATURE;
            return true;
        }
    case TAF_MIN_TEMPERATURE:
        return true;
    case TAF_NIL:
        if (parseCnl(data, map))
        {
            lastTafField = TAF_CNL;
            return false;
        }
        mErrors << ParserErrors::TafCnlField;
        return false;
    case TAF_CNL:
        mErrors << ParserErrors::ExtraField;
        return false;
    }
    mErrors << ParserErrors::UnknownFields;
    return false;
}

void MeteoParser::validateTaf(const QVariantMap &map)
{
    // valid time
    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
    {
        mErrors << ParserErrors::TafBadValidTime;
    } else
    {
        if (map.value(METEO_VALID_FROM).toDateTime() > map.value(METEO_VALID_TILL).toDateTime())
            mErrors << ParserErrors::TafBadValidTime;
    }

    // wind
    if (map.value(METEO_WIND_DIRECTION).toString() != "VRB")
    {
        if (!map.value(METEO_WIND_DIRECTION).toInt() && map.value(METEO_WIND_AVARAGE_SPEED).toInt())
            mErrors << ParserErrors::TafBadWind;
    }

    // trend
    QVariantList trends = map.value(METEO_TREND).value < QVariantList >();
    foreach (const QVariant &t, trends)
    {
        QVariantMap trend = t.value < QVariantMap >();
        if (!trend.contains(METEO_TREND_WIND_DIRECTION)
                && (!trend.contains(METEO_CAVOK)
                    || (!trend.contains(METEO_TREND_VISIBILITY)
                        && !trend.contains(METEO_TREND_WEATHER_EVENTS)
                        && !trend.contains(METEO_TREND_CLOUDINESS))))
        {
            mErrors << ParserErrors::TafBadTrend;
        }
        if (trend.contains(METEO_TREND_FROM) && !trend.value(METEO_TREND_FROM).toDateTime().isValid())
            mErrors << ParserErrors::TafBadTrendTime;
        if (trend.contains(METEO_TREND_TILL) && !trend.value(METEO_TREND_TILL).toDateTime().isValid())
            mErrors << ParserErrors::TafBadTrendTime;
        if (trend.contains(METEO_TREND_FROM)
                && trend.contains(METEO_TREND_TILL)
                && trend.value(METEO_TREND_FROM).toDateTime().isValid()
                && trend.value(METEO_TREND_TILL).toDateTime().isValid())
        {
            if (trend.value(METEO_TREND_FROM).toTime() > trend.value(METEO_TREND_TILL).toTime())
                mErrors << ParserErrors::TafBadTrendTime;
        }
    }
}

int MeteoParser::parseAnySigmet(const QString &data, QVariantMap &map)
{
    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if (dataType == "WS")
        return parseSigmet(data, map);
    else if (dataType == "WC")
        return parseSigmetTc(data, map);
    else if (dataType == "WV")
        return parseSigmetVa(data, map);
    mErrors << ParserErrors::BadWmoDataType;
    return -1;
}

void MeteoParser::validateAnySigmet(const QVariantMap &map)
{
    QString dataType = Transliteration::to4444(map.value(METEO_WMO_DATA_TYPE).toString());
    if (dataType == "WS")
        validateSigmet(map);
    else if (dataType == "WC")
        validateSigmetTc(map);
    else if (dataType == "WV")
        validateSigmetVa(map);
}

int MeteoParser::parseSigmet(const QString &data, QVariantMap &map)
{
    int ret;

    switch (lastSigmetField)
    {
    case SIGMET_START:
        ret = parseSigmetHeader(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetHeaderField))
            {
                mErrors << ParserErrors::SigmetHeaderField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_HEADER;
            return ret;
        }
    case SIGMET_HEADER:
        ret = parseSigmetTarget(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetTargetField))
            {
                mErrors << ParserErrors::SigmetTargetField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_TARGET;
            return ret;
        }
    case SIGMET_TARGET:
        ret = parseSigmetCancel(data, map);
        if (ret)
        {
            lastSigmetField = SIGMET_CANCEL;
            return ret;
        }
        ret = parseSigmetEvent(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetEventField))
            {
                mErrors << ParserErrors::SigmetEventField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_EVENT;
            return ret;
        }
    case SIGMET_EVENT:
        ret = parseSigmetObservation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetObservationField))
            {
                mErrors << ParserErrors::SigmetObservationField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_OBSERVATION;
            return ret;
        }
    case SIGMET_OBSERVATION:
        ret = parseSigmetLocation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetLocationField))
            {
                mErrors << ParserErrors::SigmetLocationField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_LOCATION;
            return ret;
        }
    case SIGMET_LOCATION:
        ret = parseSigmetFlightLevel(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetFlightLevelField))
            {
                mErrors << ParserErrors::SigmetFlightLevelField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_FLIGHT_LEVEL;
            return ret;
        }
    case SIGMET_FLIGHT_LEVEL:
        ret = parseSigmetMov(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetMovField))
            {
                mErrors << ParserErrors::SigmetMovField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_MOV;
            return ret;
        }
    case SIGMET_MOV:
        ret = parseSigmetIntensityChange(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetIntensityChangeField))
            {
                mErrors << ParserErrors::SigmetIntensityChangeField;
                return 0;
            }
        } else
        {
            lastSigmetField = SIGMET_INTENSITY_CHANGE;
            return ret;
        }
    case SIGMET_INTENSITY_CHANGE:
    case SIGMET_CANCEL:
        return -1;
    }
    mErrors << ParserErrors::UnknownFields;
    return -1;
}

void MeteoParser::validateSigmet(const QVariantMap &map)
{
    // valid time
    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
    {
        mErrors << ParserErrors::SigmetBadValidTime;
    } else
    {
        qint64 maxValidLength = 4 * 60 * 60 * 1000;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) > maxValidLength)
            mErrors << ParserErrors::SigmetBadValidTime;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) < 0)
            mErrors << ParserErrors::SigmetBadValidTime;
    }
}

int MeteoParser::parseSigmetTc(const QString &data, QVariantMap &map)
{
    Q_UNUSED(data)
    Q_UNUSED(map)
    mErrors << ParserErrors::SigmetTropicalCycloneNotSupported;
    return -1;
}

void MeteoParser::validateSigmetTc(const QVariantMap &map)
{
    Q_UNUSED(map)
    // valid time
//    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
//            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
//    {
//        mErrors << ParserErrors::SigmetBadValidTime;
//    } else
//    {
//        qint64 maxValidLength = 4 * 60 * 60 * 1000;
//        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
//                    map.value(METEO_VALID_TILL).toDateTime()) > maxValidLength)
//            mErrors << ParserErrors::SigmetBadValidTime;
//        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
//                    map.value(METEO_VALID_TILL).toDateTime()) < 0)
//            mErrors << ParserErrors::SigmetBadValidTime;
//    }
}

int MeteoParser::parseSigmetVa(const QString &data, QVariantMap &map)
{
    int ret;

    switch (lastSigmetVaField)
    {
    case SIGMET_VA_START:
        ret = parseSigmetHeader(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetHeaderField))
            {
                mErrors << ParserErrors::SigmetHeaderField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_HEADER;
            return ret;
        }
    case SIGMET_VA_HEADER:
        ret = parseSigmetTarget(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetTargetField))
            {
                mErrors << ParserErrors::SigmetTargetField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_TARGET;
            return ret;
        }
    case SIGMET_VA_TARGET:
        ret = parseSigmetCancel(data, map);
        if (ret)
        {
            lastSigmetVaField = SIGMET_VA_CANCEL;
            return ret;
        }
        ret = parseSigmetVolcano(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetVolcanoField))
            {
                mErrors << ParserErrors::SigmetVolcanoField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_VOLCANO;
            return ret;
        }
    case SIGMET_VA_VOLCANO:
        ret = parseSigmetObservation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetObservationField))
            {
                mErrors << ParserErrors::SigmetObservationField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_OBSERVATION;
            return ret;
        }
    case SIGMET_VA_OBSERVATION:
        ret = parseSigmetAshCloudLocation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetAshCloudLocationField))
            {
                mErrors << ParserErrors::SigmetAshCloudLocationField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_LOCATION;
            return ret;
        }
    case SIGMET_VA_LOCATION:
        ret = parseSigmetMov(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetMovField))
            {
                mErrors << ParserErrors::SigmetMovField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_MOV;
            return ret;
        }
    case SIGMET_VA_MOV:
        ret = parseSigmetVaForecast(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::SigmetVaForecastField))
            {
                mErrors << ParserErrors::SigmetVaForecastField;
                return 0;
            }
        } else
        {
            lastSigmetVaField = SIGMET_VA_FORECAST;
            return ret;
        }
    case SIGMET_VA_FORECAST:
    case SIGMET_VA_CANCEL:
        return -1;
    }
    mErrors << ParserErrors::UnknownFields;
    return -1;
}

void MeteoParser::validateSigmetVa(const QVariantMap &map)
{
    // valid time
    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
    {
        mErrors << ParserErrors::SigmetBadValidTime;
    } else
    {
        qint64 maxValidLength = 6 * 60 * 60 * 1000;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) > maxValidLength)
            mErrors << ParserErrors::SigmetBadValidTime;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) < 0)
            mErrors << ParserErrors::SigmetBadValidTime;
    }
}

int MeteoParser::parseAirmet(const QString &data, QVariantMap &map)
{
    int ret;

    switch (lastAirmetField)
    {
    case AIRMET_START:
        ret = parseAirmetHeader(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetHeaderField))
            {
                mErrors << ParserErrors::AirmetHeaderField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_HEADER;
            return ret;
        }
    case AIRMET_HEADER:
        ret = parseSigmetTarget(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetTargetField))
            {
                mErrors << ParserErrors::AirmetTargetField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_TARGET;
            return ret;
        }
    case AIRMET_TARGET:
        ret = parseAirmetCancel(data, map);
        if (ret)
        {
            lastAirmetField = AIRMET_CANCEL;
            return ret;
        }
        ret = parseAirmetEvent(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetEventField))
            {
                mErrors << ParserErrors::AirmetEventField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_EVENT;
            return ret;
        }
    case AIRMET_EVENT:
        ret = parseSigmetObservation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetObservationField))
            {
                mErrors << ParserErrors::AirmetObservationField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_OBSERVATION;
            return ret;
        }
    case AIRMET_OBSERVATION:
        ret = parseSigmetLocation(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetLocationField))
            {
                mErrors << ParserErrors::AirmetLocationField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_LOCATION;
            return ret;
        }
    case AIRMET_LOCATION:
        ret = parseSigmetFlightLevel(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetFlightLevelField))
            {
                mErrors << ParserErrors::AirmetFlightLevelField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_FLIGHT_LEVEL;
            return ret;
        }
    case AIRMET_FLIGHT_LEVEL:
        ret = parseSigmetMov(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetMovField))
            {
                mErrors << ParserErrors::AirmetMovField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_MOV;
            return ret;
        }
    case AIRMET_MOV:
        ret = parseSigmetIntensityChange(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::AirmetIntensityChangeField))
            {
                mErrors << ParserErrors::AirmetIntensityChangeField;
                return 0;
            }
        } else
        {
            lastAirmetField = AIRMET_INTENSITY_CHANGE;
            return ret;
        }
    case AIRMET_INTENSITY_CHANGE:
    case AIRMET_CANCEL:
        return -1;
    }
    mErrors << ParserErrors::UnknownFields;
    return -1;
}

void MeteoParser::validateAirmet(const QVariantMap &map)
{
    // valid time
    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
    {
        mErrors << ParserErrors::AirmetBadValidTime;
    } else
    {
        qint64 maxValidLength = 4 * 60 * 60 * 1000;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) > maxValidLength)
            mErrors << ParserErrors::AirmetBadValidTime;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) < 0)
            mErrors << ParserErrors::AirmetBadValidTime;
    }
}

int MeteoParser::parseGamet(const QString &data, QVariantMap &map)
{
    int ret;

    switch (lastGametField)
    {
    case GAMET_START:
        ret = parseGametHeader(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::GametHeaderField))
            {
                mErrors << ParserErrors::GametHeaderField;
                return 0;
            }
        } else
        {
            lastGametField = GAMET_HEADER;
            return ret;
        }
    case GAMET_HEADER:
        ret = parseSigmetTarget(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::GametTargetField))
            {
                mErrors << ParserErrors::GametTargetField;
                return 0;
            }
        } else
        {
            lastGametField = GAMET_TARGET;
            return ret;
        }
    case GAMET_TARGET:
        ret = parseGametFlightLevel(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::GametFlightLevelField))
            {
                mErrors << ParserErrors::GametFlightLevelField;
                return 0;
            }
        } else
        {
            lastGametField = GAMET_FLIGHT_LEVEL;
            return ret;
        }
    case GAMET_FLIGHT_LEVEL:
        ret = parseGametBody(data, map);
        if (!ret)
        {
            if (!mErrors.contains(ParserErrors::GametBody))
            {
                mErrors << ParserErrors::GametBody;
                return 0;
            }
        } else
        {
            lastGametField = GAMET_BODY;
            return ret;
        }
    case GAMET_BODY:
        return -1;
    }
    mErrors << ParserErrors::UnknownFields;
    return -1;
}

void MeteoParser::validateGamet(const QVariantMap &map)
{
    // valid time
    if (!map.value(METEO_VALID_FROM).toDateTime().isValid()
            || !map.value(METEO_VALID_TILL).toDateTime().isValid())
    {
        mErrors << ParserErrors::GametBadValidTime;
    } else
    {
        qint64 maxValidLength = 6 * 60 * 60 * 1000;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) > maxValidLength)
            mErrors << ParserErrors::GametBadValidTime;
        if (map.value(METEO_VALID_FROM).toDateTime().msecsTo(
                    map.value(METEO_VALID_TILL).toDateTime()) < 0)
            mErrors << ParserErrors::GametBadValidTime;
    }
}

bool MeteoParser::parseMetarType(const QString &data, QVariantMap &map)
{
    if (data == "METAR")
        map.insert(METEO_MESSAGE_TYPE, TelegramType::METAR);
    else if (data == "SPECI")
        map.insert(METEO_MESSAGE_TYPE, TelegramType::SPECI);
    else
        return false;
    return true;
}

bool MeteoParser::parseCor(const QString &data, QVariantMap &map)
{
    if ((data == "COR") || (data == "AMD"))
    {
        map.insert(METEO_COR, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseZone(const QString &data, QVariantMap &map)
{
    QRegExp rexp("[A-Z]{4}");
    if (rexp.exactMatch(data))
    {
        map.insert(METEO_ZONE, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseTime(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{2})?(\\d{2})(\\d{2})Z");
    if (!rexp.exactMatch(data))
        return false;

    QDateTime now = QDateTime::currentDateTimeUtc();
    QTime metarTime(rexp.cap(2).toInt(), rexp.cap(3).toInt());
    if (!metarTime.isValid())
        return false;

    QDateTime tomorrow;
    QDateTime today;
    QDateTime yesterday;

    QString dayString = rexp.cap(1);
    if (!dayString.isEmpty())
    {
        int year, month, day;
        now.date().getDate(&year, &month, &day);
        day = dayString.toInt();
        today = QDateTime(QDate(year, month, day), metarTime, Qt::UTC);
        tomorrow = today.addMonths(1);
        yesterday = today.addMonths(-1);
    }
    else
    {
        today = QDateTime(now.date().addDays(1), metarTime, Qt::UTC);
        tomorrow = today.addDays(1);
        yesterday = today.addDays(-1);
    }

    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(today.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));

    QDateTime date;

    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        date = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        date = yesterday;
    else
        date = today;

    if (!date.isValid())
        return false;

    map.insert(METEO_TIME, date);
    return true;
}

bool MeteoParser::parseNil(const QString &data, QVariantMap &map)
{
    if (data == "NIL")
    {
        map.insert(METEO_NIL, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseAuto(const QString &data, QVariantMap &map)
{
    if (data == "AUTO")
    {
        map.insert(METEO_AUTO, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseWind(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{3}|VRB)(\\d{2})(G(P)?(\\d{2}))?(MPS|KT|KMH)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_WIND_DIRECTION, rexp.cap(1));
    map.insert(METEO_WIND_AVARAGE_SPEED, rexp.cap(2).toInt());
    if (!rexp.cap(3).isEmpty())
    {
        map.insert(METEO_WIND_GUSTS, rexp.cap(5).toInt());
        if (!rexp.cap(4).isEmpty())
            map.insert(METEO_WIND_IS_ABOVE, rexp.cap(5).toInt());
    }
    map.insert(METEO_WIND_SPEED_TYPE, rexp.cap(6));

    return true;
}

bool MeteoParser::parseWindVariations(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{3})V(\\d{3})");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_WIND_VARIES_FROM, rexp.cap(1).toInt());
    map.insert(METEO_WIND_VARIES_TO, rexp.cap(2).toInt());

    return true;
}

bool MeteoParser::parseVisibility(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{4})(N|NE|E|SE|S|SW|W|NW|NDV)?");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_VISIBILITY, rexp.cap(1).toInt());
    if (!rexp.cap(2).isEmpty())
        map.insert(METEO_VISIBILITY_DIRECTION, rexp.cap(2));

    return true;
}

bool MeteoParser::parseMaxVisibility(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{4})(N|NE|E|SE|S|SW|W|NW|NDV)?");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_VISIBILITY_MAXIMUM, rexp.cap(1).toInt());
    if (!rexp.cap(2).isEmpty())
        map.insert(METEO_VISIBILITY_MAXIMUM_DIRECTION, rexp.cap(2));

    return true;
}

bool MeteoParser::parseRunwayVisibility(const QString &data, QVariantMap &map)
{
    QRegExp rexp("R(\\d{2})(LL|L|C|R|RR)?/([MP])?(\\d{4})(V([MP])?(\\d{4}))?([NDU])?");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap rvMap;

    rvMap.insert(METEO_RUNWAY_ID, rexp.cap(1).toInt());
    if (!rexp.cap(2).isEmpty())
        rvMap.insert(METEO_RUNWAY_PLACE, rexp.cap(2));
    if (!rexp.cap(3).isEmpty())
        rvMap.insert(METEO_RUNWAY_VISIBILIY_TYPE, rexp.cap(3));
    rvMap.insert(METEO_RUNWAY_VISIBILIY_VALUE, rexp.cap(4).toInt());
    if (!rexp.cap(5).isEmpty())
    {
        if (!rexp.cap(6).isEmpty())
            rvMap.insert(METEO_RUNWAY_VISIBILIY_MAX_TYPE, rexp.cap(6));
        rvMap.insert(METEO_RUNWAY_VISIBILIY_MAX_VALUE, rexp.cap(7).toInt());
    }
    if (!rexp.cap(8).isEmpty())
        rvMap.insert(METEO_RUNWAY_VISIBILIY_TENDENCY, rexp.cap(8));

    map.insert(rexp.cap(1) + rexp.cap(2), rvMap);

    return true;
}

bool MeteoParser::parseWeatherEvent(const QString &data, QVariantList &list)
{
    QRegExp rexp("(\\+|-)?"
                 "(VC)?"
                 "((MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+"
                 "|(MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+)");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap weMap;

    if (!rexp.cap(1).isEmpty())
        weMap.insert(METEO_WEATHER_EVENT_STRENGTH, rexp.cap(1));
    if (!rexp.cap(2).isEmpty())
        weMap.insert(METEO_WEATHER_EVENT_IN_VICINITY, rexp.cap(2));
    weMap.insert(METEO_WEATHER_EVENT, rexp.cap(3));

    list.append(weMap);

    return true;
}

bool MeteoParser::parseCloudinessType(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(NSC|NCD|SKC)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_CLOUDS_TYPE, rexp.cap(1));

    return true;
}

bool MeteoParser::parseClouds(const QString &data, QVariantList &list)
{
    QRegExp rexp("(FEW|SCT|BKN|OVC)(\\d{3})(CB|TCU|///)?");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap cMap;

    cMap.insert(METEO_CLOUDS_COUNT, rexp.cap(1));
    cMap.insert(METEO_CLOUDS_HEIGHT, rexp.cap(2).toInt());
    if (!rexp.cap(3).isEmpty())
        cMap.insert(METEO_CLOUDS_TYPE, rexp.cap(3));

    list.append(cMap);

    return true;
}

bool MeteoParser::parseVerticalVisibility(const QString &data, QVariantMap &map)
{
    QRegExp rexp("VV(\\d{3}|///)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_VERTICAL_VISIBILITY, rexp.cap(1));

    return true;
}

bool MeteoParser::parseCavok(const QString &data, QVariantMap &map)
{
    if (data == "CAVOK")
    {
        map.insert(METEO_CAVOK, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseTemperature(const QString &data, QVariantMap &map)
{
    QRegExp rexp("((M)?(\\d{2})/(M)?(\\d{2}))");
    if (!rexp.exactMatch(data))
        return false;

    int temperature = rexp.cap(3).toInt();
    if (!rexp.cap(2).isEmpty())
        temperature *= -1;

    int dewPoint = rexp.cap(5).toInt();
    if (!rexp.cap(4).isEmpty())
        dewPoint *= -1;

    map.insert(METEO_TEMPERATURE, temperature);
    map.insert(METEO_DEW_POINT, dewPoint);

    return true;
}

bool MeteoParser::parsePressure(const QString &data, QVariantMap &map)
{
    QRegExp rexp("([QA])(\\d{4})");
    if (!rexp.exactMatch(data))
        return false;

    if (rexp.cap(1) == "Q")
        map.insert(METEO_PRESSURE_QNH_HP, rexp.cap(2).toInt());
    else if (rexp.cap(1) == "A")
        map.insert(METEO_PRESSURE_QNH_MM, rexp.cap(2).toInt());

    return true;
}

bool MeteoParser::parseRecentWeather(const QString &data, QVariantList &list)
{
    QRegExp rexp("RE((MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+"
                 "|(MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+)");
    if (!rexp.exactMatch(data))
        return false;

    list.append(rexp.cap(1));

    return true;
}

bool MeteoParser::parseWs(const QString &data)
{
    return (data == "WS");
}

bool MeteoParser::parseAll(const QString &data)
{
    return (data == "ALL");
}

bool MeteoParser::parseRwy(const QString &data)
{
    return (data == "RWY");
}

bool MeteoParser::parseWindShear(const QString &data, QVariantMap &map)
{
    QRegExp rexp("R(\\d{2})(LL|L|C|R|RR)?");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap wsMap;

    wsMap.insert(METEO_RUNWAY_ID, rexp.cap(1).toInt());
    if (!rexp.cap(2).isEmpty())
        wsMap.insert(METEO_RUNWAY_PLACE, rexp.cap(2));

    map.insert(rexp.cap(1) + rexp.cap(2), wsMap);

    return true;
}

bool MeteoParser::parseSeaState(const QString &data, QVariantMap &map)
{
    QRegExp rexp("W(\\d{2})/S(\\d)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_SEA_TEMPERATURE, rexp.cap(1).toInt());
    map.insert(METEO_SEA_STATE, rexp.cap(2).toInt());

    return true;
}

bool MeteoParser::parseRunwayState(const QString &data, QVariantMap &map)
{
    QRegExp rexp("R?(\\d{2})/?((CLRD|(\\d|/)(\\d|/)(\\d{2}|//))(\\d{2}|//)|SNOCLO)");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap rsMap;

    rsMap.insert(METEO_RUNWAY_ID, rexp.cap(1));
    if (rexp.cap(2) == "SNOCLO")
    {
        rsMap.insert(METEO_RUNWAY_SNOCLO, rexp.cap(2));
    } else
    {
        if (rexp.cap(3) == "CLRD")
        {
            rsMap.insert(METEO_RUNWAY_CLRD, rexp.cap(3));
        } else
        {
            if (rexp.cap(4) != "/")
                rsMap.insert(METEO_RUNWAY_COVERING_CONDITIONS, rexp.cap(4).toInt());
            if (rexp.cap(5) != "/")
                rsMap.insert(METEO_RUNWAY_COVERING_RANGE, rexp.cap(5).toInt());
            if (rexp.cap(6) != "//")
                rsMap.insert(METEO_RUNWAY_COVERING_THICK, rexp.cap(6).toInt());
        }
        if (rexp.cap(7) != "//")
            rsMap.insert(METEO_RUNWAY_COHESION, rexp.cap(7).toInt());
    }

    map.insert(rexp.cap(1), rsMap);

    return true;
}

bool MeteoParser::parseTrend(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(NOSIG|TEMPO|BECMG)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_TREND_TYPE, rexp.cap(1));

    return true;
}

bool MeteoParser::parseTrendTime(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(AT|FM|TL)(\\d{2})(\\d{2})");
    if (!rexp.exactMatch(data))
        return false;

    QTime time = QTime(rexp.cap(2).toInt(), rexp.cap(3).toInt());
    if (rexp.cap(1) == "AT")
        map.insert(METEO_TREND_AT, time);
    else if (rexp.cap(1) == "FM")
        map.insert(METEO_TREND_FROM, time);
    else if (rexp.cap(1) == "TL")
        map.insert(METEO_TREND_TILL, time);

    return true;
}

bool MeteoParser::parseTrendWind(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{3}|VRB)(\\d{2})(G(P)?(\\d{2}))?(MPS|KT|KMH)");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_TREND_WIND_DIRECTION, rexp.cap(1));
    map.insert(METEO_TREND_WIND_AVARAGE_SPEED, rexp.cap(2).toInt());
    if (!rexp.cap(3).isEmpty())
    {
        map.insert(METEO_TREND_WIND_GUSTS, rexp.cap(5).toInt());
        if (!rexp.cap(4).isEmpty())
            map.insert(METEO_TREND_WIND_IS_ABOVE, rexp.cap(5).toInt());
    }
    map.insert(METEO_TREND_WIND_TYPE, rexp.cap(6));

    return true;
}

bool MeteoParser::parseTrendVisibility(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{4})");
    if (!rexp.exactMatch(data))
        return false;

    map.insert(METEO_TREND_VISIBILITY, rexp.cap(1));

    return true;
}

bool MeteoParser::parseTrendWeatherEvent(const QString &data, QVariantList &list)
{
    QRegExp rexp("(\\+|-)?"
                 "(NSW"
                 "|((MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+"
                 "|(MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+))");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap weMap;

    if (!rexp.cap(1).isEmpty())
        weMap.insert(METEO_WEATHER_EVENT_STRENGTH, rexp.cap(1));
    weMap.insert(METEO_WEATHER_EVENT, rexp.cap(3));

    list.append(weMap);

    return true;
}

bool MeteoParser::parseRmk(const QString &data)
{
    return (data == "RMK");
}

bool MeteoParser::parseCnl(const QString &data, QVariantMap &map)
{
    if (data == "CNL")
    {
        map.insert(METEO_CANCEL, data);
        return true;
    }
    return false;
}

bool MeteoParser::parseTafType(const QString &data, QVariantMap &map)
{
    if (data == "TAF")
        map.insert(METEO_MESSAGE_TYPE, TelegramType::TAF);
    else
        return false;
    return true;
}

bool MeteoParser::parseTafTime(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{2})(\\d{2})/(\\d{2})(\\d{2})");
    QRegExp rexp2("(\\d{2})(\\d{2})(\\d{2})");

    QDateTime validFrom;
    QDateTime validTill;

    if (rexp.exactMatch(data))
    {
        validFrom = QDateTime(
                    QDate(now.date().year(), now.date().month()
                          , rexp.cap(1).toUInt() + rexp.cap(2).toUInt() / 24)
                    , QTime(rexp.cap(2).toUInt() % 24, 0)
                    , Qt::UTC);
        validTill = QDateTime(
                    QDate(now.date().year(), now.date().month()
                          , rexp.cap(3).toUInt() + rexp.cap(4).toUInt() / 24)
                    , QTime(rexp.cap(4).toUInt() % 24, 0)
                    , Qt::UTC);
    } else if (rexp2.exactMatch(data))
    {
        validFrom = QDateTime(
                    QDate(now.date().year(), now.date().month()
                          , rexp2.cap(1).toUInt() + rexp2.cap(2).toUInt() / 24)
                    , QTime(rexp2.cap(2).toUInt() % 24, 0)
                    , Qt::UTC);
        validTill = QDateTime(
                    QDate(now.date().year(), now.date().month()
                          , rexp2.cap(1).toUInt() + rexp2.cap(3).toUInt() / 24)
                    , QTime(rexp2.cap(3).toUInt() % 24, 0)
                    , Qt::UTC);
    } else
    {
        return false;
    }

    map.insert(METEO_VALID_FROM, validFrom);
    map.insert(METEO_VALID_TILL, validTill);

    return true;
}

bool MeteoParser::parseTafWeatherEvent(const QString &data, QVariantList &list)
{
    QRegExp rexp("(NSW)"
                 "|((\\+|-)?"
                 "(VC)?"
                 "((MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+"
                 "|(MI|BC|PR|DR|BL|SH|TS|FZ)"
                 "(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+))");
    if (!rexp.exactMatch(data))
        return false;

    QVariantMap weMap;

    if (!rexp.cap(1).isEmpty())
    {
        weMap.insert(METEO_WEATHER_EVENT, rexp.cap(1));
    } else
    {
        if (!rexp.cap(3).isEmpty())
            weMap.insert(METEO_WEATHER_EVENT_STRENGTH, rexp.cap(3));
        if (!rexp.cap(4).isEmpty())
            weMap.insert(METEO_WEATHER_EVENT_IN_VICINITY, rexp.cap(4));
        weMap.insert(METEO_WEATHER_EVENT, rexp.cap(5));
    }

    list.append(weMap);

    return true;
}

bool MeteoParser::parseTafTrend(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(TEMPO|BECMG|((FM)(\\d{2})(\\d{2})))");
    if (!rexp.exactMatch(data))
        return false;

    if (!trendWeatherEvents.isEmpty())
    {
        map.insert(METEO_TREND_WEATHER_EVENTS, trendWeatherEvents);
        trendWeatherEvents.clear();
    }
    if (!trendClouds.isEmpty())
    {
        trendCloudiness.insert(METEO_CLOUDS, trendClouds);
        trendClouds.clear();
    }
    if (!trendCloudiness.isEmpty())
    {
        map.insert(METEO_TREND_CLOUDINESS, trendCloudiness);
        trendCloudiness.clear();
    }
    if (map.count() > 1)
    {
        trends << map;
        map.clear();
    }

    if (rexp.cap(2).isEmpty())
    {
        map.insert(METEO_TREND_TYPE, rexp.cap(1));
    } else
    {
        map.insert(METEO_TREND_TYPE, rexp.cap(3));
        QTime time = QTime(rexp.cap(4).toInt(), rexp.cap(5).toInt());
        map.insert(METEO_TREND_FROM, time);
    }

    return true;
}

bool MeteoParser::parseTafTrendProbability(const QString &data, QVariantMap &map)
{
    QRegExp rexp("PROB(\\d{2})");
    if (!rexp.exactMatch(data))
        return false;

    map.clear();
    map.insert(METEO_TREND_PROBABILITY, rexp.cap(1));

    return true;
}

bool MeteoParser::parseTafTrendTime(const QString &data, QVariantMap &map)
{
    QRegExp rexp("(\\d{2})(\\d{2})(/(\\d{2})(\\d{2}))?");
    if (!rexp.exactMatch(data))
        return false;

    QDateTime from;
    QDateTime till;

    if (!rexp.cap(3).isEmpty())
    {
        from = QDateTime(
                    QDate(
                        now.date().year()
                        , now.date().month()
                        , rexp.cap(1).toUInt()
                        )
                    , QTime(rexp.cap(2).toUInt() % 24, 0)
                    , Qt::UTC);
        from.addDays(rexp.cap(2).toUInt() / 24);
        till = QDateTime(
                    QDate(
                        now.date().year()
                        , now.date().month()
                        , rexp.cap(4).toUInt()
                        )
                    , QTime(rexp.cap(5).toUInt() % 24, 0)
                    , Qt::UTC);
        from.addDays(rexp.cap(5).toUInt() / 24);
    } else
    {
        from = QDateTime(
                    now.date().addDays(rexp.cap(1).toUInt() / 24)
                    , QTime(rexp.cap(1).toUInt() % 24, 0)
                    , Qt::UTC);
        till = QDateTime(
                    now.date().addDays(rexp.cap(2).toUInt() / 24)
                    , QTime(rexp.cap(2).toUInt() % 24, 0)
                    , Qt::UTC);
    }

    map.insert(METEO_TREND_FROM, from);
    map.insert(METEO_TREND_TILL, till);

    return true;
}

bool MeteoParser::parseTafMaxTemperature(const QString &data, QVariantMap &map)
{
    QRegExp rexp("TX(M)?(\\d{2})/(\\{d2})?(\\d{2})Z");

    if (!rexp.exactMatch(data))
        return false;

    int temperature = rexp.cap(2).toUInt();
    QDateTime time;

    if (!rexp.cap(1).isEmpty())
        temperature *= -1;
    if (rexp.cap(3).isEmpty())
    {
        time = QDateTime(now.date(), QTime(rexp.cap(4).toUInt(), 0), Qt::UTC);
    } else
    {
        time = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toUInt())
                         , QTime(rexp.cap(4).toUInt(), 0), Qt::UTC);
    }

    map.insert(METEO_MAX_TEMPERATURE, temperature);
    map.insert(METEO_MAX_TEMPERATURE_TIME, time);

    return true;
}

bool MeteoParser::parseTafMinTemperature(const QString &data, QVariantMap &map)
{
    QRegExp rexp("TN(M)?(\\d{2})/{\\d2}?(\\d{2})Z");

    if (!rexp.exactMatch(data))
        return false;

    int temperature = rexp.cap(2).toUInt();
    QDateTime time;

    if (!rexp.cap(1).isEmpty())
        temperature *= -1;
    if (rexp.cap(3).isEmpty())
    {
        time = QDateTime(now.date(), QTime(rexp.cap(4).toUInt(), 0), Qt::UTC);
    } else
    {
        time = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toUInt())
                         , QTime(rexp.cap(4).toUInt(), 0), Qt::UTC);
    }

    map.insert(METEO_MIN_TEMPERATURE, temperature);
    map.insert(METEO_MIN_TEMPERATURE_TIME, time);

    return true;
}

int MeteoParser::parseSigmetHeader(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "([A-Z]{4})"
                 "\\s+SIGMET"
                 "\\s+([A-Z]?\\d{1,3})"
                 "\\s+VALID\\s+(\\d{2})(\\d{2})(\\d{2})/(\\d{2})(\\d{2})(\\d{2})"
                 "\\s+([A-Z]{4})"
                 "\\s*-"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_CONTROL_ATS, rexp.cap(1));
    map.insert(METEO_MESSAGE_TYPE, TelegramType::SIGMET);
    map.insert(METEO_NUMBER, rexp.cap(2));

    QDateTime from = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toInt())
                               , QTime(rexp.cap(4).toInt() % 24, rexp.cap(5).toInt())
                               , Qt::UTC);
    from.addDays(rexp.cap(4).toInt() / 24);
    QDateTime tomorrow = from.addMonths(1);
    QDateTime yesterday = from.addMonths(-1);
    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(from.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));
    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        from = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        from = yesterday;

    QDateTime till = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(6).toInt())
                               , QTime(rexp.cap(7).toInt() % 24, rexp.cap(8).toInt())
                               , Qt::UTC);
    till.addDays(rexp.cap(7).toInt() / 24);
    while (from > till)
        till = till.addMonths(1);

    map.insert(METEO_SOURCE, rexp.cap(9));
    map.insert(METEO_VALID_FROM, from);
    map.insert(METEO_VALID_TILL, till);

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetTarget(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(([A-Z]{4})(\\s+([A-Z\\s]+))?\\s+(FIR/UIR))"
                 "\\s*\\n?\\s*");
    rexp.setMinimal(true);
    if (!rexp.indexIn(data))
    {
        map.insert(METEO_TARGET, rexp.cap(1));
        map.insert(METEO_TARGET_SECTOR, rexp.cap(2));
        map.insert(METEO_TARGET_NAME, rexp.cap(4));
        map.insert(METEO_TARGET_FIR, rexp.cap(5));

        return rexp.matchedLength();
    }

    QRegExp rexp1("^\\s*"
                 "(([A-Z]{4})(\\s+([A-Z\\s]+))?\\s+(FIR|UIR|CTA))"
                 "\\s*\\n?\\s*");
    rexp1.setMinimal(true);
    if (rexp1.indexIn(data))
        return 0;

    map.insert(METEO_TARGET, rexp1.cap(1));
    map.insert(METEO_TARGET_SECTOR, rexp1.cap(2));
    map.insert(METEO_TARGET_NAME, rexp1.cap(4));
    map.insert(METEO_TARGET_FIR, rexp1.cap(5));

    return rexp1.matchedLength();
}

int MeteoParser::parseSigmetEvent(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(((OBSC|EMBD|FRQ|SQL)\\s+(TS)(GR)?)"
                 "|((SEV)\\s+(TURB|MTW|(ICE(\\s+FZRA?))))"
                 "|((HVY)\\s+(DS|SS))"
                 "|((TC)\\s+([A-Z]+)?)"
                 "|((VA)\\s+([A-Z]+)?)"
                 "|((RDOACT)\\s+(CLD))"
                 ")"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_EVENT, rexp.cap(1));

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetObservation(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(OBS|FCST)(\\s+AT\\s+(\\d{2})(\\d{2})Z)?"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_OBSERVATION_TYPE, rexp.cap(1));
    if (!rexp.cap(2).isEmpty())
    {
        QTime time(rexp.cap(3).toInt(), rexp.cap(4).toInt());
        map.insert(METEO_OBSERVATION_TIME, time);
    }

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetLocation(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "OVER\\s+(WHOLE)?\\s+([A-Z]{4})\\s+FIR"
                 "\\s*\\n?\\s*");
    if (!rexp.indexIn(data))
    {
        if (rexp.cap(1).isEmpty())
            map.insert(METEO_LOCATION_TYPE, OVER_FIR);
        else
            map.insert(METEO_LOCATION_TYPE, OVER_WHOLE_FIR);
        map.insert(METEO_LOCATION_OVER_FIR, rexp.cap(2));
        return rexp.matchedLength();
    }

    QString tmp = data;
    QVariantList points;

    QRegExp rexp1("^\\s*"
                  "((N|S)\\s+OF\\s+((N|S)(\\d{2})(\\d{2})?))"
                  "\\s*\\n?\\s*");

    if (!rexp1.indexIn(tmp))
    {
        if (!rexp1.cap(2).isEmpty())
            map.insert(METEO_LATTITUDE_DIRECTION, rexp1.cap(2));
        QVariantMap point;
        point.insert(METEO_LATTITUDE, rexp1.cap(3));
        points << point;
        tmp.remove(0, rexp1.matchedLength());
    }

    QRegExp rexp2("^\\s*"
                  "(AND\\s+)?((W|E)\\s+OF\\s+((W|E)(\\d{3})(\\d{2})?))"
                  "\\s*\\n?\\s*");

    if (!rexp2.indexIn(tmp))
    {
        if (!rexp2.cap(2).isEmpty())
            map.insert(METEO_LONGITUDE_DIRECTION, rexp2.cap(2));
        QVariantMap point;
        point.insert(METEO_LONGITUDE, rexp2.cap(3));
        points << point;
        tmp.remove(0, rexp2.matchedLength());
    }

    QRegExp rexp3("^\\s*"
                  "((N|NE|E|SE|S|SW|W|NW)\\s+OF)|(AT)"
                  "\\s+\\n?\\s*");

    if (!rexp3.indexIn(tmp))
    {
        if (!rexp3.cap(1).isEmpty())
            map.insert(METEO_COMMON_DIRECTION, rexp3.cap(2));
        else
            map.insert(METEO_COMMON_DIRECTION, rexp3.cap(3));
        tmp.remove(0, rexp3.matchedLength());
    }

    QRegExp rexp4("^\\s*"
                  "(LINE|WI)"
                  "\\s*\\n?\\s*");

    if (!rexp4.indexIn(tmp))
    {
        if (rexp4.cap(1) == "WI")
            map.insert(METEO_LOCATION_TYPE, WITHIN);
        else
            map.insert(METEO_LOCATION_TYPE, LINE);
        tmp.remove(0, rexp4.matchedLength());
    }

    QRegExp rexp5("^\\s*"
                  "((-|TO)\\s+)?"
                  "((N|S)(\\d{2})(\\d{2})?)"
                  "\\s+((W|E)(\\d{3})(\\d{2})?)"
                  "\\s*\\n?\\s*");

    while (!rexp5.indexIn(tmp))
    {
        QVariantMap point;
        point.insert(METEO_LATTITUDE, rexp5.cap(3));
        point.insert(METEO_LONGITUDE, rexp5.cap(7));
        points << point;
        tmp.remove(0, rexp5.matchedLength());
    }

    if (!points.isEmpty())
    {
        map.insert(METEO_POINTS, points);
        if (!map.contains(METEO_LOCATION_TYPE))
            map.insert(METEO_LOCATION_TYPE, POINTS);
    }

    return data.size() - tmp.size();
}

int MeteoParser::parseSigmetFlightLevel(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(((SFC/)?FL)(\\d{3})(/(\\d{3}))?)"
                 "|((SFC/)?(\\d{4})(M|FT))"
                 "|(((TOP|ABV|TOP\\s+ABV)\\s+FL)(\\d{3}))"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    if (!rexp.cap(1).isEmpty())
    {
        map.insert(METEO_FLIGHT_LEVEL_TYPE, rexp.cap(2));
        map.insert(METEO_FLIGHT_LEVEL_FIRST, rexp.cap(4).toInt());
        if (!rexp.cap(5).isEmpty())
            map.insert(METEO_FLIGHT_LEVEL_SECOND, rexp.cap(6).toInt());
    } else if (!rexp.cap(7).isEmpty())
    {
        if (!rexp.cap(8).isEmpty())
            map.insert(METEO_FLIGHT_LEVEL_TYPE, rexp.cap(8));
        map.insert(METEO_FLIGHT_LEVEL_FIRST, rexp.cap(9).toInt());
        map.insert(METEO_FLIGHT_LEVEL_UNIT, rexp.cap(10));
    } else // !rexp.cap(11).isEmpty()
    {
        map.insert(METEO_FLIGHT_LEVEL_TYPE, rexp.cap(12));
        map.insert(METEO_FLIGHT_LEVEL_FIRST, rexp.cap(14).toInt());
    }

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetMov(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "((STNR)"
                 "|((MOV)"
                 "\\s+(N|NNE|NE|ENE|E|ESE|SE|SSE|S|SSW|SW|WSW|W|WNW|NW|NNW)"
                 "\\s+(\\d{1,2})\\s?(KMH|KT)))"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    if (!rexp.cap(2).isEmpty())
    {
        map.insert(METEO_MOVE_TYPE, rexp.cap(2));
    } else
    {
        map.insert(METEO_MOVE_TYPE, rexp.cap(4));
        map.insert(METEO_MOVE_DIRECTION, rexp.cap(5));
        map.insert(METEO_MOVE_SPEED, rexp.cap(6).toInt());
        map.insert(METEO_MOVE_UNIT, rexp.cap(7));
    }

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetIntensityChange(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(INTSF|WKN|NC)"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_INTENSITY_CHANGE, rexp.cap(1));

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetCancel(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(CNL\\s+SIGMET)"
                 "\\s+([A-Z]?\\d{1,3})"
                 "\\s+(\\d{2})(\\d{2})(\\d{2})/(\\d{2})(\\d{2})(\\d{2})"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_CANCEL, rexp.cap(1));
    map.insert(METEO_CANCEL_NUMBER, rexp.cap(2));

    QDateTime from = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toInt())
                               , QTime(rexp.cap(4).toInt() % 24, rexp.cap(5).toInt())
                               , Qt::UTC);
    from.addDays(rexp.cap(4).toInt() / 24);
    QDateTime tomorrow = from.addMonths(1);
    QDateTime yesterday = from.addMonths(-1);
    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(from.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));
    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        from = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        from = yesterday;

    QDateTime till = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(6).toInt())
                               , QTime(rexp.cap(7).toInt() % 24, rexp.cap(8).toInt())
                               , Qt::UTC);
    till.addDays(rexp.cap(7).toInt() / 24);
    while (from > till)
        till = till.addMonths(1);

    map.insert(METEO_CANCEL_VALID_FROM, from);
    map.insert(METEO_CANCEL_VALID_TILL, till);

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetVolcano(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(VA\\sERUPTION\\s)?"
                 "\\s*((MT\\s(([A-Z]|\\s)*))\\s)?"
                 "\\s*((PSN\\s(((N|S)\\d{2}\\d{2})\\s?((E|W)\\d{3}\\d{2})))\\s)?"
                 "\\s*VA\\sCLD"
                 "\\s*\\n?\\s*");
    rexp.setMinimal(true);
    if (rexp.indexIn(data))
        return 0;

    if (!rexp.cap(1).isEmpty())
        map.insert(METEO_VA_ERUPTION, rexp.cap(1));

    if (!rexp.cap(2).isEmpty())
        map.insert(METEO_MT_NAME, rexp.cap(4));

    if (!rexp.cap(6).isEmpty())
    {
        map.insert(METEO_MT_POSITION, rexp.cap(8));
        map.insert(METEO_MT_LATTITUDE, rexp.cap(9));
        map.insert(METEO_MT_LONGITUDE, rexp.cap(11));
    }

    return rexp.matchedLength();
}

int MeteoParser::parseSigmetAshCloudLocation(const QString &data, QVariantMap &map)
{
    QVariantList acl;

    QString tmp = data;

    bool stop = false;
    while (!stop)
    {
        QVariantMap locationMap;

        QVariantList points;

        QRegExp rexp("^\\s*"
                      "((N|S)(\\d{2})(\\d{2}))\\s?((E|W)(\\d{3})(\\d{2}))(\\s?\\-\\s?)?"
                      "\\s*\\n?\\s*");

        while (!rexp.indexIn(tmp))
        {
            QVariantMap point;
            point.insert(METEO_LATTITUDE, rexp.cap(1));
            point.insert(METEO_LONGITUDE, rexp.cap(5));
            points << point;
            tmp.remove(0, rexp.matchedLength());
        }

        if (!points.isEmpty())
            locationMap.insert(METEO_POINTS, points);

        QRegExp rexp1("^\\s*"
                      "FL\\s?(\\d{3})/(\\d{3})(\\s+APRX\\s(\\d{1,3})(KM|NM)\\sBY\\s(\\d{1,3})(KM|NM))?"
                      "\\s*\\n?\\s*");

        if (!rexp1.indexIn(tmp))
        {
            locationMap.insert(METEO_FLIGHT_LEVEL_FIRST, rexp1.cap(1));
            locationMap.insert(METEO_FLIGHT_LEVEL_SECOND, rexp1.cap(2));
            if (!rexp1.cap(3).isEmpty())
            {
                locationMap.insert(METEO_ASH_CLOUD_LENGTH1, rexp1.cap(4).toInt());
                locationMap.insert(METEO_ASH_CLOUD_LENGTH1TYPE, rexp1.cap(5));
                locationMap.insert(METEO_ASH_CLOUD_LENGTH2, rexp1.cap(6).toInt());
                locationMap.insert(METEO_ASH_CLOUD_LENGTH2TYPE, rexp1.cap(7));
            }
            tmp.remove(0, rexp1.matchedLength());
        } else
        {
            mErrors << ParserErrors::SigmetAshCloudLocationField;
        }

        acl.append(locationMap);

        QRegExp rexp2("^\\s*"
                      "AND"
                      "\\s*\\n?\\s*");
        if (!rexp2.indexIn(tmp))
        {
            tmp.remove(0, rexp2.matchedLength());
        } else
        {
            stop = true;
        }
    }

    map.insert(METEO_ASH_CLOUD_LOCATION, acl);

    return data.size() - tmp.size();
}

int MeteoParser::parseSigmetVaForecast(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "FCST\\s(\\d{2})(\\d{2})Z\\sVA\\sCLD\\sAPRX"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    QVariantMap vafMap;
    vafMap.insert(METEO_VA_FORECAST_TIME, QTime(rexp.cap(1).toInt(), rexp.cap(2).toInt()));

    QString tmp = data;
    tmp.remove(0, rexp.matchedLength());

    QVariantList points;

    QRegExp rexp1("^\\s*"
                  "((N|S)(\\d{2})(\\d{2}))\\s?((E|W)(\\d{3})(\\d{2}))(\\s?\\-\\s?)?"
                  "\\s*\\n?\\s*");

    while (!rexp1.indexIn(tmp))
    {
        QVariantMap point;
        point.insert(METEO_LATTITUDE, rexp1.cap(1));
        point.insert(METEO_LONGITUDE, rexp1.cap(5));
        points << point;
        tmp.remove(0, rexp1.matchedLength());
    }

    if (!points.isEmpty())
        vafMap.insert(METEO_POINTS, points);

    map.insert(METEO_VA_FORECAST, vafMap);

    return data.size() - tmp.size();
}

int MeteoParser::parseAirmetHeader(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "([A-Z]{4})"
                 "\\s+AIRMET"
                 "\\s+([A-Z]?\\d{1,3})"
                 "\\s+VALID\\s+(\\d{2})(\\d{2})(\\d{2})/(\\d{2})(\\d{2})(\\d{2})"
                 "\\s+([A-Z]{4})"
                 "\\s*-"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_CONTROL_ATS, rexp.cap(1));
    map.insert(METEO_MESSAGE_TYPE, TelegramType::AIRMET);
    map.insert(METEO_NUMBER, rexp.cap(2));

    QDateTime from = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toInt())
                               , QTime(rexp.cap(4).toInt() % 24, rexp.cap(5).toInt())
                               , Qt::UTC);
    from.addDays(rexp.cap(4).toInt() / 24);
    QDateTime tomorrow = from.addMonths(1);
    QDateTime yesterday = from.addMonths(-1);
    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(from.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));
    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        from = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        from = yesterday;

    QDateTime till = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(6).toInt())
                               , QTime(rexp.cap(7).toInt() % 24, rexp.cap(8).toInt())
                               , Qt::UTC);
    till.addDays(rexp.cap(7).toInt() / 24);
    while (from > till)
        till = till.addMonths(1);

    map.insert(METEO_SOURCE, rexp.cap(9));
    map.insert(METEO_VALID_FROM, from);
    map.insert(METEO_VALID_TILL, till);

    return rexp.matchedLength();
}

int MeteoParser::parseAirmetEvent(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "((MOD\\s(ICE|TURB|MTW))|(MT\\sOBSC)|((ISOL|OCNL)\\sTS\\s?(GR)?)|"
                 "((ISOL|OCNL|FRQ)\\s(CB|TCU)))"
                 "|(((BKN|OVC)\\sCLD)(\\s(\\d{3})/(ABV)?(\\d{3,5})(M|FT)))"
                 "|((SFC\\sWIND)\\s(\\d{2,3})(M|KT))"
                 "|((SFC\\sVIS)\\s(\\d{4})M(\\s([A-Z]+))?)"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    if (!rexp.cap(1).isEmpty())
    {
        map.insert(METEO_EVENT, rexp.cap(1));
    } else if (!rexp.cap(11).isEmpty())
    {
        map.insert(METEO_EVENT, rexp.cap(12));
        map.insert(METEO_CLOUDS_LOWER_LIMIT, rexp.cap(15).toInt());
        map.insert(METEO_CLOUDS_UPPER_LIMIT, rexp.cap(17).toInt());
        if (!rexp.cap(16).isEmpty())
            map.insert(METEO_CLOUDS_UPPER_LIMIT_IS_ABOVE, rexp.cap(16));
        map.insert(METEO_CLOUDS_LIMIT_TYPE, rexp.cap(18));
    } else if (!rexp.cap(19).isEmpty())
    {
        map.insert(METEO_EVENT, rexp.cap(20));
        map.insert(METEO_WIND_AVARAGE_SPEED, rexp.cap(21).toInt());
        map.insert(METEO_WIND_SPEED_TYPE, rexp.cap(22));
    } else if (!rexp.cap(23).isEmpty())
    {
        map.insert(METEO_EVENT, rexp.cap(24));
        map.insert(METEO_VISIBILITY, rexp.cap(25).toInt());
        if (!rexp.cap(26).isEmpty())
            map.insert(METEO_WEATHER_EVENT, rexp.cap(27));
    }

    return rexp.matchedLength();
}

int MeteoParser::parseAirmetCancel(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "(CNL\\s+AIRMET)"
                 "\\s+([A-Z]?\\d{1,3})"
                 "\\s+(\\d{2})(\\d{2})(\\d{2})/(\\d{2})(\\d{2})(\\d{2})"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_CANCEL, rexp.cap(1));
    map.insert(METEO_CANCEL_NUMBER, rexp.cap(2));

    QDateTime from = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(3).toInt())
                               , QTime(rexp.cap(4).toInt() % 24, rexp.cap(5).toInt())
                               , Qt::UTC);
    from.addDays(rexp.cap(4).toInt() / 24);
    QDateTime tomorrow = from.addMonths(1);
    QDateTime yesterday = from.addMonths(-1);
    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(from.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));
    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        from = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        from = yesterday;

    QDateTime till = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(6).toInt())
                               , QTime(rexp.cap(7).toInt() % 24, rexp.cap(8).toInt())
                               , Qt::UTC);
    till.addDays(rexp.cap(7).toInt() / 24);
    while (from > till)
        till = till.addMonths(1);

    map.insert(METEO_CANCEL_VALID_FROM, from);
    map.insert(METEO_CANCEL_VALID_TILL, till);

    return rexp.matchedLength();
}

int MeteoParser::parseGametHeader(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "([A-Z]{4})"
                 "\\s+GAMET"
                 "\\s+VALID\\s+(\\d{2})(\\d{2})(\\d{2})/(\\d{2})(\\d{2})(\\d{2})"
                 "\\s+([A-Z]{4})"
                 "\\s*-"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_CONTROL_ATS, rexp.cap(1));
    map.insert(METEO_MESSAGE_TYPE, TelegramType::GAMET);

    QDateTime from = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(2).toInt())
                               , QTime(rexp.cap(3).toInt() % 24, rexp.cap(4).toInt())
                               , Qt::UTC);
    from.addDays(rexp.cap(3).toInt() / 24);
    QDateTime tomorrow = from.addMonths(1);
    QDateTime yesterday = from.addMonths(-1);
    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(from.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));
    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        from = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        from = yesterday;

    QDateTime till = QDateTime(QDate(now.date().year(), now.date().month(), rexp.cap(5).toInt())
                               , QTime(rexp.cap(6).toInt() % 24, rexp.cap(7).toInt())
                               , Qt::UTC);
    till.addDays(rexp.cap(7).toInt() / 24);
    while (from > till)
        till = till.addMonths(1);

    map.insert(METEO_SOURCE, rexp.cap(8));
    map.insert(METEO_VALID_FROM, from);
    map.insert(METEO_VALID_TILL, till);

    return rexp.matchedLength();
}

int MeteoParser::parseGametFlightLevel(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "BLW\\sFL(\\d{3})"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_FLIGHT_LEVEL, rexp.cap(1).toInt());

    return rexp.matchedLength();
}

int MeteoParser::parseGametBody(const QString &data, QVariantMap &map)
{
    QRegExp rexp("^\\s*"
                 "SECN\\sI\\s*\\n?\\s*(.*)"
                 "\\s+\\n?SECN\\sII\\s*\\n?\\s*(.*)"
                 "\\s*\\n?\\s*");
    if (rexp.indexIn(data))
        return 0;

    map.insert(METEO_GAMET_SECTION1, rexp.cap(1));
    map.insert(METEO_GAMET_SECTION2, rexp.cap(2));

    return rexp.matchedLength();
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
