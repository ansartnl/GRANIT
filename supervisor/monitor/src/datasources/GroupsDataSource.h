#ifndef GROUPSDATASOURCE_H
#define GROUPSDATASOURCE_H

#include <QVariant>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"

class IConnection;
class MonitoringSocket;

class GroupsDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(QString activeGroup READ activeGroup WRITE setActiveGroup NOTIFY activeGroupChanged)
public:
    GroupsDataSource(QObject *parent = 0);

    const QString& id() const;
    const QString& connectionID() const;
    const QString& activeGroup() const;

    const QObject* item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

    IConnection* dataConnection() const;

signals:    //! Properties
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void activeGroupChanged(QVariant activeGroup);

public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setActiveGroup(const QString& acitveGroup);

    void setDataConnection(IConnection* socket);

private slots:
    void onSocketStateChanged();
    void onVariantRecieved(QVariant v);

private:
    QString mID;
    QString mConnectionID;
    QString mActiveGroup;
    MonitoringSocket* mSocket;
};

#endif // GROUPSDATASOURCE_H
