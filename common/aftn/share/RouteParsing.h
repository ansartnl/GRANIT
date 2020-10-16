#ifndef ROUTEPARSING_H
#define ROUTEPARSING_H

#include "ParseVersion.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

struct RouteItem
{
    enum Type {
        T_NONE = 0
        , T_DCT
        , T_ROUTE_SID_STAR  // Can't separate those
        , T_POINT
        , T_SPEED_FLIGHT_LEVEL
        , T_IFR
        , T_VFR
    } type;
    QString name;
    QString speedFl;

    RouteItem() :
        type(T_NONE)
    {}

    RouteItem(Type type_) :
        type(type_)
    {}

    static const RouteItem dct()
    {
        return RouteItem(T_DCT);
    }

    static const RouteItem route(const QString &name)
    {
        RouteItem ret(T_ROUTE_SID_STAR);
        ret.name = name;
        return ret;
    }

    static const RouteItem point(const QString &name, const QString &speedFl = QString())
    {
        RouteItem ret(T_POINT);
        ret.name = name;
        ret.speedFl = speedFl;
        return ret;
    }

    static const RouteItem speedFlightLevel(const QString &speedFl)
    {
        RouteItem ret(T_SPEED_FLIGHT_LEVEL);
        ret.speedFl = speedFl;
        return ret;
    }

    static const RouteItem ifr()
    {
        return RouteItem(T_IFR);
    }

    static const RouteItem vfr()
    {
        return RouteItem(T_VFR);
    }

    const QString data(ParseVersion::Version v) const;
};

typedef QList < RouteItem > Route;
const QString routeToString(const Route &route, ParseVersion::Version v);

const Route parseRoute(const QString &route);

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ROUTEPARSING_H
