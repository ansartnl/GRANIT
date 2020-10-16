#include "Asterix253MORLIDataSource.h"
#include "Log.h"

Asterix253MORLIDataSource::Asterix253MORLIDataSource(QObject *parent) :
    IDatasource(parent), mState(DEFAULT), mConnection(0)
{
}

const QString &Asterix253MORLIDataSource::id() const
{
    return mID;
}

const QString &Asterix253MORLIDataSource::connectionID() const
{
    return mConnectionID;
}

SupervisorWidgetState Asterix253MORLIDataSource::state() const
{
    return mState;
}

const QString &Asterix253MORLIDataSource::name() const
{
    return mName;
}

const QObject *Asterix253MORLIDataSource::item(const QString &key)
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
        Asterix253MORLIDataSourceItem* item = new Asterix253MORLIDataSourceItem(this);
        item->setID(QString("%1.%2").arg(index.sac).arg(index.sic));
        mItems.insert(index, item);
    }
    return mItems[index];
}

const QMetaObject *Asterix253MORLIDataSource::metaItem(const QString &key)
{
    if(item(key))
        return item(key)->metaObject();
    else
        return 0;
}

void Asterix253MORLIDataSource::setDefaut(const QString &key, const QString &defaut)
{
    /*if(key.isEmpty())
        return;

    if(mItems.contains(key))
    {
        mItems.value(key)->setDefaut(defaut);
    }*/
}

void Asterix253MORLIDataSource::setDatafield(const QString &key, const QString &datafield)
{

}

IConnection *Asterix253MORLIDataSource::dataConnection() const
{
    return mConnection;
}

void Asterix253MORLIDataSource::addReadableName(const QString &datakey, const QString &name)
{
    QRegExp rexp("(\\d+)[,. ]+(\\d+)");
    if (!rexp.exactMatch(datakey))
        return;
    const QPair < int, int > key(rexp.cap(1).toInt(), rexp.cap(2).toInt());
    if (mReadableNames.contains(key))
        return;
    mReadableNames.insert(key, name);
}

void Asterix253MORLIDataSource::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
    emit stateChanged(QVariant::fromValue(mState));
    emit nameChanged(QVariant::fromValue(mName));
}

void Asterix253MORLIDataSource::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void Asterix253MORLIDataSource::setConnectionID(const QString &id)
{
    mConnectionID = id;
    emit connectionIDChanged(QVariant::fromValue(mConnectionID));
}

void Asterix253MORLIDataSource::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Asterix253 MORLI DataSource '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void Asterix253MORLIDataSource::setName(const QString &name)
{
    mName = name;
    emit nameChanged(QVariant::fromValue(mName));
}

void Asterix253MORLIDataSource::setDataConnection(IConnection *socket)
{
    if(mConnection)
        disconnect(mConnection);

    if(dynamic_cast<asterix::AsterixListener253*>(socket))
    {
        mConnection = dynamic_cast<asterix::AsterixListener253*>(socket);
        connect(mConnection, SIGNAL(asterix253Data(QVariant)), this, SLOT(onAsterix253Data(QVariant)));
        connect(mConnection, SIGNAL(isIdle()), this, SLOT(onAsterixIdle()));

        setName(mConnection->id());
        setState(OK);

        mConnection->Start();
    }
    else
    {
        setState(DISABLED);
        mConnection = 0;
        setName("-");
    }
}

const QString Asterix253MORLIDataSource::readableName(int sac, int sic) const
{
    const QPair < int, int > key(sac, sic);
    if (mReadableNames.contains(key))
        return mReadableNames[key];
    return QString("%1, %2").arg(sac).arg(sic);
}

void Asterix253MORLIDataSource::onAsterix253Data(const QVariant &data)
{
    if(data.canConvert<Asterix253Struct>())
    {
        setState(OK);
        Asterix253Struct msg = data.value<Asterix253Struct>();
        if(mItems.contains(msg.dataSource))
        {
            mItems[msg.dataSource]->setSic(msg.dataSource.sic);
            mItems[msg.dataSource]->setSac(msg.dataSource.sac);

            SupervisorWidgetState newState;
            switch (msg.state) {
            case Asterix253Struct::NotSet:
                newState = NOT_SET;
                break;
            case Asterix253Struct::Master:
                newState = MASTER;
                break;
            case Asterix253Struct::Slave:
                newState = SLAVE;
                break;
            case Asterix253Struct::Undefined:
                newState = NOT_INITIATED;
                break;
            default:
                newState = UNREACHABLE;
                break;
            }

            if(newState != mItems[msg.dataSource]->state())
                emit eventToLog(mConnection->id(),
                                QString("<%1> MORLI %2").arg(readableName(msg.dataSource.sac, msg.dataSource.sic)).arg(msg.serviceIdentifier),
                                QString("State changed to '%1'").arg( Asterix253Tools::stateAsString(msg.state) ));
            mItems[msg.dataSource]->setState(newState);
        }
    }
}

void Asterix253MORLIDataSource::onAsterixIdle()
{
    setState(DEFAULT);
    foreach(Asterix253MORLIDataSourceItem* item, mItems)
    {
        item->setState(DEFAULT);
    }
}


Asterix253MORLIDataSourceItem::Asterix253MORLIDataSourceItem(QObject *parent) :
    QObject(parent), mState(DEFAULT), mSic(0), mSac(0)
{

}

const QString &Asterix253MORLIDataSourceItem::id() const
{
    return mID;
}

SupervisorWidgetState Asterix253MORLIDataSourceItem::state() const
{
    return mState;
}

int Asterix253MORLIDataSourceItem::sic() const
{
    return mSic;
}

int Asterix253MORLIDataSourceItem::sac() const
{
    return mSac;
}

void Asterix253MORLIDataSourceItem::setDefaut(const QString &defaut)
{
    mDefaut = defaut;
}

QString Asterix253MORLIDataSourceItem::getDefaut()
{
    return mDefaut;
}

void Asterix253MORLIDataSourceItem::bind()
{
    emit idChanged(QVariant::fromValue(mID));
    emit stateChanged(QVariant::fromValue(mState));
    emit sicChanged(QVariant::fromValue(mSic));
    emit sacChanged(QVariant::fromValue(mSac));
}

void Asterix253MORLIDataSourceItem::setID(const QString &id)
{
    mID = id;
    emit idChanged(QVariant::fromValue(mID));
}

void Asterix253MORLIDataSourceItem::setState(SupervisorWidgetState state)
{
    if(mState != state)
        qLogInfo(tr("Event")) << "Asterix253 MORLI DataSource item '" << mID << "' changed state to: '" << widgetStateAsString(state) << "'";

    mState = state;
    emit stateChanged(QVariant::fromValue(mState));
}

void Asterix253MORLIDataSourceItem::setSic(int sic)
{
    mSic = sic;
    emit sicChanged(QVariant::fromValue(mSic));
}

void Asterix253MORLIDataSourceItem::setSac(int sac)
{
    mSac = sac;
    emit sacChanged(QVariant::fromValue(mSac));
}
