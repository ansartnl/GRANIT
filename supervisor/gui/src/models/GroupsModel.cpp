#include "GroupsModel.h"
#include "MonitoringSocket.h"
#include "SpProto.h"

#include <QtGui/QFont>
#include <QtGui/QApplication>
#include <QtGui/QIcon>

static QIcon& iconListViewHeight()
{
    static QIcon g_iconListViewSize(QString::fromUtf8(":/images/ListViewHeight"));
    return g_iconListViewSize;
}


GroupsModel::GroupsModel(QObject *parent)
    : QAbstractItemModel(parent), mMonHost(0)
{
}

void GroupsModel::setSocket(MonitoringSocket *socket)
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

QModelIndex GroupsModel::index(int row, int column,
                                   const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    if ( !parent.isValid() && mMonHost )
    {
        if ( row >= 0 && row < mMonHost->countGroups() )
            return createIndex(row, column, 0);
    }

    return QModelIndex();
}

QModelIndex GroupsModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

Qt::ItemFlags GroupsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int GroupsModel::rowCount(const QModelIndex &parent) const
{
    if ( !parent.isValid() && mMonHost )
        return mMonHost->countGroups();
    return 0;
}

int GroupsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant GroupsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || !mMonHost )
        return QVariant();

    int column = index.column(),
            row = index.row();

    Q_ASSERT( column >= 0 && column < columnCount() );

    const Group &group = mMonHost->group( row );

    if ( role == Qt::DisplayRole )
    {
        if ( 0 == column )
            return group.visualName;
        else if ( 1 == column )
            return group.name == mMonHost->activeGroup() ? tr("Active") : QString();
    }
    else if ( role == Qt::UserRole )
    {
        if ( column == 0 )
            return group.name;
    }
    else if ( role == Qt::FontRole )
    {
        if ( group.name == mMonHost->activeGroup() )
        {
            QFont fontBold = qApp->font();
            fontBold.setBold(true);
            return fontBold;
        }
    }
    else if ( role == Qt::DecorationRole )
    {
        return iconListViewHeight();
    }
    else if ( role == Qt::ForegroundRole )
    {
        if ( group.name != mMonHost->activeGroup() )
            return Qt::darkGray;
    }

    return QVariant();
}

QVariant GroupsModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if ( orientation == Qt::Vertical )
        return QVariant();

    if ( role == Qt::DisplayRole )
    {
        if ( 0 == section )
            return tr("Name");
        else if ( 1 == section )
            return tr("Status");
    }

    return QVariant();
}

void GroupsModel::update()
{
    emit layoutChanged();
}

void GroupsModel::on_variantReceived(QVariant v)
{
    if ( !mMonHost )
        return;

    // Services information received
    if ( v.canConvert<SupervisorProto::Groups>() )
        update();

    // New active group received
    else if ( v.canConvert<SupervisorProto::ActiveGroup>() )
        update();
}

void GroupsModel::on_disconnected()
{
    update();
}
