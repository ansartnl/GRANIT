#include "MonitoringSocket.h"

#include "SpProto.h"

#include "Log.h"
#include "main.h"
#include "qtvariantconnection.h"
#include "qtsocketautoconnector.h"
#include "qttcpconnectionchecker.h"

#include <QtNetwork/QHostAddress>

QString toString(MonitoredObject::Type type)
{
    QString typeString;
    if ( MonitoredObject::Server == type )
        typeString = "Server";
    if ( MonitoredObject::CWP == type )
        typeString = "CWP";
    else if ( MonitoredObject::FDP == type )
        typeString = "FDP";
    else if ( MonitoredObject::FMTP == type )
        typeString = "FMTP";
    else if ( MonitoredObject::STCA == type )
        typeString = "STCA";
    else if ( MonitoredObject::AFTNWS == type )
        typeString = "AFTNWS";
    else if ( MonitoredObject::DBSYNC == type )
        typeString = "DBSYNC";
    else if ( MonitoredObject::PROCESS == type )
        typeString = "Process";
    else if ( MonitoredObject::SERVICE == type )
        typeString = "Service";
    else
        typeString = "Unknown";
    return typeString;
}
QString toString(MonitoredObject::Status status)
{
    QString statusString = QObject::tr("Unknown");
    switch (status)
    {
    case MonitoredObject::Started:
        statusString = QObject::tr("Started");
        break;
    case MonitoredObject::Stopped:
        statusString = QObject::tr("Stopped");
        break;
    }
    return statusString;
}

