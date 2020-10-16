#include "GroupsDataSource.h"

#include "src/connections/IConnection.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"

#include "SpProto.h"

GroupsDataSource::GroupsDataSource(QObject* parent) :
    IDatasource(parent)
{
}

const QString &GroupsDataSource::id() const
{
    return mID;
}

const QString &GroupsDataSource::connectionID() const
{
    return mConnectionID;
}

const QString &GroupsDataSource::activeGroup() const
{
    return mActiveGroup;
}

const QObject *GroupsDataSource::item(const QString &key)
{
    Q_UNUSED(key)
    return this;
}

const QMetaObject *GroupsDataSource::metaItem(const QString &key)
{
    Q_UNUSED(key)
    return metaObject();
}

void GroupsDataSource::setDefaut(const QString &key, const QString &defaut)
{

}

void GroupsDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

IConnection *GroupsDataSource::dataConnection() const
{
    return mSocket;
}


void GroupsDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void GroupsDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void GroupsDataSource::setActiveGroup(const QString &acitveGroup)
{
    mActiveGroup = acitveGroup;
    emit activeGroupChanged(QVariant::fromValue(mActiveGroup));
}

void GroupsDataSource::setDataConnection(IConnection *socket)
{
    if(mSocket)
        disconnect(mSocket);

    if(dynamic_cast<MonitoringSocket*>(socket))
    {
        mSocket = dynamic_cast<MonitoringSocket*>(socket);
        connect(mSocket, SIGNAL(socketConnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(socketDisconnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(newVariantMessage(QVariant)), this, SLOT(onVariantRecieved(QVariant)));
    }
    else
    {
        mSocket = 0;
    }
}

void GroupsDataSource::onSocketStateChanged()
{
    if(!mSocket)
        return;

    if(mSocket->isConnected())
    {
        mSocket->requestActiveGroup();
    }
    else
    {
        setActiveGroup(QString());
    }
}

void GroupsDataSource::onVariantRecieved(QVariant v)
{
    if ( v.canConvert<SupervisorProto::Groups>() )
    {
        mSocket->requestActiveGroup();
    }
    else if ( v.canConvert<SupervisorProto::ActiveGroup>() )
    {
        int idx = mSocket->findGroupIndex(mSocket->activeGroup());
        if(idx >= 0 && idx < mSocket->countGroups())
            setActiveGroup(mSocket->group(idx).visualName);
        else
            setActiveGroup(QString());
    }
}

void GroupsDataSource::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit activeGroupChanged(QVariant::fromValue(mActiveGroup));
}

