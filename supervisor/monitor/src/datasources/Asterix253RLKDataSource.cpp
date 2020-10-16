#include "Asterix253RLKDataSource.h"
#include "Log.h"
//#include "src/connections/asterix253/Asterix253Struct.h"

Asterix253RLKDataSource::Asterix253RLKDataSource(QObject *parent) :
    IDatasource(parent), mState(DEFAULT)
{
}

const QString &Asterix253RLKDataSource::id() const
{
    return mID;
}

const QString &Asterix253RLKDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState Asterix253RLKDataSource::state() const
{
    return mState;
}

const QString &Asterix253RLKDataSource::name() const
{
    return mName;
}

const QObject *Asterix253RLKDataSource::item(const QString &key)
{
    if(key.isEmpty())
        return this;

    QStringList list = key.split(QRegExp("[,. ]"), QString::SkipEmptyParts);

    if(list.size() != 2)
        return 0;

    bool ok1(false);
    bool ok2(false);
    Asterix253Struct::Source index;
    index.sac = list[0].toUInt(&ok1);
    index.sic = list[1].toUInt(&ok2);
    if(!ok1 || !ok2)
        return 0;

    if(!mItems.contains(index))
    {
        Asterix253RLKDataSourceItem* item = new Asterix253RLKDataSourceItem(this);
        item->setID(QString("%1.%2").arg(index.sac).arg(index.sic));
        mItems.insert(index, item);
    }
    return mItems[index];
}

const QMetaObject *Asterix253RLKDataSource::metaItem(const QString &key)
{
    if(item(key))
        return item(key)->metaObject();
    else
        return 0;
}

void Asterix253RLKDataSource::setDefaut(const QString &key, const QString &defaut)
{
    /*if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        mItems.value(key)->setDefaut(defaut);
    }*/
}

void Asterix253RLKDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

IConnection *Asterix253RLKDataSource::dataConnection() const
{
    return mConnection;
}

void Asterix253RLKDataSource::addReadableName(const QString &datakey, const QString &name)
{
    QRegExp rexp("(\\d+)[,. ]+(\\d+)");
    if (!rexp.exactMatch(datakey))
        return;
    const QPair < int, int > key(rexp.cap(1).toInt(), rexp.cap(2).toInt());
    if (mReadableNames.contains(key))
        return;
    mReadableNames.insert(key, name);
}

void Asterix253RLKDataSource::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
    emit nameChanged(QVariant::fromValue(mName));
    foreach(Asterix253RLKDataSourceItem* item, mItems)
        item->bind();
}

void Asterix253RLKDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void Asterix253RLKDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void Asterix253RLKDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Asterix253 RLK DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void Asterix253RLKDataSource::setName(const QString &name)
{
    mName = name;
    emit nameChanged(QVariant::fromValue(mName));
}

void Asterix253RLKDataSource::setDataConnection(IConnection *socket)
{
    if(mConnection)
        disconnect(mConnection);

    if(dynamic_cast<asterix::AsterixListener253*>(socket))
    {
        mConnection = dynamic_cast<asterix::AsterixListener253*>(socket);
        connect(mConnection, SIGNAL(asterix253Data(QVariant)), this, SLOT(onAsterix253Data(QVariant)));
        connect(mConnection, SIGNAL(isIdle()), this, SLOT(onAsterixIdle()));

        setName(mConnection->id());

        mConnection->Start();
        setState(OK);
    }
    else
    {
        mConnection = 0;
        setName("-");
        setState(DISABLED);
    }
}

const QString Asterix253RLKDataSource::readableName(int sac, int sic) const
{
    const QPair < int, int > key(sac, sic);
    if (mReadableNames.contains(key))
        return mReadableNames[key];
    return QString("%1, %2").arg(sac).arg(sic);
}

