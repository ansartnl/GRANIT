#include "ClientConnection.h"

#include "SpProto.h"
#include "SupervisorService.h"
#include "NetController.h"
#include "RequestThread.h"
#include "ScriptController.h"
#include "SnmpController.h"

#include "GroupsHelper.h"
#include "ApplicationController.h"
#include "DatabaseController.h"

#include "Log.h"
#include "qttcpconnectionchecker.h"

#include <QtNetwork/QHostAddress>


ClientConnection::ClientConnection(SupervisorService *supervisor,
                                   QTcpSocket *socket, QObject *parent)
    : QObject(parent), mConnection(0), mSupervisor(supervisor), mRequests(0)
{
    setObjectName(QString("%1:%2")
                  .arg(socket->peerAddress().toString())
                  .arg(socket->peerPort())
                  );

    mConnection = new QtVariantConnection(socket, this);
    mConnection->setProtoVersion( SP_PROTO_VERSION );
    connect(mConnection, SIGNAL(gotVariant(QVariant))
            , SLOT(on_variantReceived(QVariant)));
    connect(mConnection, SIGNAL(disconnected())
            , SLOT(on_disconnected()));

    mConnectionPing = new QtConnectionPing(mConnection, this);

    mNetController = new NetController(this);
    connect(mNetController, SIGNAL(up(QNetworkInterface))
            , SLOT(onNetChanged()));
    connect(mNetController, SIGNAL(down(QNetworkInterface))
            , SLOT(onNetChanged()));
    mNetController->startMonitoring();

    mScriptController = supervisor->scriptController();
    connect(mScriptController, SIGNAL(outputString(QString))
            , SLOT(onScriptOutputString(QString)));
    connect(mScriptController, SIGNAL(scriptFinished(int, QProcess::ExitStatus))
            , SLOT(onScriptFinished(int, QProcess::ExitStatus)));

    mFolderController = supervisor->folderController();
    connect(mFolderController, SIGNAL(folderItems(QList<FolderController::FileItem>)),
            this, SLOT(onFolderItemsRecieved(QList<FolderController::FileItem>)));
    mFolderController->startMonitoring();

    mSnmpController = supervisor->snmpController();
    connect(mSnmpController, SIGNAL(variablesReceived(QString,QVariantMap)),
            this, SLOT(onSnmpVariablesReceived(QString,QVariantMap)));
    connect(mSnmpController, SIGNAL(trap(QString,QVariantMap)),
            this, SLOT(onSnmpVariablesReceived(QString,QVariantMap)));
    mSnmpController->start();

    mRequests = new RequestThread(this);
    mRequests->setControllers(supervisor->applicationController()
                              , supervisor->databaseController()
                              , mNetController
                              , mScriptController
                              , supervisor->logController()
                              , mSnmpController);
    connect(mRequests, SIGNAL(sendVariant(QVariant))
            , mConnection, SLOT(sendVariant(QVariant)));
    connect(mRequests, SIGNAL(sendStatus(Type,QString,Status)),
            SLOT(setStatus(Type,QString,Status)));
    connect(mRequests, SIGNAL(sendFmtpStatus(QString,EFMTPStatus)),
            SLOT(setFmtpStatus(QString,EFMTPStatus)));
    connect(mRequests, SIGNAL(sendCWPSectors(QString,QString,QStringList)),
            SLOT(setCWPSectors(QString,QString,QStringList)));
    connect(mRequests, SIGNAL(sendCWPUser(QString,QString)),
            SLOT(setCWPUser(QString,QString)));
    connect(mRequests, SIGNAL(sendFdpStatus(QString,bool)),
            SLOT(setFdpStatus(QString,bool)));
    connect(mRequests, SIGNAL(sendAdpStatus(QString, QString)),
            SLOT(setAdpStatus(QString, QString)));
    mRequests->start();
}

ClientConnection::~ClientConnection()
{
    mRequests->stop();
    if ( !mRequests->wait(30000) )
        mRequests->terminate();
}

void ClientConnection::on_disconnected()
{
    qLogInfo(tr("Client")) << tr(" Client connection '%1' disconnected").arg(objectName());

    // Do not need connection any more.
    deleteLater();
}

void ClientConnection::onNetChanged()
{
    mRequests->add( QVariant::fromValue(SupervisorProto::Request::AvailableNetworks) );
}

void ClientConnection::onScriptOutputString(const QString &str)
{
    SupervisorProto::ScriptString spString;
    spString.str = str;

    mConnection->sendVariant( QVariant::fromValue(spString) );
}

void ClientConnection::onScriptFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    SupervisorProto::ScriptFinished spFinished;
    spFinished.exitCode = exitCode;
    spFinished.exitStatus = exitStatus;

    mConnection->sendVariant( QVariant::fromValue(spFinished) );
}

