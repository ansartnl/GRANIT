#include "SpProto.h"

//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    static int registerHelper[] = {
        qRegisterMetaTypeStreamOperators<Request::Request>(),
        qRegisterMetaTypeStreamOperators<RequestStatus>(),
        qRegisterMetaTypeStreamOperators<RequestSpecialStatus>(),
        qRegisterMetaTypeStreamOperators<RequestLog>(),
        qRegisterMetaTypeStreamOperators<RequestRemoteOperations>(),
        qRegisterMetaTypeStreamOperators<RequestRemoteLogs>(),
        qRegisterMetaTypeStreamOperators<Status>(),
        qRegisterMetaTypeStreamOperators<Service>(),
        qRegisterMetaTypeStreamOperators<Services>(),
        qRegisterMetaTypeStreamOperators<Groups>(),
        qRegisterMetaTypeStreamOperators<ActiveGroup>(),
        qRegisterMetaTypeStreamOperators<ActivateGroup>(),
        qRegisterMetaTypeStreamOperators<ActiveGroupChanged>(),
        qRegisterMetaTypeStreamOperators<Log>(),
        qRegisterMetaTypeStreamOperators<FMTPStatus>(),
        qRegisterMetaTypeStreamOperators<ADPStatus>(),
        qRegisterMetaTypeStreamOperators<Restart>(),
        qRegisterMetaTypeStreamOperators<NetList>(),
        qRegisterMetaTypeStreamOperators<CWPSectors>(),
        qRegisterMetaTypeStreamOperators<StopAll>(),
        qRegisterMetaTypeStreamOperators<StartAll>(),
        qRegisterMetaTypeStreamOperators<FDPStatus>(),
        qRegisterMetaTypeStreamOperators<ActivateFdp>(),
        qRegisterMetaTypeStreamOperators<StartScript>(),
        qRegisterMetaTypeStreamOperators<StopScript>(),
        qRegisterMetaTypeStreamOperators<ScriptList>(),
        qRegisterMetaTypeStreamOperators<ScriptString>(),
        qRegisterMetaTypeStreamOperators<ScriptFinished>(),
        qRegisterMetaTypeStreamOperators<CWPUser>(),
        qRegisterMetaTypeStreamOperators<Shutdown>(),
        qRegisterMetaTypeStreamOperators<Reboot>(),
        qRegisterMetaTypeStreamOperators<RemoteOperations>(),
        qRegisterMetaTypeStreamOperators<RemoteLog>(),
        qRegisterMetaTypeStreamOperators<AvailableLogs>(),
        qRegisterMetaTypeStreamOperators<FolderItems>(),
        qRegisterMetaTypeStreamOperators<SnmpVariables>(),
        qRegisterMetaTypeStreamOperators<SetSnmpVariable>()
    };

    QString version()
    {
        quint32 major = (SP_PROTO_VERSION >> 16) & 0xFFFF,
                minor = SP_PROTO_VERSION & 0xFFFF;

        return QString("%1.%2").arg(major).arg(minor);
    }

    QString toString(EType type)
    {
        switch (type)
        {
        case CWP:
            return "CWP";
        case FMTP:
            return "FMTP";
        case FDP:
            return "FDP";
        case ADP:
            return "ADP";
        case STCA:
            return "STCA";
        case AFTNWS:
            return "AFTNWS";
        case DBSYNC:
            return "DBSYNC";
        case PROCESS:
            return "Process";
        case SERVICE:
            return "Service";
        case Server:
            return "Server";
        case LOG:
            return "Log";
        }
        return QObject::tr("Unknown");
    }

    QString toString(EFMTPStatus status)
    {
        switch (status)
        {
        case enDisable:
            return QObject::tr("Disabled");
        case enIDLE:
            return QObject::tr("Idle");
        case enSystemIdPending:
            return QObject::tr("System idle pending");
        case enIdPending:
            return QObject::tr("Id pending");
        case enReady:
            return QObject::tr("Ready");
        case enAssociationPending:
            return QObject::tr("Association pending");
        case enDataReady:
            return QObject::tr("Data ready");
        }
        return QObject::tr("Unknown");
    }

    QString fdpStatus(bool status)
    {
        if (status)
            return QObject::tr("Master");
        return QObject::tr("Slave");
    }

}
