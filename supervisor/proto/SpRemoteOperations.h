#ifndef SPREMOTEOPERATIONS_H
#define SPREMOTEOPERATIONS_H

#include "SpCommon.h"

#include <QtCore/QStringList>
#include <QtCore/QVariant>


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Remote Operations information structure
    struct RemoteOperations
    {
        QStringList names;
        QStringList log;
    };
    inline QDataStream &operator<<(QDataStream &stream, const RemoteOperations &val)
    {
        stream << val.names << val.log;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, RemoteOperations &val)
    {
        stream >> val.names >> val.log;
        return stream;
    }
}

Q_DECLARE_METATYPE(SupervisorProto::RemoteOperations)


#endif // SPREMOTEOPERATIONS_H
