#ifndef _SUPERVISOR_PROTO_H__
#define _SUPERVISOR_PROTO_H__

#include "SpCommands.h"
#include "SpLog.h"
#include "SpRequest.h"
#include "SpStatus.h"
#include "SpService.h"
#include "SpGroup.h"
#include "SpRemoteOperations.h"
#include "SpRemoteLog.h"

// Current version for Supervisor protocol communication
#define SP_PROTO_VERSION  0x00010009


//! Intermediate data structures used to do interactions between client-server applications.
namespace SupervisorProto
{
    //! Retrieve unpacked (readable) protocol version
    QString version();
}

#endif  // _SUPERVISOR_PROTO_H__