MonitoringSocket::MonitoringSocket(const QString &id, const QString &name,
                                   const QString &host, const QString &host2,
                                   quint16 port, QObject *parent)
    : QTcpSocket(parent),
      mConnection(0), mConnector(0),
      mId(id), mName(name), mHost(host), mHost2(host2), mPort(port), mCurrentObject(0),
      mIsStatusOn(false), mModifyFlag(false)
{
    if (host2.isEmpty())
        qLogInfo(tr("Monitoring")) << tr(" Added new socket for monitoring. Id: %1. Name: %2. Address: %3. Port: %4.")
                                      .arg(id, name, host, QString::number(port));
    else
        qLogInfo(tr("Monitoring")) << tr(" Added new socket for monitoring. Id: %1. Name: %2. Address: %3. Spare Address: %4. Port: %5.")
                                      .arg(id, name, host, host2, QString::number(port));
    mConnection = new QtVariantConnection(this, this);
    mConnection->setProtoVersion( SP_PROTO_VERSION );
    connect(mConnection, SIGNAL(gotVariant(QVariant)), SLOT(on_variantReceived(QVariant)));

    int reconnectionTimeout = xmlSettings().value("reconnection_timeout", 10).toInt() * 1000;

    mConnectionController = new QtConnectionControler(mConnection, this);
    mConnectionController->setInterval( reconnectionTimeout );
    connect(mConnectionController, SIGNAL(connected()), SIGNAL(socketConnected()));
    connect(mConnectionController, SIGNAL(disconnected()), SIGNAL(socketDisconnected()));

    connect(this, SIGNAL(connected()), SIGNAL(socketConnected()));
    connect(this, SIGNAL(disconnected()), SIGNAL(socketDisconnected()));

    mConnector = new QtSocketAutoConnector(this);
    mConnector->setInterval( reconnectionTimeout );
    connect(mConnector, SIGNAL(connectingAttempt(QHostAddress,quint16)), this, SLOT(on_connectingAttempt(QHostAddress,quint16)));

    connect(mConnectionController, SIGNAL(disconnected()), SLOT(on_disconnected()));
    connect(mConnectionController, SIGNAL(connected()), SLOT(on_connected()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(on_error(QAbstractSocket::SocketError)));
}

bool MonitoringSocket::isConnected() const
{
    return ( mConnectionController && mConnectionController->isConnected() );
}

void MonitoringSocket::changeStatus(bool isOn)
{
    if ( mIsStatusOn == isOn )
        return;

    mIsStatusOn = isOn;

    // Clear available hosts for this connection.
    mConnector->removeAllAddresses();

    // Disconnect this socket from connected host.
    if ( !isOn )
    {
        mConnector->setSocket(0);

        disconnectFromHost();
        if ( !(state() == QAbstractSocket::UnconnectedState || waitForDisconnected(2000)) )
            abort();
    }
    // Connect this socket to specified hosts.
    else
    {
        mConnector->addAddress(QHostAddress(mHost), mPort);
        if (!mHost2.isEmpty())
            mConnector->addAddress(QHostAddress(mHost2), mPort);
        mConnector->setSocket(this);
    }
}

void MonitoringSocket::on_connected()
{
    emit eventToLog(mName, QString(), tr("Host connected"), Qt::darkGreen);
    qLogInfo(tr("Monitoring")) << tr(" Monitoring socket '%1' has been connected to %2:%3 successfully")
                                  .arg(mName).arg(peerAddress().toString()).arg(peerPort());

    // Request monitored object list.
    requestObjects();
    requestNetworks();
    requestScripts();
    requestGroups();

    emit netStatusChanged();
}

void MonitoringSocket::on_disconnected()
{
    emit eventToLog(mName, QString(), tr("Host disconnected"), Qt::red);
    qLogInfo(tr("Monitoring")) << tr(" Monitoring socket '%1' was disconnected").arg(mName);

    clear();

    mConnector->removeAllAddresses();
    mConnector->addAddress(QHostAddress(mHost), mPort);
    mConnector->addAddress(QHostAddress(mHost2), mPort);
    mConnector->setSocket(this);

    emit netStatusChanged();
}

void MonitoringSocket::on_error(QAbstractSocket::SocketError socketError)
{
    qLogWarn(tr("Monitoring")) << tr(" Monitoring socket '%1' error: %2")
                                  .arg(mName).arg(socketError);
}

void MonitoringSocket::on_connectingAttempt(QHostAddress host, quint16 port)
{
    qLogInfo(tr("Monitoring")) << tr(" Monitoring socket '%1' is connecting to %2:%3")
                                  .arg(mName).arg(host.toString()).arg(port);
}

int MonitoringSocket::countObjects() const
{
    return mObjects.count();
}

const MonitoredObject& MonitoringSocket::object(int index) const
{
    Q_ASSERT(index < mObjects.size());
    return mObjects.at(index);
}

int MonitoringSocket::findIndex(MonitoredObject::Type type, const QString &name) const
{
    int index = -1;
    foreach ( const MonitoredObject &obj, mObjects )
    {
        index++;
        if ( type != obj.type )
            continue;

        if ( name == obj.name )
            return index;
    }
    return -1;
}

void MonitoringSocket::clearObjects()
{
    mCurrentObject = 0;
    mObjects.clear();
}

void MonitoringSocket::clear()
{
    clearObjects();
    clearGroups();
    mAvailableNets.clear();
    mScripts.clear();
}

MonitoredObject* MonitoringSocket::findObject(MonitoredObject::Type type, const QString &name)
{
    int index = findIndex(type, name);
    if ( index < 0 || index >= mObjects.size() )
        return 0;

    return &mObjects[index];
}

int MonitoringSocket::countGroups() const
{
    return mGroups.size();
}

const Group& MonitoringSocket::group(int index) const
{
    Q_ASSERT(index < mGroups.size());
    return mGroups.at(index);
}

int MonitoringSocket::findGroupIndex(const QString &name) const
{
    int index = -1;
    foreach ( const Group &g, mGroups )
    {
        index++;
        if ( g.name == name )
            return index;
    }
    return -1;
}

void MonitoringSocket::clearGroups()
{
    mActiveGroup = QString();
    mGroups.clear();
}

void MonitoringSocket::on_variantReceived(QVariant v)
{
    // Services information
    if ( v.canConvert<SupervisorProto::Services>() )
    {
        clearObjects();

        QSettings stg;
        stg.beginGroup("status/groups");

        SupervisorProto::Services services = v.value<SupervisorProto::Services>();
        // Create monitored object list.
        foreach ( const SupervisorProto::Service &s, services )
        {
            MonitoredObject obj;
            obj.type = (MonitoredObject::Type) s.type;
            obj.name = s.name;
            obj.description = s.description;
            obj.status = (MonitoredObject::Status) s.status;
            obj.mVisualName = s.visualName;
            obj.isOn = stg.value(obj.name, true).toBool();

            mObjects << obj;
        }

        stg.endGroup();

        requestAllStatuses();
    }
    // Groups information
    else if ( v.canConvert<SupervisorProto::Groups>() )
    {
        clearGroups();

        SupervisorProto::Groups groups = v.value<SupervisorProto::Groups>();

        // Fill list widget with available groups for selected monitored connection.
        foreach ( const SupervisorProto::Group &g, groups )
        {
            Group group;
            group.name = g.name;
            group.visualName = g.visualName;

            mGroups << group;
        }

        requestActiveGroup();
    }
    else if ( v.canConvert<SupervisorProto::ActiveGroup>() )
    {
        SupervisorProto::ActiveGroup activeGroup = v.value<SupervisorProto::ActiveGroup>();
        mActiveGroup = activeGroup.name;
    }
    // Status changed
    else if ( v.canConvert<SupervisorProto::Status>() )
    {
        SupervisorProto::Status status = v.value<SupervisorProto::Status>();
        MonitoredObject *obj = findObject((MonitoredObject::Type)status.type, status.name);
        if ( obj && int(obj->status) != int(status.status) )
        {
            setModify(true);
            if (status.status == SupervisorProto::Started)
            {
                emit eventToLog(mName, obj->visualName(), tr("Status changed to: '%1'")
                                .arg(toString((MonitoredObject::Status)status.status))
                                , Qt::darkGreen);
            } else
            {
                emit eventToLog(mName, obj->visualName(), tr("Status changed to: '%1'")
                                .arg(toString((MonitoredObject::Status)status.status))
                                , Qt::red);
            }
            obj->status = (MonitoredObject::Status) status.status;
            qLogInfo(tr("Monitoring")) << tr(" Monitored object %1 changed status to %2")
                                          .arg(status.name, toString((MonitoredObject::Status)status.status));
        }
    }
    // FMTP Status changed
    else if ( v.canConvert<SupervisorProto::FMTPStatus>() )
    {
        SupervisorProto::FMTPStatus status = v.value<SupervisorProto::FMTPStatus>();
        MonitoredObject *obj = findObject((MonitoredObject::FMTP), status.name);
        if ( obj )
        {
            setModify(true);
            emit eventToLog(mName, obj->visualName(), tr("Fmtp status changed to: '%1'")
                            .arg(SupervisorProto::toString(status.status)));
            obj->additionalStatus = QString("%1").arg(toString(status.status));
            qLogInfo(tr("Monitoring")) << tr(" Monitored fmtp service %1 changed status to %2")
                                          .arg(status.name, SupervisorProto::toString(status.status));
        }
    }
    // CWP Sectors changed
    else if ( v.canConvert<SupervisorProto::CWPSectors>() )
    {
        SupervisorProto::CWPSectors sectors = v.value<SupervisorProto::CWPSectors>();
        MonitoredObject *obj = findObject((MonitoredObject::CWP), sectors.name);
        if ( obj )
        {
            setModify(true);
            if (obj->additionalStatus != QString("%1").arg(sectors.client))
                emit eventToLog(mName, obj->visualName(), tr("Cwp client changed to: '%1'")
                                .arg(sectors.client));
            if (obj->cwpSectors != sectors.sectors)
                emit eventToLog(mName, obj->visualName(), tr("Cwp sectors changed to: '%1'")
                                .arg(sectors.sectors.join("; ")));
            obj->additionalStatus = QString("%1").arg(sectors.client);
            obj->cwpSectors = sectors.sectors;
            qLogInfo(tr("Monitoring")) << tr(" Monitored cwp %1 with client %2 changed sectors to '%3'")
                                          .arg(sectors.name
                                               , sectors.client
                                               , sectors.sectors.join("; "));
        }
    }
    // CWP User changed
    else if ( v.canConvert<SupervisorProto::CWPUser>() )
    {
        SupervisorProto::CWPUser cwpUser = v.value<SupervisorProto::CWPUser>();
        MonitoredObject *obj = findObject((MonitoredObject::CWP), cwpUser.name);
        if ( obj )
        {
            setModify(true);
            emit eventToLog(mName, obj->visualName(), tr("Cwp user changed to: '%1'")
                            .arg(cwpUser.user));
            obj->user = cwpUser.user;
            qLogInfo(tr("Monitoring")) << tr(" Monitored cwp %1 user changed to %2")
                                          .arg(cwpUser.name
                                               , cwpUser.user);
        }
    }
    // FDP Status changed
    else if ( v.canConvert<SupervisorProto::FDPStatus>() )
    {
        SupervisorProto::FDPStatus status = v.value<SupervisorProto::FDPStatus>();
        MonitoredObject *obj = findObject((MonitoredObject::FDP), status.name);
        if ( obj )
        {
            setModify(true);
            emit eventToLog(mName, obj->visualName(), tr("Fdp status changed to: '%1'")
                            .arg(SupervisorProto::fdpStatus(status.status)));
            obj->additionalStatus = QString("%1").arg(SupervisorProto::fdpStatus(status.status));
            qLogInfo(tr("Monitoring")) << tr(" Monitored fdp service %1 changed status to %2")
                                          .arg(status.name, SupervisorProto::fdpStatus(status.status));
        }
    }
    // Nets changed
    else if ( v.canConvert<SupervisorProto::NetList>() )
    {
        SupervisorProto::NetList list = v.value<SupervisorProto::NetList>();
        if (mAvailableNets != list.list)
        {
            setModify(true);
            if (mAvailableNets.count() > list.list.count())
            {
                emit eventToLog(mName, QString(), tr("Net changed to: '%1'")
                                .arg(list.list.join("; ")), Qt::red);
            } else if (mAvailableNets.count() < list.list.count())
            {
                emit eventToLog(mName, QString(), tr("Net changed to: '%1'")
                                .arg(list.list.join("; ")), Qt::darkGreen);
            } else
            {
                emit eventToLog(mName, QString(), tr("Net changed to: '%1'")
                                .arg(list.list.join("; ")), QColor::fromRgb(255, 128, 0));
            }
        }
        if (list.list.count())
        {
            qLogInfo(tr("Monitoring")) << tr(" Monitored server's net state changed. Avaliable networks : '%1'")
                                          .arg(list.list.join("' '"));
            mAvailableNets = list.list;
        } else
        {
            qLogInfo(tr("Monitoring")) << tr(" Monitored server's net state changed. No available networks");
            mAvailableNets.clear();
        }
    }
    // Available scripts
    else if ( v.canConvert<SupervisorProto::ScriptList>() )
    {
        SupervisorProto::ScriptList list = v.value<SupervisorProto::ScriptList>();
        if (list.scripts.count())
        {
            qLogInfo(tr("Monitoring")) << tr(" Monitored server's available scripts list received");
            mScripts = list.scripts;
            emit scriptListChanged();
        } else
        {
            qLogInfo(tr("Monitoring")) << tr(" No scripts available on monitored server");
            mScripts.clear();
            emit scriptListChanged();
        }
    }
    // Script string
    else if ( v.canConvert<SupervisorProto::ScriptString>() )
    {
        SupervisorProto::ScriptString s = v.value<SupervisorProto::ScriptString>();
        mScriptOutput.append(s.str);
        emit scriptOutputChanged();
    }

    // Emit signal to other receivers.
    // Need it here to be sure that object is parsed in this function first.
    emit newVariantMessage(v);
}

bool MonitoringSocket::isValid() const
{
    foreach ( const MonitoredObject &obj, mObjects )
    {
        // Do not process objects that are 'Off'.
        if ( !obj.isOn )
            continue;

        if ( MonitoredObject::Started != obj.status )
            return false;
        if ( MonitoredObject::FMTP == obj.type)
        {
            if (obj.additionalStatus != QString("%1").arg(SupervisorProto::toString(SupervisorProto::enDataReady)))
                return false;
        }
    }
    return !mObjects.empty();
}

void MonitoringSocket::requestObjects()
{
    if ( state() == QAbstractSocket::ConnectedState )
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::Request::Services) );
}

