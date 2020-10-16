#include "SnmpDataSource.h"

#include "Log.h"

#include "src/connections/IConnection.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"
#include "SpProto.h"

SnmpDataSource::SnmpDataSource(QObject *parent) :
    IDatasource(parent), mState(DEFAULT), mSocket(0)
{

}

IConnection *SnmpDataSource::dataConnection() const
{
    return mSocket;
}

const QString &SnmpDataSource::id() const
{
    return mID;
}

const QString &SnmpDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState SnmpDataSource::state() const
{
    return mState;
}

const QObject *SnmpDataSource::item(const QString &key)
{
    if(key.isEmpty())
        return this;

    if(!mItems.contains(key))
    {
        SnmpDataSourceItem* item = new SnmpDataSourceItem(mConnectionID, this);
        item->setID(key);
        mItems.insert(key, item);
    }
    return mItems[key];
}

const QMetaObject *SnmpDataSource::metaItem(const QString &key)
{
    return item(key)->metaObject();
}

void SnmpDataSource::setDefaut(const QString &key, const QString &defaut)
{
    if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        mItems.value(key)->setDefaut(defaut);
    }
}

void SnmpDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

void SnmpDataSource::bind()
{
    foreach(SnmpDataSourceItem* item, mItems)
        item->bind();

    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
}

void SnmpDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void SnmpDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void SnmpDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "SNMP DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(state));
}

void SnmpDataSource::setDataConnection(IConnection *socket)
{
    if(mSocket)
        disconnect(mSocket);

    if(dynamic_cast<MonitoringSocket*>(socket))
    {
        mSocket = dynamic_cast<MonitoringSocket*>(socket);
        connect(mSocket, SIGNAL(netStatusChanged()), this, SLOT(onNetStatusChanged()));
        connect(mSocket, SIGNAL(socketConnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(socketDisconnected()), this, SLOT(onSocketStateChanged()));
        connect(mSocket, SIGNAL(newVariantMessage(QVariant)), this, SLOT(onVariantRecieved(QVariant)));
    }
    else
    {
        mSocket = 0;
    }
}

void SnmpDataSource::onNetStatusChanged()
{
    if(!mSocket)
        return;
}

void SnmpDataSource::onSocketStateChanged()
{
    if(!mSocket)
        return;

    if(mSocket->isConnected())
    {
        foreach(SnmpDataSourceItem* item, mItems)
            item->setState(OK_WARNING);
    }
    else
    {
        SupervisorWidgetState state;
        if ( !mSocket->isStatusOn() )
            state = OUT_OF_SCOPE;
        else
            state = UNREACHABLE;

        foreach(SnmpDataSourceItem* item, mItems)
        {
            item->setState(state);
            QString defaut = item->getDefaut();
            if(defaut.size() != 0)
                item->setItemName(item->getDefaut());
            else
                item->setItemName("");
        }

        setState(state);
    }
}

void SnmpDataSource::onVariantRecieved(QVariant v)
{
    if(v.canConvert<SupervisorProto::SnmpVariables>())
    {
        SupervisorProto::SnmpVariables spVariables = v.value<SupervisorProto::SnmpVariables>();

        if (mItems.contains(spVariables.agent))
        {
            SnmpDataSourceItem *item = mItems[spVariables.agent];

            if (spVariables.variables.contains(item->stateVariable()))
            {
                item->setState(spVariables.variables[item->stateVariable()].toString());
            }
            else
            {
                item->setState(UNREACHABLE);
            }
        }
//        QVariantMap variables = spVariables.variables;

//        QVariantMap::const_iterator it = variables.constBegin();
//        while (it != variables.constEnd()) {
//            qDebug() << it.key() << it.value();
//            ++it;
//        }
    }
}


SnmpDataSourceItem::SnmpDataSourceItem(const QString &connectionId, QObject *parent) :
    QObject(parent), mConnectionID(connectionId), mState(DEFAULT), mStateVariable("state")
{

}

const QString &SnmpDataSourceItem::id() const
{
    return mID;
}

SupervisorWidgetState SnmpDataSourceItem::state() const
{
    return mState;
}

const QString &SnmpDataSourceItem::itemName() const
{
    return mItemName;
}

void SnmpDataSourceItem::setDefaut(const QString &defaut)
{
    mDefaut = defaut;
}

QString SnmpDataSourceItem::getDefaut()
{
    return mDefaut;
}

QString SnmpDataSourceItem::stateVariable() const
{
    return mStateVariable;
}

void SnmpDataSourceItem::setStateVariable(const QString variable)
{
    mStateVariable = variable;
}

void SnmpDataSourceItem::setStateMap(QMap<QString, SupervisorWidgetState> map)
{
    mStateMap = map;
}

void SnmpDataSourceItem::setState(const QString &variable)
{
    SupervisorWidgetState state;
    if (mStateMap.contains(variable))
    {
        state = mStateMap[variable];
    }
    else
    {
        state = static_cast<SupervisorWidgetState>(variable.toInt());
    }

    setState(state);
}

void SnmpDataSourceItem::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit stateChanged(QVariant::fromValue(mState));
    emit itemNameChanged(QVariant::fromValue(mItemName));
}

void SnmpDataSourceItem::setID(const QString &id)
{
    mID = id;

    emit idChanged(QVariant::fromValue(mID));
}

void SnmpDataSourceItem::setState(SupervisorWidgetState state)
{
    if (mState != state)
        qLogInfo(tr("Event")) << QString("SNMP DataSource item '%1/%2' changed state to: '%3'")
                              .arg(mConnectionID).arg(mID).arg(widgetStateAsString(state));

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void SnmpDataSourceItem::setItemName(const QString &name)
{
    mItemName = name;
    emit itemNameChanged(QVariant::fromValue(mItemName));
}