void Asterix253RLKDataSource::onAsterix253Data(const QVariant &data)
{
    if(data.canConvert<Asterix253Struct>())
    {
        setState(OK);
        Asterix253Struct msg = data.value<Asterix253Struct>();

        foreach (const Asterix253Struct::ChannelState& state, msg.channelState)
        {
            if(!mItems.contains(state.dataSource))
                continue;

            mItems[state.dataSource]->setSic(state.dataSource.sic);
            mItems[state.dataSource]->setSac(state.dataSource.sac);

            SupervisorWidgetState newState;
            switch(state.first)
            {
            case Asterix253Struct::ChannelState::Valid:
                newState = VALID;
                break;
            case Asterix253Struct::ChannelState::NotConnected:
                newState = NOT_CONNECTED;
                break;
            case Asterix253Struct::ChannelState::NotInitiated:
                newState = NOT_INITIATED;
                break;
            case Asterix253Struct::ChannelState::Disabled:
                newState = DISABLED;
                break;
            default:
                newState = DEFAULT;
                break;
            }

            if(newState != mItems[state.dataSource]->firstState())
                emit eventToLog(mConnection->id(),
                                QString("<%1> Channel 1").arg(readableName(state.dataSource.sac, state.dataSource.sic)),
                                QString("State changed to '%1'").arg( Asterix253Tools::stateAsString(state.first) ));
            mItems[state.dataSource]->setFirstState(newState);

            switch(state.second)
            {
            case Asterix253Struct::ChannelState::Valid:
                newState = VALID;
                break;
            case Asterix253Struct::ChannelState::NotConnected:
                newState = NOT_CONNECTED;
                break;
            case Asterix253Struct::ChannelState::NotInitiated:
                newState = NOT_INITIATED;
                break;
            case Asterix253Struct::ChannelState::Disabled:
                newState = DISABLED;
                break;
            default:
                newState = DEFAULT;
                break;
            }

            if(newState != mItems[state.dataSource]->secondState())
                emit eventToLog(mConnection->id(),
                                QString("<%1> Channel 2").arg(readableName(state.dataSource.sac, state.dataSource.sic)),
                                QString("State changed to '%1'").arg( Asterix253Tools::stateAsString(state.second) ));
            mItems[state.dataSource]->setSecondState(newState);
        }
    }
}

void Asterix253RLKDataSource::onAsterixIdle()
{
    setState(DEFAULT);
    foreach(Asterix253RLKDataSourceItem* item, mItems)
    {
        item->setFirstState(DEFAULT);
        item->setSecondState(DEFAULT);
    }
}


Asterix253RLKDataSourceItem::Asterix253RLKDataSourceItem(QObject *parent) :
    QObject(parent), mFirstState(DEFAULT), mSecondState(DEFAULT), mSic(0), mSac(0)
{

}

const QString &Asterix253RLKDataSourceItem::id() const
{
    return mID;
}

SupervisorWidgetState Asterix253RLKDataSourceItem::firstState() const
{
    return mFirstState;
}

SupervisorWidgetState Asterix253RLKDataSourceItem::secondState() const
{
    return mSecondState;
}

int Asterix253RLKDataSourceItem::sic() const
{
    return mSic;
}

int Asterix253RLKDataSourceItem::sac() const
{
    return mSac;
}

void Asterix253RLKDataSourceItem::setDefaut(const QString &defaut)
{
    mDefaut = defaut;
}

QString Asterix253RLKDataSourceItem::getDefaut()
{
    return mDefaut;
}

void Asterix253RLKDataSourceItem::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit firstStateChanged(QVariant::fromValue(mFirstState));
    emit secondStateChanged(QVariant::fromValue(mSecondState));
    emit sicChanged(QVariant::fromValue(mSic));
    emit sacChanged(QVariant::fromValue(mSac));
}

void Asterix253RLKDataSourceItem::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void Asterix253RLKDataSourceItem::setFirstState(SupervisorWidgetState state)
{
    if(mFirstState != state)
        qLogInfo(tr("Event")) << "Asterix253 RLK DataSource item '" << mID << "' changed first state to: '" << widgetStateAsString(state) << "'";

    mFirstState = state;
    emit firstStateChanged(QVariant::fromValue(mFirstState));
}

void Asterix253RLKDataSourceItem::setSecondState(SupervisorWidgetState state)
{
    if(mSecondState != state)
        qLogInfo(tr("Event")) << "Asterix253 RLK DataSource item '" << mID << "' changed second state to: '" << widgetStateAsString(state) << "'";

    mSecondState = state;
    emit secondStateChanged(QVariant::fromValue(mSecondState));
}

void Asterix253RLKDataSourceItem::setSic(int sic)
{
    mSic = sic;
    emit sicChanged(QVariant::fromValue(mSic));
}

void Asterix253RLKDataSourceItem::setSac(int sac)
{
    mSac = sac;
    emit sacChanged(QVariant::fromValue(mSac));
}
