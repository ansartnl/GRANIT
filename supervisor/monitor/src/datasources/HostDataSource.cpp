#include "HostDataSource.h"
#include "Log.h"

#include "src/connections/IConnection.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"

HostDataSource::HostDataSource(QObject *parent)
    : IDatasource(parent), mState(DEFAULT)
{
}

const QString &HostDataSource::id() const
{
    return mID;
}

const QString &HostDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState HostDataSource::state() const
{
    return mState;
}

const QString &HostDataSource::name() const
{
    return mName;
}

const QString &HostDataSource::cwp() const
{
    return mCWPSector.name;
}

const QString &HostDataSource::client() const
{
    return mCWPSector.client;
}

const QStringList &HostDataSource::sectors() const
{
    return mCWPSector.sectors;
}

const QObject *HostDataSource::item(const QString &key)
{
    Q_UNUSED(key)
    return this;
}

const QMetaObject *HostDataSource::metaItem(const QString &key)
{
    Q_UNUSED(key)
    return metaObject();
}

void HostDataSource::setDefaut(const QString &key, const QString &defaut)
{

}

void HostDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

IConnection *HostDataSource::dataConnection() const
{
    return mSocket;
}


void HostDataSource::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
    emit nameChanged(QVariant::fromValue(mName));
    emit cwpChanged(QVariant::fromValue(mCWPSector.name));
    emit clientChanged(QVariant::fromValue(mCWPSector.client));
    emit sectorsChanged(QVariant::fromValue(mCWPSector.sectors.join("\n")));
}

void HostDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void HostDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void HostDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Host DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void HostDataSource::setName(const QString &name)
{
    mName = name;
    emit nameChanged(QVariant::fromValue(mName));
}

void HostDataSource::setCWP(const QString &cwp)
{
    mCWPSector.name = cwp;
    emit cwpChanged(QVariant::fromValue(mCWPSector.name));
}

void HostDataSource::setClient(const QString &client)
{
    mCWPSector.client = client;
    emit clientChanged(QVariant::fromValue(mCWPSector.client));
}

void HostDataSource::setSectors(const QStringList &sectors)
{
    mCWPSector.sectors = sectors;
    emit sectorsChanged(QVariant::fromValue(mCWPSector.sectors.join("\n")));
}

void HostDataSource::setDataConnection(IConnection *socket)
{
    if(mSocket)
        disconnect(mSocket);

    if(dynamic_cast<MonitoringSocket*>(socket))
    {
        mSocket = dynamic_cast<MonitoringSocket*>(socket);
        connect(mSocket, SIGNAL(socketConnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(socketDisconnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(newVariantMessage(QVariant)), this, SLOT(onVariantRecieved(QVariant)));

        setName(mSocket->name());
    }
    else
    {
        mSocket = 0;
        setName("-");
    }
}

void HostDataSource::onSocketStateChanged()
{
    if(!mSocket)
        return;

    if(mSocket->isConnected())
    {
        setState(OK);
        mSocket->requestAllStatuses();
    }
    else
    {
        if ( !mSocket->isStatusOn() )
            setState(OUT_OF_SCOPE);
        else
            setState(UNREACHABLE);
        setCWP(QString());
        setClient(QString());
        setSectors(QStringList());
    }
}

void HostDataSource::onVariantRecieved(QVariant v)
{
    if ( v.canConvert<SupervisorProto::Status>() )
    {
        SupervisorProto::Status status = v.value<SupervisorProto::Status>();
        if(status.type == SupervisorProto::CWP && status.status == SupervisorProto::Stopped)
        {
            setCWP(QString());
            setClient(QString());
            setSectors(QStringList());
        }
    }
    else if ( v.canConvert<SupervisorProto::CWPSectors>() )
    {
        SupervisorProto::CWPSectors sectors = v.value<SupervisorProto::CWPSectors>();
        setCWP(sectors.name);
        setClient(sectors.client);
        setSectors(sectors.sectors);
    }

    updateSocketState();
}

void HostDataSource::updateSocketState()
{
    if(!mSocket)
        return;

    if ( !mSocket->isStatusOn() )
        setState(OUT_OF_SCOPE);
    else if ( !mSocket->isConnected() )
        setState(UNREACHABLE);
    else if ( !mSocket->isValid() )
    {
//        if ( mSocket->isModify() )
//            setState(MODIFY);
//        else
            setState(OK_WARNING);
    }
    else if (!mSocket->isAllProcessesOn())
        setState(PARTIAL);
    else
        setState(OK);
}
