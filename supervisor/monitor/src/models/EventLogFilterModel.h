#ifndef EVENTLOGFILTERMODEL_H
#define EVENTLOGFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>

class EventLogFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EventLogFilterModel(QObject *parent = 0);
    
    //! Inherited from QSortFilterProxyModel
    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void setFailureFilter(bool isOn);
    void setTimeFrom(const QDate& from);
    void setTimeTo(const QDate& to);
    void setHostFilter(const QString& host);
    void setServiceFiler(const QString& service);
    void setEventFilter(const QString& event);

protected:
    //@{
    //! Inherited from QSortFilterProxyModel
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    //@}
    
private:
    QDate mTimeFromFilter;
    QDate mTimeToFilter;
    QString mHostFilter;
    QString mServiceFilter;
    QString mEventFilter;
    bool mFailureOnly;
};

#endif // EVENTLOGFILTERMODEL_H