void MonitoringSocket::requestNetworks()
{
    if ( state() == QAbstractSocket::ConnectedState )
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::Request::AvailableNetworks) );
}

void MonitoringSocket::requestScripts()
{
    if ( state() == QAbstractSocket::ConnectedState )
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::Request::AvailableScripts) );
}

void MonitoringSocket::requestStatus(MonitoredObject::Type type, const QString &name)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::RequestStatus reqStatus = { SupervisorProto::EType(type), name };
        mConnection->sendVariant( QVariant::fromValue(reqStatus) );
    }
}

void MonitoringSocket::requestAllStatuses()
{
    foreach (const MonitoredObject &obj, mObjects)
    {
        requestStatus(obj.type, obj.name);
    }
}

void MonitoringSocket::requestLog(MonitoredObject::Type type, const QString &name)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::RequestLog reqLog = { SupervisorProto::EType(type), name };
        mConnection->sendVariant( QVariant::fromValue(reqLog) );
    }
}

void MonitoringSocket::requestRemoteOperations(const QString &name, const QDateTime &from, const QDateTime &to)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::RequestRemoteOperations reqLog;
        reqLog.name = name;
        reqLog.from = from;
        reqLog.to   = to;
        mConnection->sendVariant( QVariant::fromValue(reqLog) );
    }
}

