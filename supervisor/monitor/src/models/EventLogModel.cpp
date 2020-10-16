#include <QColor>
#include <QSqlRecord>

#include "EventLogModel.h"

const QString EventLogModel::QUERY = "select DATE, HOST, SERVICE, MESSAGE, COLOR from LOG order by ID asc";
const QString EventLogModel::COLOR_FIELD = "COLOR";

EventLogModel::EventLogModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void EventLogModel::select(const QSqlDatabase &db)
{
    clear();

    if (db.isOpen())
    {
        setQuery(QUERY, db);

        // get all data to avoid sqlite lock
        while (canFetchMore())
        {
            fetchMore();
        }
    }
}

Qt::ItemFlags EventLogModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int EventLogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

QVariant EventLogModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ForegroundRole)
    {
        return itemForeground(index);
    }

    return QSqlQueryModel::data(index, role);
}

QVariant EventLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section >= 0 && section < COLUMN_COUNT)
        {
            switch(section)
            {
            case TIME:
                return tr("Time");
            case HOST:
                return tr("Host");
            case SERVICE:
                return tr("Service");
            case EVENT:
                return tr("Event");
            default:
                return QVariant();
            }
        }
    }

    return QSqlQueryModel::headerData(section, orientation, role);
}

QVariant EventLogModel::itemForeground(const QModelIndex &index) const
{
    return QColor(record(index.row()).value(COLOR_FIELD).toString());
}

