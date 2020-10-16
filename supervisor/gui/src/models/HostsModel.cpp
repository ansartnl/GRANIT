#include "HostsModel.h"
#include "MonitoringSocket.h"
#include "SpProto.h"
#include "AppSettings.h"

#include <QtGui/QIcon>
#include <QtGui/QFont>
#include <QtGui/QApplication>

// Column indexes
static const int gIndexName = 0;
static const int gIndexStatus = 1;

static QIcon& iconServer()
{
    static QIcon g_iconServer(QString::fromUtf8(":/images/NetworkServer"));
    return g_iconServer;
}
static QIcon& iconServerInvalid()
{
    static QIcon g_iconServerInvalid(QString::fromUtf8(":/images/NetworkServerInvalid"));
    return g_iconServerInvalid;
}
static QIcon& iconServerOff()
{
    static QIcon g_iconServerOff(QString::fromUtf8(":/images/NetworkServerOff"));
    return g_iconServerOff;
}


HostsModel::HostsModel(const THostList hosts, const AppSettings* appsettings, QObject *parent)
    : QAbstractItemModel(parent), mHosts(hosts), mAppSettings(appsettings)
{
    for ( THostList::iterator it = mHosts.begin(); it != mHosts.end(); ++it )
    {
        MonitoringSocket *socket = *it;
        connect(socket, SIGNAL(newVariantMessage(QVariant)), SLOT(on_variantReceived(QVariant)));
        connect(socket, SIGNAL(socketDisconnected()), SLOT(on_disconnected()));
//        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(onStateChanged(QAbstractSocket::SocketState)));
    }
}

QModelIndex HostsModel::index(int row, int column,
                               const QModelIndex &parent) const
{
    if ( row >= mHosts.size() )
        return QModelIndex();

    return createIndex(row, column, reinterpret_cast<void *>(mHosts.at(row)));
}

QModelIndex HostsModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Qt::ItemFlags HostsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int HostsModel::rowCount(const QModelIndex &parent) const
{
    return mHosts.size();
}

int HostsModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant HostsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    int column = index.column();

    Q_ASSERT( column >= 0 && column < columnCount() );
    Q_ASSERT( index.internalPointer() );

    MonitoringSocket *socket = static_cast<MonitoringSocket *>(index.internalPointer());

    if ( role == Qt::DisplayRole )
    {
        if (column == gIndexName)
            return socket->name().isEmpty() ? socket->id() : socket->name();
        else if (column == gIndexStatus)
        {
            QString statusMessage;
            if ( !socket->isStatusOn() )
                statusMessage = tr("Status Off");
            else
            {
                int activeIndex = socket->findGroupIndex(socket->activeGroup());
                if(activeIndex != -1)
                    statusMessage = socket->group(activeIndex).name;
            }

            MonitoredObject *obj = socket->cwp();
            if (obj)
            {
                QString sectors = obj->cwpSectors.join("/");
                if(!sectors.isEmpty())
                {
                    statusMessage += statusMessage.isEmpty() ? QString() : QString(", ");
                    statusMessage += sectors;
                }
            }

            return statusMessage;

        }
    }
    if ( role == Qt::DecorationRole )
    {
        if (column == gIndexName)
        {
            if ( !socket->isConnected() )
                return iconServerOff();
            else if ( !socket->isValid() )
                return iconServerInvalid();
            return iconServer();
        } else
        {
            return QVariant();
        }
    }
    else if ( role == Qt::FontRole )
    {
        if ( !socket->isConnected() || !socket->isValid() )
        {
            QFont fontBold = qApp->font();
            fontBold.setBold(true);
            return fontBold;
        }
    }
    else if ( role == Qt::ForegroundRole )
    {
        if ( !socket->isStatusOn() )
            return mAppSettings ? mAppSettings->getColor(AppSettings::INIT) : Qt::darkGray;
        else if ( !socket->isConnected() )
            return mAppSettings ? mAppSettings->getColor(AppSettings::NOTWORK) : Qt::red;
        else if ( !socket->isValid() )
        {
            if ( socket->isModify() )
                return mAppSettings ? mAppSettings->getColor(AppSettings::MODIFY) : QColor::fromRgb(255, 0, 255);
            else
                return mAppSettings ? mAppSettings->getColor(AppSettings::PARTWORK) : QColor::fromRgb(255, 128, 0);
        }

        return mAppSettings ? mAppSettings->getColor(AppSettings::WORK) : Qt::black;
    }
    else if ( role == Qt::ToolTipRole )
    {
        return tr("Host: '%1' Host2: '%2' Port: %3")
                .arg(socket->host())
                .arg(socket->host2().isEmpty() ? tr("No") : socket->host2())
                .arg(socket->port());
    }

    return QVariant();
}

QVariant HostsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Vertical )
        return QVariant();
    if ( role == Qt::DisplayRole )
    {
        if (section == gIndexName)
            return tr("Name");
        else if (section == gIndexStatus)
            return tr("Status");
    }

    return QVariant();
}

void HostsModel::update()
{
    emit layoutChanged();
}

void HostsModel::on_variantReceived(QVariant v)
{
    // Services information received
    // or New status received
    if ( v.canConvert<SupervisorProto::Services>() ||
         v.canConvert<SupervisorProto::Status>() )
        update();
}

void HostsModel::on_disconnected()
{
    update();
}
