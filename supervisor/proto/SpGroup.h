#ifndef _SUPERVISOR_PROTO_GROUP_H__
#define _SUPERVISOR_PROTO_GROUP_H__

#include "SpCommon.h"

#include <QtCore/QString>
#include <QtCore/QVariant>


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Group information structure
    struct Group
    {
        QString     name;
        QString     visualName;
    };
    inline QDataStream &operator<<(QDataStream &stream, const Group &val)
    {
        stream << val.name << val.visualName;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Group &val)
    {
        stream >> val.name >> val.visualName;
        return stream;
    }


    //! Declaration of groups list.
    typedef QList<Group>        Groups;


    //! Active group information structure
    struct ActiveGroup
    {
        QString     name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ActiveGroup &val)
    {
        stream << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ActiveGroup &val)
    {
        stream >> val.name;
        return stream;
    }


    //! Activate specified group information structure
    struct ActivateGroup
    {
        QString     name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ActivateGroup &val)
    {
        stream << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ActivateGroup &val)
    {
        stream >> val.name;
        return stream;
    }


    //! Active group was changed structure
    struct ActiveGroupChanged
    {
        QString     name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ActiveGroupChanged &val)
    {
        stream << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ActiveGroupChanged &val)
    {
        stream >> val.name;
        return stream;
    }
}

Q_DECLARE_METATYPE(SupervisorProto::Groups)
Q_DECLARE_METATYPE(SupervisorProto::ActiveGroup)
Q_DECLARE_METATYPE(SupervisorProto::ActivateGroup)
Q_DECLARE_METATYPE(SupervisorProto::ActiveGroupChanged)

#endif  // _SUPERVISOR_PROTO_GROUP_H__
