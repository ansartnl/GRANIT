#ifndef _GROUPS_HELPER_H__
#define _GROUPS_HELPER_H__

#include <QtCore/QString>
#include <QtCore/QStringList>


//! Namespace for storing database connection parameters.
namespace GroupsHelper
{
    struct GroupInfo
    {
        QString name;
        QString visualName;
    };

    typedef QList<GroupInfo>    TGroups;

    //! Create group with specified name and visual name.
    void createGroup(const QString &name, const QString &visualName);

    //! Add process/service to group be order.
    void addService(const QString &groupName, const QString &name, int order);

    //! Retrieve list of available group names.
    QStringList groups();

    //! True if group name is in the list.
    bool contains(const QString &name);

    //! Retrieve list of available group information.
    TGroups groupsInformation();

    //! Set active group name.
    void setActiveGroup(const QString &name);

    //! Retrieve active group name.
    QString activeGroup();

    //! Retrieve list of names of ordered process/services for specified group.
    /*!
     *  \param name Group name
    */
    QStringList orderedServices(const QString &name);

    //! Retrieve list of names of ordered process/services for active group.
    QStringList orderedServices();
}

#endif // _GROUPS_HELPER_H__