void ClientConnection::onFolderItemsRecieved(const QList<FolderController::FileItem> &items)
{
    SupervisorProto::FolderItems spFolderItems;
    spFolderItems.items = items;

    mConnection->sendVariant( QVariant::fromValue(spFolderItems));
}

void ClientConnection::onSnmpVariablesReceived(QString agent, QVariantMap variables)
{
    SupervisorProto::SnmpVariables spVariables;
    spVariables.agent = agent;
    spVariables.variables = variables;

    mConnection->sendVariant(QVariant::fromValue(spVariables));
}

void ClientConnection::setStatus(Type type, const QString &name, Status status)
{
    SupervisorProto::Status spStatus;
    spStatus.type = (SupervisorProto::EType) type;
    spStatus.name = name;
    spStatus.status = (SupervisorProto::EStatus) status;

    mConnection->sendVariant( QVariant::fromValue(spStatus) );
}

void ClientConnection::setFmtpStatus(const QString &name, EFMTPStatus status)
{
    SupervisorProto::FMTPStatus spStatus;
    spStatus.name = name;
    spStatus.status = (SupervisorProto::EFMTPStatus) status;

    mConnection->sendVariant( QVariant::fromValue(spStatus) );
    qLogInfo(tr("Client")) << tr(" FMTP '%1' status changed to '%2' (%3)")
                              .arg(name)
                              .arg(SupervisorProto::toString(
                                       (SupervisorProto::EFMTPStatus)status))
                              .arg(QString::number(status));
}

void ClientConnection::setCWPSectors(const QString &name, const QString &client, const QStringList &sectors)
{
    SupervisorProto::CWPSectors spSectors;
    spSectors.name = name;
    spSectors.client = client;
    spSectors.sectors = sectors;

    mConnection->sendVariant( QVariant::fromValue(spSectors) );
    qLogInfo(tr("Client")) << tr(" CWP '%1' with client '%2' changed sectors to '%3'")
                              .arg(name)
                              .arg(client)
                              .arg(sectors.join(" "));
}

void ClientConnection::setCWPUser(const QString &name, const QString &user)
{
    SupervisorProto::CWPUser spUser;
    spUser.name = name;
    spUser.user = user;

    mConnection->sendVariant( QVariant::fromValue(spUser) );
    qLogInfo(tr("Client")) << tr(" CWP '%1' user changed to '%2'")
                              .arg(name)
                              .arg(user);
}

void ClientConnection::setFdpStatus(const QString &name, bool status)
{
    SupervisorProto::FDPStatus spStatus;
    spStatus.name = name;
    spStatus.status = status;

    mConnection->sendVariant( QVariant::fromValue(spStatus) );
    qLogInfo(tr("Client")) << tr(" FDP '%1' status changed to '%2'")
                              .arg(name)
                              .arg(status ? tr("Master") : tr("Slave"));
}

void ClientConnection::setAdpStatus(const QString &name, const QString &status)
{
    SupervisorProto::ADPStatus spStatus;
    spStatus.name = name;
    spStatus.status = status;

    mConnection->sendVariant( QVariant::fromValue(spStatus) );
    qLogInfo(tr("Client")) << tr(" ADP '%1' status changed to '%2'")
                              .arg(name)
                              .arg(status);
}

