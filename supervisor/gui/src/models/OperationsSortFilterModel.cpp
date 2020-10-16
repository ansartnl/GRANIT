#include "OperationsSortFilterModel.h"

COperationsSortFilterModel::COperationsSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    enabled(false)
{
}

void COperationsSortFilterModel::setFilterMinimumDate(const QDateTime &date)
{
    minDate = date;
    invalidateFilter();
}

void COperationsSortFilterModel::setFilterMaximumDate(const QDateTime &date)
{
    maxDate = date;
    invalidateFilter();
}

void COperationsSortFilterModel::setFilterCODE(const QString &value)
{
    CODE = value;
    invalidateFilter();
}

void COperationsSortFilterModel::setFilterOperator(const QString &value)
{
    oper = value;
    invalidateFilter();
}

//void COperationsSortFilterModel::setEnableFiltering(bool enable)


bool COperationsSortFilterModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    if(!enabled)
        return true;

    QModelIndex index_date = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index_oper = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index_CODE = sourceModel()->index(sourceRow, 2, sourceParent);

    return (   (sourceModel()->data(index_oper).toString().contains(oper) ||
                oper == tr("<All>") )
            && sourceModel()->data(index_CODE).toString().contains(CODE)
            && dateInRange(sourceModel()->data(index_date).toDateTime()));
}

bool COperationsSortFilterModel::dateInRange(const QDateTime &date) const
{
    return (!minDate.isValid() || date > minDate.toUTC())
            && (!maxDate.isValid() || date < maxDate.toUTC());
}