void MonitoringSocket::requestRemoteLogs(const QDateTime &from, const QDateTime &to)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::RequestRemoteLogs reqLog;
        reqLog.from = from;
        reqLog.to   = to;
        mConnection->sendVariant( QVariant::fromValue(reqLog) );
    }
}

bool MonitoringSocket::restart(MonitoredObject::Type type, const QString &name)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::Restart restart = { SupervisorProto::EType(type), name };
        mConnection->sendVariant( QVariant::fromValue(restart) );
        return true;
    }
    return false;
}

bool MonitoringSocket::startAll()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::StartAll()) );
        return true;
    }
    return false;
}

bool MonitoringSocket::stopAll()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::StopAll()) );
        return true;
    }
    return false;
}

MonitoredObject *MonitoringSocket::cwp()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        for (int i = 0; i < mObjects.count(); ++i)
        {
            if (mObjects.at(i).type == MonitoredObject::CWP)
                return &mObjects[i];
        }
    }
    return 0;
}

MonitoredObject * MonitoringSocket::fdp()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        for (int i = 0; i < mObjects.count(); ++i)
        {
            if (mObjects.at(i).type == MonitoredObject::FDP)
                return &mObjects[i];
        }
    }
    return 0;
}

void MonitoringSocket::activateFdp(const QString &name)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::ActivateFdp a;
        a.name = (name.isEmpty()) ? (fdp()->name) : (name);
        mConnection->sendVariant( QVariant::fromValue(a) );
    }
}

