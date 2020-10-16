#include "NotamCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "NotamMessage.h"

#include "aftnmessageprocessor.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

NotamCreator::NotamCreator()
{
    addType(TelegramType::NOTAM);
    addType(TelegramType::NOTAM1);
}

AftnMessage * NotamCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    NotamStruct notam;
    notam.version = aftnParameters->supportedTypes()->version(type);

    notam.series = map.value(AFTN_MESSAGE_NOTAM_SERIES).toString();
    notam.number = map.value(AFTN_MESSAGE_NOTAM_NUMBER).toInt();
    notam.year = map.value(AFTN_MESSAGE_NOTAM_YEAR).toInt();

    notam.type = map.value(AFTN_MESSAGE_NOTAM_SUFFIX).toString();

    notam.replaceSeries = map.value(AFTN_MESSAGE_NOTAM_REPLACE_SERIES).toString();
    notam.replaceNumber = map.value(AFTN_MESSAGE_NOTAM_REPLACE_NUMBER).toInt();
    notam.replaceYear = map.value(AFTN_MESSAGE_NOTAM_REPLACE_YEAR).toInt();

    notam.fir = map.value(AFTN_MESSAGE_NOTAM_FIR).toString();
    notam.subject = map.value(AFTN_MESSAGE_NOTAM_SUBJECT).toString();
    notam.condition = map.value(AFTN_MESSAGE_NOTAM_CONDITION).toString();
    notam.traffic = map.value(AFTN_MESSAGE_NOTAM_TRAFFIC).toString();
    notam.purpose = map.value(AFTN_MESSAGE_NOTAM_PURPOSE).toString();
    notam.scope = map.value(AFTN_MESSAGE_NOTAM_SCOPE).toString();
    notam.qLowerLimit = map.value(AFTN_MESSAGE_NOTAM_Q_LOWER_LIMIT).toString();
    notam.qUpperLimit = map.value(AFTN_MESSAGE_NOTAM_Q_UPPER_LIMIT).toString();
    notam.location = map.value(AFTN_MESSAGE_NOTAM_LOCATION).toString();
    notam.radius = map.value(AFTN_MESSAGE_NOTAM_RADIUS).toInt();

    notam.aerodrome = map.value(AFTN_MESSAGE_NOTAM_AERODROME).toString();
    notam.validFrom = map.value(AFTN_MESSAGE_NOTAM_VALID_FROM).toDateTime();
    notam.validTill = map.value(AFTN_MESSAGE_NOTAM_VALID_UNTIL).toDateTime();
    notam.validType = map.value(AFTN_MESSAGE_NOTAM_VALID_UNTIL_TYPE).toString();

    notam.schedule = map.value(AFTN_MESSAGE_NOTAM_SCHEDULE).toString();
    notam.description = map.value(AFTN_MESSAGE_NOTAM_DESCRIPTION).toString();

    notam.lowerLimit = map.value(AFTN_MESSAGE_NOTAM_LOWER_LIMIT).toString();
    notam.upperLimit = map.value(AFTN_MESSAGE_NOTAM_UPPER_LIMIT).toString();

    return new NotamMessage(notam);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
