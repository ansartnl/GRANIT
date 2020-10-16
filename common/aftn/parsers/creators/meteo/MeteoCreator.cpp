#include "MeteoCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "MeteoMessage.h"

#include "MeteoParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

MeteoCreator::MeteoCreator()
{
    addType(TelegramType::METAR);
    addType(TelegramType::SPECI);
    addType(TelegramType::TAF);
    addType(TelegramType::SIGMET);
    addType(TelegramType::AIRMET);
    addType(TelegramType::GAMET);
}

AftnMessage * MeteoCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    MeteoStruct m;
    m.wmoDataType = map.value(METEO_WMO_DATA_TYPE).toString();
    m.type = type;
    m.arrivalTime = map.value(METEO_ARRIVAL_TIME).toDateTime();
    m.zone = map.value(METEO_ZONE).toString();
    m.text = map.value(METEO_MESSAGE_TEXT).toString();

    m.isCnl = map.contains(METEO_CANCEL);
    if (m.isCnl)
    {
        m.validFrom = map.value(METEO_CANCEL_VALID_FROM).toDateTime();
        m.validTill = map.value(METEO_CANCEL_VALID_TILL).toDateTime();
        m.number = map.value(METEO_CANCEL_NUMBER).toString();
    } else
    {
        m.validFrom = map.value(METEO_VALID_FROM).toDateTime();
        if (!m.validFrom.isValid())
            m.validFrom = map.value(METEO_TIME).toDateTime();
        m.validTill = map.value(METEO_VALID_TILL).toDateTime();
        if (!m.validTill.isValid())
            m.validTill = m.validFrom.addSecs(60 * 30); // TODO: заменить на параметр
        m.number = map.value(METEO_NUMBER).toString();
    }

    return new MeteoMessage(m);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
