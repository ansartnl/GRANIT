#ifndef _SUPERVISOR_PROTO_STATUS_H__
#define _SUPERVISOR_PROTO_STATUS_H__

#include "SpCommon.h"

#include <QtCore/QStringList>
#include <QtCore/QVariant>


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Status information structure
    struct Status
    {
        EType       type;
        QString     name;
        EStatus     status;
    };
    inline QDataStream &operator<<(QDataStream &stream, const Status &val)
    {
        stream << val.type << val.name << val.status;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Status &val)
    {
        stream >> val.type >> val.name >> val.status;
        return stream;
    }

    //! FMTP status information structure
    struct FMTPStatus
    {
        QString     name;
        EFMTPStatus status;
    };
    inline QDataStream &operator<<(QDataStream &stream, const FMTPStatus &val)
    {
        stream << val.name << val.status;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, FMTPStatus &val)
    {
        stream >> val.name >> val.status;
        return stream;
    }

    //! CWP sectors information structure
    struct CWPSectors
    {
        QString     name;
        QString     client;
        QStringList sectors;
    };
    inline QDataStream &operator<<(QDataStream &stream, const CWPSectors &val)
    {
        stream << val.name << val.client << val.sectors;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, CWPSectors &val)
    {
        stream >> val.name >> val.client >> val.sectors;
        return stream;
    }

    //! CWP user information structure
    struct CWPUser
    {
        QString     name;
        QString     user;
    };
    inline QDataStream &operator<<(QDataStream &stream, const CWPUser &val)
    {
        stream << val.name << val.user;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, CWPUser &val)
    {
        stream >> val.name >> val.user;
        return stream;
    }

    //! List of available networks
    struct NetList
    {
        QStringList list;
    };
    inline QDataStream &operator<<(QDataStream &stream, const NetList &val)
    {
        stream << val.list;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, NetList &val)
    {
        stream >> val.list;
        return stream;
    }

    //! FDP status information structure
    struct FDPStatus
    {
        QString     name;
        bool        status;
    };
    inline QDataStream &operator<<(QDataStream &stream, const FDPStatus &val)
    {
        stream << val.name << val.status;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, FDPStatus &val)
    {
        stream >> val.name >> val.status;
        return stream;
    }
    QString fdpStatus(bool status);

    //! ADP status information structure
    struct ADPStatus
    {
        QString     name;
        QString     status;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ADPStatus &val)
    {
        stream << val.name << val.status;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ADPStatus &val)
    {
        stream >> val.name >> val.status;
        return stream;
    }

    //! List of available scripts
    struct ScriptList
    {
        QStringList scripts;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ScriptList &val)
    {
        stream << val.scripts;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ScriptList &val)
    {
        stream >> val.scripts;
        return stream;
    }

    //! Another string, sent by script
    struct ScriptString
    {
        QString str;
    };
    inline QDataStream &operator<<(QDataStream &stream, const ScriptString &val)
    {
        stream << val.str;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ScriptString &val)
    {
        stream >> val.str;
        return stream;
    }

    //! Script finished state, sent by script
    struct ScriptFinished
    {
        int exitCode;       //!< Exit code of the QProcess
        int exitStatus;     //!< Packed value from QProcess::ExitStatus enum
    };
    inline QDataStream &operator<<(QDataStream &stream, const ScriptFinished &val)
    {
        stream << val.exitCode << val.exitStatus;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, ScriptFinished &val)
    {
        stream >> val.exitCode >> val.exitStatus;
        return stream;
    }

    //! Tracked folder items, tracked by FolderController
    struct FolderItems
    {
        QList<QPair<QString, QByteArray>> items;  //!< List of file name/content
    };
    inline QDataStream &operator<<(QDataStream &stream, const FolderItems &val)
    {
        stream << val.items;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, FolderItems &val)
    {
        stream >> val.items;
        return stream;
    }

    struct SnmpVariables
    {
        QString agent;
        QVariantMap variables;
    };
    inline QDataStream &operator<<(QDataStream &stream, const SnmpVariables &val)
    {
        stream << val.agent << val.variables;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, SnmpVariables &val)
    {
        stream >> val.agent >> val.variables;
        return stream;
    }
}

Q_DECLARE_METATYPE(SupervisorProto::Status)
Q_DECLARE_METATYPE(SupervisorProto::FMTPStatus)
Q_DECLARE_METATYPE(SupervisorProto::CWPSectors)
Q_DECLARE_METATYPE(SupervisorProto::NetList)
Q_DECLARE_METATYPE(SupervisorProto::FDPStatus)
Q_DECLARE_METATYPE(SupervisorProto::ADPStatus)
Q_DECLARE_METATYPE(SupervisorProto::ScriptList)
Q_DECLARE_METATYPE(SupervisorProto::ScriptString)
Q_DECLARE_METATYPE(SupervisorProto::ScriptFinished)
Q_DECLARE_METATYPE(SupervisorProto::CWPUser)
Q_DECLARE_METATYPE(SupervisorProto::FolderItems)
Q_DECLARE_METATYPE(SupervisorProto::SnmpVariables)

#endif  // _SUPERVISOR_PROTO_STATUS_H__
