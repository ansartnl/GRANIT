#include "FolderDataSource.h"

#include "Log.h"

#include "src/connections/IConnection.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"
#include "SpProto.h"

FolderDataSource::FolderDataSource(QObject *parent) :
    IDatasource(parent), mState(DEFAULT), mSocket(0)
{

}

IConnection *FolderDataSource::dataConnection() const
{
    return mSocket;
}

const QString &FolderDataSource::id() const
{
    return mID;
}

const QString &FolderDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState FolderDataSource::state() const
{
    return mState;
}

SupervisorWidgetState FolderDataSource::stateNew() const
{
    return mState;
}

const QObject *FolderDataSource::item(const QString &key)
{
    if(key.isEmpty())
        return this;

    if(!mItems.contains(key))
    {
        FolderDataSourceItem* item = new FolderDataSourceItem(mConnectionID, this);
        item->setID(key);
        mItems.insert(key, item);
    }
    return mItems[key];
}

const QMetaObject *FolderDataSource::metaItem(const QString &key)
{
    return item(key)->metaObject();
}

void FolderDataSource::setDefaut(const QString &key, const QString &defaut)
{
    if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        if(!mItems.value(key)->getDefaut().size())
            mItems.value(key)->setDefaut(defaut);
    }
}

void FolderDataSource::setDatafield(const QString &key, const QString &datafield)
{
    if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        mItems.value(key)->setDatafield(datafield);
    }
}

void FolderDataSource::bind()
{
    foreach(FolderDataSourceItem* item, mItems)
        item->bind();

    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
}

void FolderDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void FolderDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void FolderDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Folder DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(state));
}

void FolderDataSource::setStateNew(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Folder DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChangedNew(QVariant::fromValue(state));
}

void FolderDataSource::setDataConnection(IConnection *socket)
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

void FolderDataSource::onNetStatusChanged()
{
    if(!mSocket)
        return;
}

void FolderDataSource::onSocketStateChanged()
{
    if(!mSocket)
        return;

    if(mSocket->isConnected())
    {
        foreach(FolderDataSourceItem* item, mItems)
            item->setState(UNREACHABLE);
    }
    else
    {
        SupervisorWidgetState state;
        if ( !mSocket->isStatusOn() )
            state = OUT_OF_SCOPE;
        else
            state = UNREACHABLE;

        foreach(FolderDataSourceItem* item, mItems)
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

void FolderDataSource::onVariantRecieved(QVariant v)
{
    if(v.canConvert<SupervisorProto::FolderItems>())
    {
        SupervisorProto::FolderItems msg = v.value<SupervisorProto::FolderItems>();
        foreach(FolderDataSourceItem* item, mItems)
        {
            QList<QPair<QString, QByteArray>> filtered;
            std::copy_if(msg.items.cbegin(), msg.items.cend(), std::back_inserter(filtered),
                         [item](const QPair<QString, QByteArray> &i) { return i.first.contains(item->id()); }
            );

            if(!filtered.isEmpty())
            {
                const QString name = filtered.first().first;
                const QString content = filtered.first().second;

                bool ok(false);
                int newState;
                QString key;
                if(name.size() > 1)
                {
                    newState = name.left(2).toInt(&ok);
                    key = name.mid(2, name.size() - 2);
                }

                if(ok)
                    item->setState((SupervisorWidgetState)newState);
                else
                    item->setState(OK);

                if(mItems.contains(key))
                {
                    QString datafield = mItems.value(key)->getDatafield();
                    if(datafield == "itemName")
                    {
                        item->setItemName(name);
                    }
                    if(datafield == "itemData")
                    {
                        if(content.size() == 0)
                        {
                            QString defaut = mItems.value(key)->getDefaut();
                            if(defaut.size() != 0)
                                item->setItemName(defaut);
                        }
                        else
                            item->setItemName(content);
                    }
                }

            }
            else
            {
                item->setState(UNREACHABLE);
                QString defaut = item->getDefaut();
                if(defaut.size() != 0)
                    item->setItemName(defaut);
            }

            if(filtered.size() > 1)
                qLogWarn(tr("Folder Datasource")) << "Multiple key match found: " << item->id();
        }
    }
}


FolderDataSourceItem::FolderDataSourceItem(const QString &connectionId, QObject *parent) :
    QObject(parent), mConnectionID(connectionId), mState(DEFAULT)
{

}

const QString &FolderDataSourceItem::id() const
{
    return mID;
}

SupervisorWidgetState FolderDataSourceItem::state() const
{
    return mState;
}

SupervisorWidgetState FolderDataSourceItem::stateNew() const
{
    return mState;
}

const QString &FolderDataSourceItem::itemName() const
{
    return mItemName;
}

const QString &FolderDataSourceItem::itemData() const
{

}

void FolderDataSourceItem::setDefaut(const QString &defaut)
{
    mDefaut = defaut;
}

QString FolderDataSourceItem::getDefaut()
{
    return mDefaut;
}

void FolderDataSourceItem::setDatafield(const QString &datafield)
{
    mDatafield = datafield;
}

QString FolderDataSourceItem::getDatafield()
{
    return mDatafield;
}

void FolderDataSourceItem::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit stateChanged(QVariant::fromValue(mState));
    emit itemNameChanged(QVariant::fromValue(mItemName));
}

void FolderDataSourceItem::setID(const QString &id)
{
    mID = id;

    emit idChanged(QVariant::fromValue(mID));
}

void FolderDataSourceItem::setState(SupervisorWidgetState state)
{
    if (mState != state)
        qLogInfo(tr("Event")) << QString("Folder DataSource item '%1/%2' changed state to: '%3'")
                              .arg(mConnectionID).arg(mID).arg(widgetStateAsString(state));

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void FolderDataSourceItem::setStateNew(SupervisorWidgetState state)
{
    if (mState != state)
        qLogInfo(tr("Event")) << QString("Folder DataSource item '%1/%2' changed state to: '%3'")
                              .arg(mConnectionID).arg(mID).arg(widgetStateAsString(state));

    mState = state;
    emit stateChangedNew(QVariant::fromValue(mState));
}

void FolderDataSourceItem::setItemName(const QString &name)
{
    mItemName = name;
    emit itemNameChanged(QVariant::fromValue(mItemName));
}

void FolderDataSourceItem::setItemDate(const QString &date)
{
    mItemName = date;
    emit itemNameChanged(QVariant::fromValue(mItemName));
}
