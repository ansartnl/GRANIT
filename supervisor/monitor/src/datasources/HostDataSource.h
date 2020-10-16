#ifndef HOSTDATASOURCE_H
#define HOSTDATASOURCE_H

#include <QVariant>

#include "IDatasource.h"
#include "src/SupervisorWidget.h"
#include "SpProto.h"

class IConnection;
class MonitoringSocket;

class HostDataSource : public IDatasource
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setID NOTIFY idChanged)
    Q_PROPERTY(QString connectionID READ connectionID WRITE setConnectionID NOTIFY connectionIDChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(SupervisorWidgetState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(QString cwp READ cwp WRITE setCWP NOTIFY cwpChanged)
    Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QStringList sectors READ sectors WRITE setSectors NOTIFY sectorsChanged)
public:
    HostDataSource(QObject *parent = 0);

    const QString& id() const;
    const QString& connectionID() const;
    SupervisorWidgetState state() const;
    const QString& name() const;
    const QString& cwp() const;
    const QString& client() const;
    const QStringList& sectors() const;

    const QObject* item(const QString& key);
    const QMetaObject* metaItem(const QString& key);
    void setDefaut(const QString& key, const QString& defaut);
    void setDatafield(const QString& key, const QString& datafield);

    IConnection* dataConnection()const;

signals:    //! Properties
    void idChanged(QVariant id);
    void connectionIDChanged(QVariant id);
    void stateChanged(QVariant state);
    void nameChanged(QVariant name);
    void cwpChanged(QVariant cwp);
    void clientChanged(QVariant client);
    void sectorsChanged(QVariant sectors);

public slots:
    void bind();
    void setID(const QString& id);
    void setConnectionID(const QString& id);
    void setState(SupervisorWidgetState state);
    void setName(const QString& name);
    void setCWP(const QString& cwp);
    void setClient(const QString& client);
    void setSectors(const QStringList& sectors);

    void setDataConnection(IConnection* socket);

private slots:
    void onSocketStateChanged();
    void onVariantRecieved(QVariant v);
    void updateSocketState();

private:
    QString mID;
    QString mConnectionID;
    SupervisorWidgetState mState;
    QString mName;
    SupervisorProto::CWPSectors mCWPSector;
    MonitoringSocket* mSocket;
};

#endif // HOSTDATASOURCE_H
