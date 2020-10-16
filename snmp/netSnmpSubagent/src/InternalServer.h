#ifndef INTERNALSERVER_H
#define INTERNALSERVER_H

#include "Service.h"

#include "Notify.h"
#include "SetValue.h"

class InternalRunnable;

namespace local_connection
{

class LocalServer;
class LocalSocket;

} // namespace local_connection

class Pinger;

class QThreadPool;

class InternalServer : public Service
{
    Q_OBJECT
public:
    explicit InternalServer(QObject *parent = 0);

public slots:
    void getValue(const QString &socketName, const QString &key);
    void setValue(const QString &socketName, const QString &key, const QVariant &value);

    void onHandshakeCame(local_connection::LocalSocket *socket);
    void onNotifyCame(local_connection::LocalSocket *socket
                      , const QString &key, const snmp_interaction::Vars &vars);
    void onSetValueResultCame(local_connection::LocalSocket *socket, const QString &key
                              , snmp_interaction::SetValueResult::SetResult result);
    void onValueCame(local_connection::LocalSocket *socket, const QString &key, const QVariant &value);

signals:
    void clientDisconnected(const QString &socketName);

    void handshakeCame(const QString &socketName);
    void notifyCame(const QString &socketName, const QString &key, const snmp_interaction::Vars &vars);
    void pingCame(const QString &socketName);
    void valueCame(const QString &socketName, const QString &key, const QVariant &value);

protected:
    void pStart();
    void pStop();

private:
    InternalRunnable * runnable(local_connection::LocalSocket *socket, const QVariant &data);

    local_connection::LocalSocket * socketByName(const QString &socketName);

private slots:
    void onNewConnection(local_connection::LocalSocket *socket);
    void onDisconnected(local_connection::LocalSocket *socket);
    void onError(local_connection::LocalSocket *socket, const QString &text);
    void onReadyRead(local_connection::LocalSocket *socket, const QVariant &data);

    void onRunnableInfo(const QString &object, const QString &text);
    void onRunnableError(const QString &object, const QString &text);
    void onSend(local_connection::LocalSocket *socket, const QVariant &data);

    void onSendPing(const QString &socketName);
    void onTimeout(const QString &socketName);

private:
    local_connection::LocalServer *mServer;
    QThreadPool *mPool;
    Pinger *mPinger;
};

#endif // INTERNALSERVER_H
