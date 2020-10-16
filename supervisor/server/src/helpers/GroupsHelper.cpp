#include "GroupsHelper.h"

#include "init.h"

#include <QtCore/QMap>


namespace GroupsHelper
{

// Start process/service order map (which order process/service should be started).
// first - order value, second process/service identifier
typedef QMap<int, QString>  TStartOrderMap;

struct Group
{
    QString         visualName;     //!< Visual name of a group.
    TStartOrderMap  services;       //!< Processes or services - groupped in start orders
};

typedef QMap<QString, Group>    TGroupMap;

TGroupMap g_groups;
// Active group name.
QString g_activeGroup;


void createGroup(const QString &name, const QString &visualName)
{
    if ( g_groups.contains(name) )
        return;

    Group group;
    group.visualName = visualName;

    g_groups[name] = group;
}

void addService(const QString &groupName, const QString &name, int order)
{
    if ( g_groups.contains(groupName) )
    {
        Group &group = g_groups[groupName];

        // Only unique processes can be started.
        if ( group.services.values().count(name) == 0 )
            group.services[order] = name;
    }
}

QStringList groups()
{
    return g_groups.keys();
}

bool contains(const QString &name)
{
    return g_groups.contains(name);
}

TGroups groupsInformation()
{
    TGroups groupsInfo;

    for ( TGroupMap::const_iterator it = g_groups.constBegin(); it != g_groups.constEnd(); ++it )
    {
        const QString &groupName = it.key();
        const Group &group = *it;

        GroupInfo gi;
        gi.name = groupName;
        gi.visualName = group.visualName;

        groupsInfo << gi;
    }

    return groupsInfo;
}

QStringList orderedServices(const QString &name)
{
    if ( g_groups.contains(name) )
        return g_groups[name].services.values();
    return QStringList();
}

QStringList orderedServices()
{
    return orderedServices( activeGroup() );
}

void setActiveGroup(const QString &name)
{
    g_activeGroup = name;

    // Save new active group in the settings.
    xmlSettings()->setValue("active_group", name);
}

QString activeGroup()
{
    return g_activeGroup;
}

}
