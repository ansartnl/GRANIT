#include "NonStandardRvmCreator.h"

#include "AftnParameters.h"
#include "SupportedTypes.h"
#include "TelegramTypes.h"

#include "NonStandardRvmMessage.h"

#include "RestrictionParser.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

NonStandardRvmCreator::NonStandardRvmCreator()
{
    addType(TelegramType::RVM, TypeKey::kNonstandardDisambiguation);
    addType(TelegramType::RVM1, TypeKey::kNonstandardDisambiguation);
}

AftnMessage * NonStandardRvmCreator::pCreate(const QString &type, const QVariantMap &map) const
{
    NonStandardRvmStruct rvm;

    rvm.version = aftnParameters->supportedTypes()->version(type);

    rvm.id = Transliteration::toVersion(map.value(MESSAGE_ID_RESTRICTION).toString(), rvm.version);
    rvm.type =  Transliteration::toVersion(map.value(TYPE_RESTRICTION).toString(), rvm.version);
    rvm.beginTime = QDateTime::fromString(map.value(TIME_RESTRICTION_BEGIN).toString(), "yyMMddhhmm").addYears(100);
    rvm.endTime = QDateTime::fromString(map.value(TIME_RESTRICTION_END).toString(), "yyMMddhhmm").addYears(100);
    rvm.form = Transliteration::toVersion(map.value(FORM_RESTRICTION).toString(), rvm.version);
    rvm.upperLimit = Transliteration::toVersion(map.value(UPPER_LIMIT_RESTRICTION).toString(), rvm.version);
    rvm.lowerLimit = Transliteration::toVersion(map.value(LOWER_LIMIT_RESTRICTION).toString(), rvm.version);

    if (map.contains(CENTER_CIRCLE_COORDINATE))
        rvm.centerCircleCoordinate = Transliteration::toVersion(map.value(CENTER_CIRCLE_COORDINATE).toString(), rvm.version);
    if (map.contains(CENTER_CIRCLE_NAME))
        rvm.centerCircleName = Transliteration::toVersion(map.value(CENTER_CIRCLE_NAME).toString(), rvm.version);

    rvm.radius = map.value(RADIUS).toInt();

    if (map.contains(POINTS_COORDINATE))
    {
        int i = 0;
        foreach (const QString &arg, map.value(POINTS_COORDINATE).toString().split(" "))
        {
            rvm.polygonPointsCoordinate.insert(i, Transliteration::toVersion(arg, rvm.version));
            ++i;
        }
    }
    if (map.contains(POINTS_NAME))
    {
        int i = 0;
        foreach (const QString &arg, map.value(POINTS_NAME).toString().split(" "))
        {
            rvm.polygonPointsName.insert(i, Transliteration::toVersion(arg, rvm.version));
            ++i;
        }
    }

    rvm.name = Transliteration::toVersion(map.value(NAME).toString(), rvm.version);

    return new NonStandardRvmMessage(rvm);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
