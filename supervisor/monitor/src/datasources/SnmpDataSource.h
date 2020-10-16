#ifndef SNMPDATASOURCE_H
#define SNMPDATASOURCE_H

#include <QObject>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"

class IConnection;
class MonitoringSocket;
class SnmpDataSourceItem;

class SnmpDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    typedef QHash<QString, SnmpDataSourceItem*> TItemHash;
public:
    explicit SnmpDataSource(QObject *parent = 0);

    IConnection* dataConnection() const;

    const QString& id() const;
    const QString& connectionID() const;
    SupervisorWidgetState state() const;

    const QObject *item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

signals:    //! Properties
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void stateChanged(QVariant state);

public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setState(SupervisorWidgetState state);

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


class SnmpDataSourceItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)
public:
    SnmpDataSourceItem(const QString &connectionId, QObject* parent = 0);

    const QString& id() const;
    SupervisorWidgetState state() const;
    const QString& itemName() const;
    void setDefaut(const QString& defaut);
    QString getDefaut();

    QString stateVariable() const;
    void setStateVariable(const QString variable);
    void setStateMap(QMap<QString, SupervisorWidgetState> map);
    void setState(const QString &variable);

signals:    //! Properties
    void idChanged(QVariant id);
    void stateChanged(QVariant state);
    void itemNameChanged(QVariant name);

public slots:
    void bind();
    void setID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setItemName(const QString& name);

private:
    QString mID;
    const QString mConnectionID;
    SupervisorWidgetState mState;
    QString mItemName;
    QString mStateVariable;
    QMap<QString, SupervisorWidgetState> mStateMap;
    QString mDefaut;
};
#endif // SNMPDATASOURCE_H
