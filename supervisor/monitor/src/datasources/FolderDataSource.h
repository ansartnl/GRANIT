#ifndef FOLDERDATASOURCE_H
#define FOLDERDATASOURCE_H

#include <QObject>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"

class IConnection;
class MonitoringSocket;
class FolderDataSourceItem;

class FolderDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(SupervisorWidgetState stateNew READ stateNew WRITE setStateNew NOTIFY stateChangedNew)
    typedef QHash<QString, FolderDataSourceItem*> TItemHash;
public:
    explicit FolderDataSource(QObject *parent = 0);

    IConnection* dataConnection() const;

    const QString& id() const;
    const QString& connectionID() const;
    SupervisorWidgetState state() const;
    SupervisorWidgetState stateNew() const;

    const QObject *item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

signals:    //! Properties
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void stateChanged(QVariant state);
    void stateChangedNew(QVariant state);

public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setStateNew(SupervisorWidgetState state);

    void setDataConnection(IConnection* socket);

private slots:
    void onNetStatusChanged();
    void onSocketStateChanged();
    void onVariantRecieved(QVariant v);

private:
    QString mID;
    QString mConnectionID;
    SupervisorWidgetState mState;
    QString mText;
    MonitoringSocket* mSocket;
    TItemHash mItems;
};


class FolderDataSourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(SupervisorWidgetState stateNew READ stateNew WRITE setStateNew NOTIFY stateChangedNew)
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)
    Q_PROPERTY(QString itemData READ itemData WRITE setItemDate NOTIFY itemNameChanged)
public:
    FolderDataSourceItem(const QString &connectionId, QObject* parent = 0);

    const QString& id() const;
    SupervisorWidgetState state() const;
    SupervisorWidgetState stateNew() const;
    const QString& itemName() const;
    const QString& itemData() const;
    void setDefaut(const QString& defaut);
    QString getDefaut();
    void setDatafield(const QString& datafield);
    QString getDatafield();

signals:    //! Properties
    void idChanged(QVariant id);
    void stateChanged(QVariant state);
    void stateChangedNew(QVariant state);
    void itemNameChanged(QVariant name);

public slots:
    void bind();
    void setID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setStateNew(SupervisorWidgetState state);
    void setItemName(const QString& name);
    void setItemDate(const QString& date);

private:
    QString mID;
    const QString mConnectionID;
    SupervisorWidgetState mState;
    QString mItemName;
    QString mDefaut;
    QString mDatafield;
};
#endif // FOLDERDATASOURCE_H
