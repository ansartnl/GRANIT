#ifndef EVENTLOGMODEL_H
#define EVENTLOGMODEL_H

#include <QSqlQueryModel>

class EventLogModel : public QSqlQueryModel
{
    Q_OBJECT
    static const int COLUMN_COUNT = 4;
    static const QString QUERY;
    static const QString COLOR_FIELD;

public:
    enum ColumnType { TIME = 0, HOST = 1, SERVICE = 2, EVENT = 3 };

    explicit EventLogModel(QObject *parent = 0);

    void select(const QSqlDatabase &db);

    //! Inherited from QAbstractTableModel
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QVariant itemForeground(const QModelIndex &index) const;
};

#endif // EVENTLOGMODEL_H
