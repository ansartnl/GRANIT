#include <QColor>

#include "EventLogModel.h"
#include "EventLogFilterModel.h"

EventLogFilterModel::EventLogFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent), mFailureOnly(false)
{
    setDynamicSortFilter(true);
}

Qt::ItemFlags EventLogFilterModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void EventLogFilterModel::setFailureFilter(bool isOn)
{
    mFailureOnly = isOn;
    invalidate();
}

void EventLogFilterModel::setTimeFrom(const QDate &from)
{
    mTimeFromFilter = from;
    invalidate();
}

void EventLogFilterModel::setTimeTo(const QDate &to)
{
    mTimeToFilter = to;
    invalidate();
}

void EventLogFilterModel::setHostFilter(const QString &host)
{
    mHostFilter = host;
    invalidate();
}

void EventLogFilterModel::setServiceFiler(const QString &service)
{
    mServiceFilter = service;
    invalidate();
}

void EventLogFilterModel::setEventFilter(const QString &event)
{
    mEventFilter = event;
    invalidate();
}

bool EventLogFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)

    auto columnData = [this, source_row](int column, int role = Qt::DisplayRole) {
        QModelIndex index = sourceModel()->index(source_row, column);
        return sourceModel()->data(index, role).toString();
    };

    if (mFailureOnly)
    {
        QColor color = QColor(columnData(0, Qt::ForegroundRole));
        return (color == Qt::red);
    }
    else
    {
        bool rc = true;

        if(!mHostFilter.isEmpty() && !columnData(EventLogModel::HOST).contains(mHostFilter))
            rc = false;
        if(!mServiceFilter.isEmpty() && !columnData(EventLogModel::SERVICE).contains(mServiceFilter))
            rc = false;
        if(!mEventFilter.isEmpty() && !columnData(EventLogModel::EVENT).contains(mEventFilter))
            rc = false;

        return rc;
    }

    return true;
}
