#ifndef _SUPERVISOR_PROTO_SERVICE_H__
#define _SUPERVISOR_PROTO_SERVICE_H__

#include "SpCommon.h"

#include <QtCore/QString>
#include <QtCore/QVariant>


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Status information structure
    struct Service
    {
        EType       type;
        QString     name;
        QString     description;
        EStatus     status;
        QString     visualName;
    };
    inline QDataStream &operator<<(QDataStream &stream, const Service &val)
    {
        stream << val.type << val.name << val.description << val.status << val.visualName;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Service &val)
    {
        stream >> val.type >> val.name >> val.description >> val.status >> val.visualName;
        return stream;
    }

    //! Declaration of services list.
    typedef QList<Service>      Services;
}

Q_DECLARE_METATYPE(SupervisorProto::Service)

Q_DECLARE_METATYPE(SupervisorProto::Services)

#endif  // _SUPERVISOR_PROTO_SERVICE_H__
