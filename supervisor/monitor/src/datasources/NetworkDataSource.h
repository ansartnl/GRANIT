#ifndef NETWORKDATASOURCE_H
#define NETWORKDATASOURCE_H

#include <QObject>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"

class IConnection;
class MonitoringSocket;
class NetworkDataSourceItem;

class NetworkDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString networkName READ networkName WRITE setNetworkName NOTIFY networkChanged)
    Q_PROPERTY(QStringList availableNetworks READ availableNetworks WRITE setAvailableNetworks NOTIFY availableNetworksChanged)
    typedef QHash<QString, NetworkDataSourceItem*> TItemHash;
public:
    explicit NetworkDataSource(QObject *parent = 0);

    IConnection* dataConnection() const;

    const QString& id() const;
    const QString& connectionID() const;
    SupervisorWidgetState state() const;
    const QString& networkName() const;
    const QStringList& availableNetworks() const;

    const QObject *item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

signals:    //! Properties
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void stateChanged(QVariant state);
    void networkChanged(QVariant name);
    void availableNetworksChanged(QVariant networks);
    
public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setNetworkName(const QString& name);
    void setAvailableNetworks(const QStringList& networks);

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
    QStringList mAvailableNetworks;
    MonitoringSocket* mSocket;
    TItemHash mItems;
};

class NetworkDataSourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString networkName READ networkName WRITE setNetworkName NOTIFY networkChanged)
public:
    NetworkDataSourceItem(QObject* parent = 0);

    const QString& id() const;
    SupervisorWidgetState state() const;
    const QString& networkName() const;
    void setDefaut(const QString& defaut);
    QString getDefaut();

signals:    //! Properties
    void idChanged(QVariant id);
    void stateChanged(QVariant state);
    void networkChanged(QVariant name);

public slots:
    void bind();
    void setID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setNetworkName(const QString& name);

private:
    QString mID;
    QString mConnectionID;
    SupervisorWidgetState mState;
    QString mNetworkName;
    QString mDefaut;
};

#endif // NETWORKDATASOURCE_H
