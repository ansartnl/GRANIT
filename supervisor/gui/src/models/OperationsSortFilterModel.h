#ifndef OPERATIONSSORTFILTERMODEL_H
#define OPERATIONSSORTFILTERMODEL_H

#include <QDateTime>
#include <QSortFilterProxyModel>

class COperationsSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit COperationsSortFilterModel(QObject *parent = 0);

    QDateTime filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDateTime &date);

    QDateTime filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDateTime &date);

    QString filterCODE() const { return CODE; }
    void setFilterCODE(const QString& value);

    QString filterOperator() const { return oper; }
    void setFilterOperator(const QString& value);

    void setEnableFiltering(bool enable)
    {
        enabled = enable;
        invalidateFilter();
    };

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    bool dateInRange(const QDateTime &date) const;

    QDateTime minDate;
    QDateTime maxDate;
    QString   CODE;
    QString   oper;
    bool      enabled;
};

#endif // OPERATIONSSORTFILTERMODEL_H
