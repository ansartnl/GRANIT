#ifndef _SUPERVISOR_PROTO_COMMON_H__
#define _SUPERVISOR_PROTO_COMMON_H__

#include <QtCore/QDataStream>

#define DECLARE_ENUM_STREAMING(x) \
    inline QDataStream &operator<<(QDataStream &stream, const x &val) \
    { \
        stream << qint32(val); \
        return stream; \
    } \
    inline QDataStream &operator>>(QDataStream &stream, x &val) \
    { \
        qint32 temp; \
        stream >> temp; \
        val = x(temp); \
        return stream; \
    }

//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    enum EStatus
    {
        Started,
        Stopped
    };
    enum EType
    {
        CWP,
        FMTP,
        FDP,
        ADP,
        STCA,
        AFTNWS,
        DBSYNC,
        PROCESS,
        SERVICE,
        Server,
        LOG
    };
    QString toString(EType type);
    enum EFMTPStatus
    {
        enDisable = -1,    //!< disable state
        enIDLE = 0,     //!< IDLE state
        enSystemIdPending,   //!< system id pending
        enIdPending,    //!< id pending
        enReady,     //!< ready state
        enAssociationPending,  //!< association pending state
        enDataReady     //!< data ready state
    };
    QString toString(EFMTPStatus status);

    DECLARE_ENUM_STREAMING(EStatus)
    DECLARE_ENUM_STREAMING(EType)
    DECLARE_ENUM_STREAMING(EFMTPStatus)
}

#endif // _SUPERVISOR_PROTO_COMMON_H__
