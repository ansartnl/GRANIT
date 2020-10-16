#ifndef _HOSTS_MODEL_H__
#define _HOSTS_MODEL_H__

#include <QtCore/QAbstractItemModel>
#include <QtNetwork/QAbstractSocket>

class MonitoringSocket;
class AppSettings;

//! Model for representing list of available hosts.
class HostsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    typedef QList<MonitoringSocket*>    THostList;

    explicit HostsModel(const THostList hosts, const AppSettings* appsettings, QObject *parent = 0);

    //! Returns the data stored under the given role for the item referred to by the index.
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //! Returns the data for the given role and section in the header with the specified orientation.
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    //! Returns the item flags for the given index.
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Returns the index of the item in the model specified by the given row, column and parent index.
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    //! Returns the parent of the model item with the given index. If the item has no parent, an invalid QModelIndex is returned.
    virtual QModelIndex parent(const QModelIndex &child) const;

    /*! Returns the number of rows under the given parent. When the parent is valid it
        means that rowCount is returning the number of children of parent.
    */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //! Returns the number of columns for the children of the given parent.
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Redraw view.
    void update();

private slots:
    void on_variantReceived(QVariant);
    void on_disconnected();

private:
    /** Parent connections */
    THostList       mHosts;
    const AppSettings*    mAppSettings;
};

#endif // _HOSTS_MODEL_H__
