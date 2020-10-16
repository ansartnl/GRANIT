#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

#include <QtCore/QString>

namespace xml
{

const QString kLog = "Log";
const QString kPath = "Path";
const QString kLevel = "Level";

const QString kClients = "Clients";

const QString kClientTimerMs = "ClientTimerMs";
const QString kManagerLoopTimerMs = "ManagerLoopTimerMs";
const QString kPingOid = "PingOid";

const QString kTraps = "Traps";
const QString kCommunity = "Community";
const QString kReceivers = "Receivers";

} // namespace xml

namespace conf
{

const QString kName = "Name";
const QString kIdentifier = "Identifier";

const QString kProgramState = "ProgramState";

const QString kScripts = "Scripts";
const QString kReadOnly = "ReadOnly";
const QString kReadWrite = "ReadWrite";
const QString kTraps = "Traps";

const QString kKey = "Key";
const QString kOid = "Oid";
const QString kPath = "Path";
const QString kArguments = "Arguments";
const QString kType = "Type";

} // namespace conf

#endif // CONFIGKEYS_H
