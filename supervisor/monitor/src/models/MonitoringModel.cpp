#include "MonitoringModel.h"
#include "src/connections/supervisorServer/MonitoringSocket.h"
#include "SpProto.h"

#include <QtGui/QIcon>

// Column indexes
static const int gIndexName = 0;
static const int gIndexStatus = 1;

static QIcon& iconCWP()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/CWP"));
    return g_iconServer;
}
static QIcon& iconFDP()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/FDP"));
    return g_iconServer;
}
static QIcon& iconDBSYNC()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/DBSYNC"));
    return g_iconServer;
}
static QIcon& iconFMTP()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/FMTP"));
    return g_iconServer;
}
static QIcon& iconSTCA()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/STCA"));
    return g_iconServer;
}
static QIcon& iconAFTNWS()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/AFTNWS"));
    return g_iconServer;
}
static QIcon& iconProcess()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/Process"));
    return g_iconServer;
}
static QIcon& iconService()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/Service"));
    return g_iconServer;
}


MonitoringModel::MonitoringModel(QObject *parent)
    : QAbstractItemModel(parent), mMonHost(0)
{
}

void MonitoringModel::setSocket(MonitoringSocket *socket)
{
    if ( mMonHost == socket )
        return;

    if ( mMonHost )
        disconnect(mMonHost, 0, this, 0);

    mMonHost = socket;
    if ( mMonHost )
    {
        connect(mMonHost, SIGNAL(newVariantMessage(QVariant)), SLOT(on_variantReceived(QVariant)));
        connect(mMonHost, SIGNAL(socketDisconnected()), SLOT(on_disconnected()));
    }
}

QModelIndex MonitoringModel::index(int row, int column,
                                   const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    if ( !parent.isValid() && mMonHost )
    {
        if ( row >= 0 && row < mMonHost->countObjects() )
            return createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex MonitoringModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

Qt::ItemFlags MonitoringModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int MonitoringModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid() && mMonHost )
        return mMonHost->countObjects();
    return 0;
}

int MonitoringModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant MonitoringModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || !mMonHost )
        return QVariant();

    int column = index.column(),
            row = index.row();

    Q_ASSERT( column >= 0 && column < columnCount() );

    const MonitoredObject &object = mMonHost->object( row );

    if ( role == Qt::DisplayRole )
    {
        if ( column == gIndexName )
            return object.visualName();
        else if ( column == gIndexStatus )
        {
            QString statusMessage;
            if ( !object.isOn )
                statusMessage = tr("Status Off") + QString(", ");

            if (object.status == MonitoredObject::Started)
                statusMessage += tr("Started") + additionalStatus(object);
            else
                statusMessage += tr("Stopped");
            return statusMessage;
        }
    }
    else if ( role == Qt::ForegroundRole )
    {
        if ( !object.isOn )
            return QColor(Qt::darkGray);
        else
        {
            if ( MonitoredObject::Started != object.status )
                return QColor(Qt::red);
            if ( MonitoredObject::FMTP == object.type)
            {
                if (object.additionalStatus != QString("%1").arg(SupervisorProto::toString(SupervisorProto::enDataReady)))
                    return QColor(Qt::red);
            }
        }
    }
    else if ( role == Qt::DecorationRole )
    {
        if ( 0 == column )
        {
            if ( MonitoredObject::PROCESS == object.type )
                return iconProcess();
            else if ( MonitoredObject::SERVICE == object.type )
                return iconService();
            else if ( MonitoredObject::CWP == object.type )
                return iconCWP();
            if ( MonitoredObject::AFTNWS == object.type )
                return iconAFTNWS();
            else if ( MonitoredObject::FDP == object.type )
                return iconFDP();
            else if ( MonitoredObject::DBSYNC == object.type )
                return iconDBSYNC();
            else if ( MonitoredObject::FMTP == object.type )
                return iconFMTP();
            else if ( MonitoredObject::STCA == object.type )
                return iconSTCA();
        }
    }
    else if ( role == Qt::TextAlignmentRole )
    {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::ToolTipRole )
    {
        return object.description;
    }

    return QVariant();
}

QVariant MonitoringModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if ( orientation == Qt::Vertical )
        return QVariant();
    if ( role == Qt::DisplayRole )
    {
        if ( section == gIndexName )
            return tr("Name");
        else if ( section == gIndexStatus )
            return tr("Status");
    }

    return QVariant();
}

void MonitoringModel::update()
{
    emit layoutChanged();
}

QString MonitoringModel::additionalStatus(const MonitoredObject &obj) const
{
    int c = 0;
    if (!obj.additionalStatus.isEmpty())
        c += 1;
    if (!obj.user.isEmpty())
        c += 2;
    switch (c)
    {
    case 0:
        // no addStatus, no user
        return QString();
    case 1:
        // addStatus, no user
        return tr(" (%1)").arg(obj.additionalStatus);
    case 2:
        // user, no addStatus
        return tr(" (User: %1)").arg(obj.user);
    case 3:
        // user and addStatus
        return tr(" (%1, User: %2)").arg(obj.additionalStatus).arg(obj.user);
    }
    return QString();
}

void MonitoringModel::on_variantReceived(const QVariant &v)
{
    if ( !mMonHost )
        return;

    // Services information received
    if ( v.canConvert<SupervisorProto::Services>() )
        update();

    // New status received
    else if ( v.canConvert<SupervisorProto::Status>() )
    {
        SupervisorProto::Status s = v.value<SupervisorProto::Status>();
        int index = mMonHost->findIndex( MonitoredObject::Type(s.type), s.name );
        if ( index >= 0 && index < mMonHost->countObjects() )
            emit dataChanged(createIndex(index, 0), createIndex(index, 2));
    }
    else if ( v.canConvert<SupervisorProto::FMTPStatus>() )
    {
        SupervisorProto::FMTPStatus s = v.value<SupervisorProto::FMTPStatus>();
        int index = mMonHost->findIndex( MonitoredObject::FMTP, s.name );
        if ( index >= 0 && index < mMonHost->countObjects() )
            emit dataChanged(createIndex(index, 0), createIndex(index, 2));
    }
    else if ( v.canConvert<SupervisorProto::CWPSectors>() )
    {
        SupervisorProto::CWPSectors s = v.value<SupervisorProto::CWPSectors>();
        int index = mMonHost->findIndex( MonitoredObject::CWP, s.name );
        if ( index >= 0 && index < mMonHost->countObjects() )
            emit dataChanged(createIndex(index, 0), createIndex(index, 2));
    }
    else if ( v.canConvert<SupervisorProto::FDPStatus>() )
    {
        SupervisorProto::FDPStatus s = v.value<SupervisorProto::FDPStatus>();
        int index = mMonHost->findIndex( MonitoredObject::FDP, s.name );
        if ( index >= 0 && index < mMonHost->countObjects() )
            emit dataChanged(createIndex(index, 0), createIndex(index, 2));
    }
}

void MonitoringModel::on_disconnected()
{
    update();
}