void MonitoringSocket::startScript(const QString &name, const QString &parameter)
{
    mScriptOutput.clear();
    emit scriptOutputChanged();
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::StartScript a;
        a.name = name;
        a.parameter = parameter;
        mConnection->sendVariant( QVariant::fromValue(a) );
    }
}

void MonitoringSocket::stopScript()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::StopScript()) );
    }
}

bool MonitoringSocket::shutdown()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::Shutdown()) );
        return true;
    }
    return false;
}

bool MonitoringSocket::reboot()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        mConnection->sendVariant( QVariant::fromValue(SupervisorProto::Reboot()) );
        return true;
    }
    return false;
}

void MonitoringSocket::requestGroups()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::Request::Request req = SupervisorProto::Request::Groups;
        mConnection->sendVariant( QVariant::fromValue(req) );
    }
}

void MonitoringSocket::requestActiveGroup()
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::Request::Request req = SupervisorProto::Request::ActiveGroup;
        mConnection->sendVariant( QVariant::fromValue(req) );
    }
}

void MonitoringSocket::activateGroup(const QString &name)
{
    if ( state() == QAbstractSocket::ConnectedState )
    {
        SupervisorProto::ActivateGroup group;
        group.name = name;

        mConnection->sendVariant( QVariant::fromValue(group) );
    }
}

void MonitoringSocket::setCurrentObject(int index)
{
    if (index >= 0 && index < countObjects())
        mCurrentObject = &mObjects[index];
    else
        mCurrentObject = 0;
}

void MonitoringSocket::setModify(bool modify)
{
    mModifyFlag = modify;
}

bool MonitoringSocket::isModify() const
{
    return mModifyFlag;
}
