#ifndef _SUPERVISOR_PROTO_LOG_H__
#define _SUPERVISOR_PROTO_LOG_H__

#include "SpCommon.h"

#include <QtCore/QStringList>
#include <QtCore/QVariant>


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Status information structure
    struct Log
    {
        EType       type;
        QString     name;
        QStringList log;
    };
    inline QDataStream &operator<<(QDataStream &stream, const Log &val)
    {
        stream << val.type << val.name << val.log;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Log &val)
    {
        stream >> val.type >> val.name >> val.log;
        return stream;
    }

    //! List of available logs
    struct AvailableLogs
    {
        QStringList names;
    };
    inline QDataStream &operator<<(QDataStream &stream, const AvailableLogs &val)
    {
        stream << val.names;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, AvailableLogs &val)
    {
        stream >> val.names;
        return stream;
    }
}

Q_DECLARE_METATYPE(SupervisorProto::Log)
Q_DECLARE_METATYPE(SupervisorProto::AvailableLogs)

#endif  // _SUPERVISOR_PROTO_LOG_H__
