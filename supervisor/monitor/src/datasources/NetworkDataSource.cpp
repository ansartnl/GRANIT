#include "NetworkDataSource.h"
#include "Log.h"

#include "src/connections/IConnection.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"
#include "SpProto.h"

bool compareIPUnstrict(const QString& ip1, const QString& ip2)
{
    QStringList octets1 = ip1.split('.', QString::SkipEmptyParts);
    QStringList octets2 = ip2.split('.', QString::SkipEmptyParts);

    if(octets1.size() != octets2.size())
        return false;

    for(int i = 0; i < octets1.size(); ++i)
    {
        if(octets1[i] != octets2[i] &&
                octets1[i] != "*" &&
                octets2[i] != "*")
            return false;
    }
    return true;
}

NetworkDataSource::NetworkDataSource(QObject *parent) :
    IDatasource(parent), mState(DEFAULT), mSocket(0)
{
}

IConnection *NetworkDataSource::dataConnection() const
{
    return mSocket;
}

const QString &NetworkDataSource::id() const
{
    return mID;
}

const QString &NetworkDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState NetworkDataSource::state() const
{
    return mState;
}

const QString &NetworkDataSource::networkName() const
{
    return mText;
}

const QStringList &NetworkDataSource::availableNetworks() const
{
    return mAvailableNetworks;
}

const QObject *NetworkDataSource::item(const QString &key)
{
    if(key.isEmpty())
        return this;

    if(!mItems.contains(key))
    {
        NetworkDataSourceItem* item = new NetworkDataSourceItem(this);
        item->setID(key);
        mItems.insert(key, item);
    }
    return mItems[key];
}

const QMetaObject *NetworkDataSource::metaItem(const QString &key)
{
    return item(key)->metaObject();
}

void NetworkDataSource::setDefaut(const QString &key, const QString &defaut)
{
    if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        mItems.value(key)->setDefaut(defaut);
    }
}

void NetworkDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

void NetworkDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Network DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(state));
}

void NetworkDataSource::setNetworkName(const QString &name)
{
    emit networkChanged(QVariant::fromValue(name));
}

void NetworkDataSource::setAvailableNetworks(const QStringList &networks)
{
    mAvailableNetworks = networks;
    emit availableNetworksChanged(QVariant::fromValue(networks.join("\n")));
}

void NetworkDataSource::onNetStatusChanged()
{
    if(!mSocket)
        return;

    setAvailableNetworks(mSocket->availableNets());

    foreach(NetworkDataSourceItem* item, mItems.values())
    {
        QRegExp rx("((\\d{1,3}|\\*)\\.){3}(\\d{1,3}|\\*)");
        item->setState(UNREACHABLE);
        foreach (const QString& network, mAvailableNetworks)
        {
            if(network.indexOf(rx) != -1)
            {
                if(rx.captureCount() &&
                        compareIPUnstrict(rx.capturedTexts().first(), item->id()))
                {
                    if(item->networkName() != network)
                        item->setNetworkName(network);
                    item->setState(OK);
                }
            }
        }
    }
}

void NetworkDataSource::onSocketStateChanged()
{
    if(!mSocket)
        return;

    if(mSocket->isConnected())
    {
        //setState(OK);
        foreach(NetworkDataSourceItem* item, mItems)
            item->setState(OK_WARNING);
    }
    else
    {
        SupervisorWidgetState state;
        if ( !mSocket->isStatusOn() )
            state = OUT_OF_SCOPE;
        else
            state = UNREACHABLE;

        setAvailableNetworks(QStringList());
        foreach(NetworkDataSourceItem* item, mItems)
        {
            item->setState(state);
            QString defaut = item->getDefaut();
            if(defaut.size() != 0)
                item->setNetworkName(item->getDefaut());
            else
                item->setNetworkName("");
        }

        setState(state);
    }
}

void NetworkDataSource::onVariantRecieved(QVariant v)
{
    // Nets changed
    if ( v.canConvert<SupervisorProto::NetList>() )
        onNetStatusChanged();
}

void NetworkDataSource::setDataConnection(IConnection *socket)
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

        setNetworkName(mSocket->name());
    }
    else
    {
        mSocket = 0;
        setNetworkName("-");
    }
}

void NetworkDataSource::setID(const QString& id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void NetworkDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}


void NetworkDataSource::bind()
{
    foreach(NetworkDataSourceItem* item, mItems)
        item->bind();

    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
    setAvailableNetworks(availableNetworks());
    if(mSocket)
        emit networkChanged(QVariant::fromValue(mSocket->name()));
    else
        emit networkChanged(QVariant::fromValue(QString("")));
}


NetworkDataSourceItem::NetworkDataSourceItem(QObject *parent)
    : QObject(parent), mState(DEFAULT)
{
}

const QString &NetworkDataSourceItem::id() const
{
    return mID;
}

SupervisorWidgetState NetworkDataSourceItem::state() const
{
    return mState;
}

const QString &NetworkDataSourceItem::networkName() const
{
    return mNetworkName;
}

void NetworkDataSourceItem::setDefaut(const QString &defaut)
{
    mDefaut = defaut;
}

QString NetworkDataSourceItem::getDefaut()
{
    return mDefaut;
}

void NetworkDataSourceItem::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit stateChanged(QVariant::fromValue(mState));
    emit networkChanged(QVariant::fromValue(mNetworkName));
}

void NetworkDataSourceItem::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void NetworkDataSourceItem::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Network DataSource item '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void NetworkDataSourceItem::setNetworkName(const QString &name)
{
    mNetworkName = name;
    emit networkChanged(QVariant::fromValue(mNetworkName));
}
