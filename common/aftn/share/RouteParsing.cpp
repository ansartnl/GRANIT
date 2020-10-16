#include "RouteParsing.h"

#include "Transliteration.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

const QString RouteItem::data(ParseVersion::Version v) const
{
    switch (type) {
    case RouteItem::T_NONE:
        return QString();
    case RouteItem::T_DCT:
        return Transliteration::routeToVersion("DCT", v);
    case RouteItem::T_ROUTE_SID_STAR:
        return name;
    case RouteItem::T_POINT:
        if (speedFl.isEmpty())
            return Transliteration::routeToVersion(name, v);
        else
            return Transliteration::routeToVersion(name + "/" + speedFl, v);
    case RouteItem::T_SPEED_FLIGHT_LEVEL:
        return Transliteration::routeToVersion(speedFl, v);
    case RouteItem::T_IFR:
        return Transliteration::routeToVersion("IFR", v);
    case RouteItem::T_VFR:
        return Transliteration::routeToVersion("VFR", v);
    }
    return QString();
}

const QString routeToString(const Route &route, ParseVersion::Version v)
{
    QStringList ret;

    foreach (const RouteItem &item, route) {
        const QString s = item.data(v);
        if (!s.isEmpty())
            ret << s;
    }

    return ret.join(" ");
}

const Route parseRoute(const QString &route)
{
    Route ret;

    QRegExp speedFlRexp(QString::fromUtf8(
                            "([KNКН]\\d{4}|[MМ]\\d{3}|VFR|ЖФР|/RA/|/РА/)"
                            "([FAФА]\\d{3}|[SMСМ]\\d{4}|VFR|ЖФР|ПВП)"
                            ));

    QRegExp pointRexp(QString::fromUtf8(
                          "("
                          "((\\d{2}(\\d{2})?[NSСЮ]\\d{3}(\\d{2})?[WEЗВ]))"
                          "|((\\d{2}(\\d{2})?[NSСЮ]\\s\\d{3}(\\d{2})?[WEЗВ]))"
                          "|([A-ZА-Я]{2,5})"
                          ")"
                          "("
                          "/(\\w+)"
                          ")?"
                          ));

    const QStringList list = route.simplified().split(" ", QString::SkipEmptyParts);
    foreach (const QString &s, list) {
        if (s == "DCT" || s == QString::fromUtf8("ДЦТ")) {
            ret << RouteItem::dct();
        } else if (s == "IFR" || s == QString::fromUtf8("ИФР")) {
            ret << RouteItem::ifr();
        } else if (s == "VFR" || s == QString::fromUtf8("ЖФР")) {
            ret << RouteItem::vfr();
        } else if (speedFlRexp.exactMatch(s)) {
            ret << RouteItem::speedFlightLevel(s);
        } else if (pointRexp.exactMatch(s)) {
            ret << RouteItem::point(pointRexp.cap(1), pointRexp.cap(12));
        } else {
            ret << RouteItem::route(s);
        }
    }

    return ret;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