void ClientConnection::on_variantReceived(const QVariant &v)
{
    // Request
    if ( v.canConvert<SupervisorProto::Request::Request>() )
    {
        SupervisorProto::Request::Request req = v.value<SupervisorProto::Request::Request>();
        if ( SupervisorProto::Request::Services == req )
        {
            qLogInfo(tr("Client")) << tr(" Services request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
        else if ( SupervisorProto::Request::AvailableNetworks == req )
        {
            qLogInfo(tr("Client")) << tr(" Available networks request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
        else if ( SupervisorProto::Request::AvailableScripts == req )
        {
            qLogInfo(tr("Client")) << tr(" Available scripts request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
        else if ( SupervisorProto::Request::Groups == req )
        {
            qLogInfo(tr("Client")) << tr(" Groups request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
        else if ( SupervisorProto::Request::ActiveGroup == req )
        {
            qLogInfo(tr("Client")) << tr(" Active group request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
        else if ( SupervisorProto::Request::Logs == req )
        {
            qLogInfo(tr("Client")) << tr(" Available logs request received from %1:%2")
                                      .arg(mConnection->socket()->peerAddress().toString())
                                      .arg(mConnection->socket()->peerPort());
            mRequests->add(v);
        }
    }
    // Request Status
    else if ( v.canConvert<SupervisorProto::RequestStatus>() )
    {
        SupervisorProto::RequestStatus status = v.value<SupervisorProto::RequestStatus>();

        qLogInfo(tr("Client")) << tr(" Status '%1' request received from %2:%3")
                                  .arg(status.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Request special status
    else if ( v.canConvert<SupervisorProto::RequestSpecialStatus>() )
    {
        SupervisorProto::RequestSpecialStatus status = v.value<SupervisorProto::RequestSpecialStatus>();

        qLogInfo(tr("Client")) << tr(" Special status '%1' request received from %2:%3")
                                  .arg(status.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Request Log
    else if ( v.canConvert<SupervisorProto::RequestLog>() )
    {
        SupervisorProto::RequestLog log = v.value<SupervisorProto::RequestLog>();

        qLogInfo(tr("Client")) << tr(" Log '%1' request received from %2:%3")
                                  .arg(log.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Request Remote Operations
    else if ( v.canConvert<SupervisorProto::RequestRemoteOperations>() )
    {
        SupervisorProto::RequestRemoteOperations log = v.value<SupervisorProto::RequestRemoteOperations>();

        qLogInfo(tr("Client")) << tr(" Remote Operations '%1' request received from %2:%3")
                                  .arg(log.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Request Remote Logs
    else if ( v.canConvert<SupervisorProto::RequestRemoteLogs>() )
    {
        qLogInfo(tr("Client")) << tr(" Remote Logs request received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Restart service
    else if ( v.canConvert<SupervisorProto::Restart>() )
    {
        SupervisorProto::Restart restart = v.value<SupervisorProto::Restart>();

        qLogInfo(tr("Client")) << tr(" Restart '%1' command received from %2:%3")
                                  .arg(restart.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Stop all services
    else if ( v.canConvert<SupervisorProto::StopAll>() )
    {
        qLogInfo(tr("Client")) << tr(" Stop all command received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Start all services
    else if ( v.canConvert<SupervisorProto::StartAll>() )
    {
        qLogInfo(tr("Client")) << tr(" Start all command received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Activate FDP
    else if ( v.canConvert<SupervisorProto::ActivateFdp>() )
    {
        SupervisorProto::ActivateFdp fdp = v.value<SupervisorProto::ActivateFdp>();

        qLogInfo(tr("Client")) << tr(" Activate FDP '%1' command received from %2:%3")
                                  .arg(fdp.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Start new group (activate group)
    else if ( v.canConvert<SupervisorProto::ActivateGroup>() )
    {
        SupervisorProto::ActivateGroup aGroup = v.value<SupervisorProto::ActivateGroup>();

        qLogInfo(tr("Client")) << tr(" Activate group '%1' command received from %2:%3")
                                  .arg(aGroup.name)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());

        // Can not move these operations to thread as they require specified order of calls.
        // Note: potential looses of conections between clients and server as operations can take time.
        if ( aGroup.name != GroupsHelper::activeGroup() )
        {
            ApplicationController *actrl = mSupervisor->applicationController();
            DatabaseController *dbctrl = mSupervisor->databaseController();

            actrl->stop();

            GroupsHelper::setActiveGroup(aGroup.name);

            dbctrl->setMonitoredDatabases( actrl->activeDatabaseList() );
            dbctrl->restart();
        }
    }
    // Start script
    else if ( v.canConvert<SupervisorProto::StartScript>() )
    {
        SupervisorProto::StartScript script = v.value<SupervisorProto::StartScript>();

        qLogInfo(tr("Client")) << tr(" Start script '%1 %2' command received from %3:%4")
                                  .arg(script.name)
                                  .arg(script.parameter)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Stop script
    else if ( v.canConvert<SupervisorProto::StopScript>() )
    {
        qLogInfo(tr("Client")) << tr(" Stop script command received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Shutdown computer
    else if ( v.canConvert<SupervisorProto::Shutdown>() )
    {
        qLogInfo(tr("Client")) << tr(" Shutdown command received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Reboot computer
    else if ( v.canConvert<SupervisorProto::Reboot>() )
    {
        qLogInfo(tr("Client")) << tr(" Reboot command received from %1:%2")
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
    // Set SNMP variable
    else if ( v.canConvert<SupervisorProto::SetSnmpVariable>() )
    {
        SupervisorProto::SetSnmpVariable a = v.value<SupervisorProto::SetSnmpVariable>();

        qLogInfo(tr("Client")) << tr(" Set SNMP variable '%1' to '%2' on '%3' command received from %4:%5")
                                  .arg(a.variable)
                                  .arg(a.value)
                                  .arg(a.agent)
                                  .arg(mConnection->socket()->peerAddress().toString())
                                  .arg(mConnection->socket()->peerPort());
        mRequests->add(v);
    }
}

void ClientConnection::on_commandReceived(QVariant cmd)
{
    // This function used to avoid of writing lots of extra functions and used
    // for communication purpose between server code and Tcp client connections.

    // Workaround as if client manages from the server side.
    on_variantReceived(cmd);
}
