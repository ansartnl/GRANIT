#ifndef _COMMON_H__
#define _COMMON_H__

#include <QtCore/QString>

enum Status { Started, Stopped };
enum Type { CWP, FMTP, FDP, ADP, STCA, AFTNWS, DBSYNC, PROCESS, SERVICE };
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

QString getConnectionDbName();

QString toString(Type type);

#endif // _COMMON_H__
