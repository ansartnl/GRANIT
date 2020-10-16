#ifndef _SUPERVISOR_PROTO_COMMANDS_H__
#define _SUPERVISOR_PROTO_COMMANDS_H__

#include "SpCommon.h"

#include <QtCore/QString>
#include <QtCore/QVariant>

//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Command for server to restart specified service
    struct Restart
    {
        EType       type;
        QString     name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const Restart &val)
    {
        stream << val.type << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Restart &val)
    {
        stream >> val.type >> val.name;
        return stream;
    }
    //! Command for stopping all services and programs
    struct StopAll
    {
    };
    inline QDataStream &operator<<(QDataStream &stream, const StopAll &)
    {
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, StopAll &)
    {
        return stream;
    }
    //! Command for starting all services and programs
    struct StartAll
    {
    };
    inline QDataStream &operator<<(QDataStream &stream, const StartAll &)
    {
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, StartAll &)
    {
        return stream;
    }
    //! Command for activating fdp
    struct ActivateFdp
    {
        QString name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ActivateFdp &val)
    {
        stream << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ActivateFdp &val)
    {
        stream >> val.name;
        return stream;
    }
    //! Command for starting script
    struct StartScript
    {
        QString name;
        QString parameter;
    };
    inline QDataStream &operator<<(QDataStream &stream, const StartScript &val)
    {
        stream << val.name << val.parameter;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, StartScript &val)
    {
        stream >> val.name >> val.parameter;
        return stream;
    }
    //! Command for stopping script
    struct StopScript
    {
        QString name;
    };
    inline QDataStream &operator<<(QDataStream &stream, const StopScript &val)
    {
        stream << val.name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, StopScript &val)
    {
        stream >> val.name;
        return stream;
    }
    //! Command for shutting down computer
    struct Shutdown
    {
    };
    inline QDataStream &operator<<(QDataStream &stream, const Shutdown &)
    {
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Shutdown &)
    {
        return stream;
    }
    //! Command for rebooting computer
    struct Reboot
    {
    };
    inline QDataStream &operator<<(QDataStream &stream, const Reboot &)
    {
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Reboot &)
    {
        return stream;
    }

    //! Command for change SNMP variable
    struct SetSnmpVariable
    {
        QString agent;
        QString variable;
        QString value;
    };
    inline QDataStream &operator<<(QDataStream &stream, const SetSnmpVariable &val)
    {
        stream << val.agent << val.variable << val.value;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, SetSnmpVariable &val)
    {
        stream >> val.agent >> val.variable >> val.value;
        return stream;
    }
}

Q_DECLARE_METATYPE(SupervisorProto::Restart)
Q_DECLARE_METATYPE(SupervisorProto::StopAll)
Q_DECLARE_METATYPE(SupervisorProto::StartAll)
Q_DECLARE_METATYPE(SupervisorProto::ActivateFdp)
Q_DECLARE_METATYPE(SupervisorProto::StartScript)
Q_DECLARE_METATYPE(SupervisorProto::StopScript)
Q_DECLARE_METATYPE(SupervisorProto::Shutdown)
Q_DECLARE_METATYPE(SupervisorProto::Reboot)
Q_DECLARE_METATYPE(SupervisorProto::SetSnmpVariable)

#endif // _SUPERVISOR_PROTO_COMMANDS_H__
