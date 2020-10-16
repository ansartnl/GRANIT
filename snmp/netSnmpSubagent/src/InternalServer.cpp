#include "InternalServer.h"

#include "InternalRequestHandler.h"
#include "InternalRunnable.h"

#include "Interaction.h"
#include "LocalServer.h"
#include "LocalSocket.h"
#include "Pinger.h"

#include "Log.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QThreadPool>

InternalServer::InternalServer(QObject *parent) :
    Service(parent)
  , mServer(new local_connection::LocalServer(this))
  , mPool(new QThreadPool(this))
  , mPinger(new Pinger(this))
{
    setObjectName("internal_server");

    connect(mServer, SIGNAL(newConnection(local_connection::LocalSocket*))
            , SLOT(onNewConnection(local_connection::LocalSocket*)));
    connect(mServer, SIGNAL(disconnected(local_connection::LocalSocket*))
            , SLOT(onDisconnected(local_connection::LocalSocket*)));
    connect(mServer, SIGNAL(error(local_connection::LocalSocket*,QString))
            , SLOT(onError(local_connection::LocalSocket*,QString)));
    connect(mServer, SIGNAL(readyRead(local_connection::LocalSocket*,QVariant))
            , SLOT(onReadyRead(local_connection::LocalSocket*,QVariant)));

    requestHandler(this);   // creating
    connect(requestHandler(), SIGNAL(info(QString,QString)), SLOT(onRunnableInfo(QString,QString)));
    connect(requestHandler(), SIGNAL(error(QString,QString)), SLOT(onRunnableError(QString,QString)));

    mPool->setMaxThreadCount(2);

    connect(mPinger, SIGNAL(sendPing(QString)), SLOT(onSendPing(QString)));
    connect(mPinger, SIGNAL(timeout(QString)), SLOT(onTimeout(QString)));
    connect(this, SIGNAL(pingCame(QString)), mPinger, SLOT(pingCame(QString)));
}

void InternalServer::getValue(const QString &socketName, const QString &key)
{
    local_connection::LocalSocket *socket = socketByName(socketName);
    if (!socket) {
        qLogWarn(objectName()) << tr("Error fetching socket '%1' while getting value for key '%2'")
                                  .arg(socketName).arg(key);
        return;
    }
    socket->send(snmp_interaction::GetValue(key));
}

void InternalServer::setValue(const QString &socketName, const QString &key, const QVariant &value)
{
    local_connection::LocalSocket *socket = socketByName(socketName);
    if (!socket) {
        qLogWarn(objectName()) << tr("Error fetching socket '%1' while settings value for key '%2'")
                                  .arg(socketName).arg(key);
        return;
    }
    socket->send(snmp_interaction::SetValue(key, value));
}

void InternalServer::onHandshakeCame(local_connection::LocalSocket *socket)
{
    const QString socketName = socket->objectName();
    qLogInfo(objectName()) << tr("Handshake came from '%1'").arg(socketName);

    mPinger->addItem(socketName);

    emit handshakeCame(socketName);
}

void InternalServer::onNotifyCame(local_connection::LocalSocket *socket
                                  , const QString &key, const snmp_interaction::Vars &vars)
{
    emit notifyCame(socket->objectName(), key, vars);
}

void InternalServer::onSetValueResultCame(local_connection::LocalSocket *socket, const QString &key
                                          , snmp_interaction::SetValueResult::SetResult result)
{
    if (result == snmp_interaction::SetValueResult::SR_OK)
        return;
    qLogWarn(objectName()) << tr("Error when trying to change value '%1' of client '%2': '%3'")
                              .arg(key)
                              .arg(socket->objectName())
                              .arg(snmp_interaction::SetValueResult::toString(result));
}

void InternalServer::onValueCame(local_connection::LocalSocket *socket, const QString &key
                                 , const QVariant &value)
{
    emit valueCame(socket->objectName(), key, value);
}

void InternalServer::pStart()
{
    mServer->listen(LOCAL_SERVER_NAME);

    qLogInfo(objectName()) << tr("Started");
}

void InternalServer::pStop()
{
    mServer->stop();

    qLogInfo(objectName()) << tr("Stopped");
}

InternalRunnable * InternalServer::runnable(local_connection::LocalSocket *socket, const QVariant &data)
{
    InternalRunnable *ret = new InternalRunnable(socket, data);
    ret->setAutoDelete(true);
    connect(ret, SIGNAL(send(local_connection::LocalSocket*,QVariant))
            , SLOT(onReadyRead(local_connection::LocalSocket*,QVariant)));
    return ret;
}

local_connection::LocalSocket * InternalServer::socketByName(const QString &socketName)
{
    foreach (local_connection::LocalSocket *socket, mServer->sockets()) {
        if (socket->objectName() == socketName)
            return socket;
    }
    return 0;
}

void InternalServer::onNewConnection(local_connection::LocalSocket *socket)
{
    qLogInfo(objectName()) << tr("New client connected");

    socket->send(snmp_interaction::Handshake(qApp->applicationName()));
}

void InternalServer::onDisconnected(local_connection::LocalSocket *socket)
{
    mPinger->removeItem(socket->objectName());

    qLogInfo(objectName()) << tr("Client '%1' disconnected").arg(socket->objectName());

    emit clientDisconnected(socket->objectName());
}

void InternalServer::onError(local_connection::LocalSocket *socket, const QString &text)
{
    Q_UNUSED(socket)
    qLogInfo(objectName()) << tr("Connection error: %1").arg(text);
}

void InternalServer::onReadyRead(local_connection::LocalSocket *socket, const QVariant &data)
{
    mPool->start(runnable(socket, data));
}

void InternalServer::onRunnableInfo(const QString &object, const QString &text)
{
    qLogInfo(objectName()) << tr("%1 - %2").arg(object).arg(text);
}

void InternalServer::onRunnableError(const QString &object, const QString &text)
{
    qLogWarn(objectName()) << tr("%1 - error, while processing request: %2").arg(object).arg(text);
}

void InternalServer::onSend(local_connection::LocalSocket *socket, const QVariant &data)
{
    socket->send(data);
}

void InternalServer::onSendPing(const QString &socketName)
{
    local_connection::LocalSocket *socket = socketByName(socketName);
    if (!socket) {
        qLogWarn(objectName()) << tr("Error fetching socket '%1' for sending ping").arg(socketName);
        return;
    }
    socket->send(snmp_interaction::Ping());
}

void InternalServer::onTimeout(const QString &socketName)
{
    qLogWarn(objectName()) << tr("Ping timeout for '%1'").arg(socketName);

    local_connection::LocalSocket *socket = socketByName(socketName);
    if (!socket) {
        qLogWarn(objectName()) << tr("Error fetching socket '%1' on timeout").arg(socketName);
        return;
    }
    if (socket->isConnected()) {
        socket->disconnectFromServer();
    } else {
        mPinger->removeItem(socket->objectName());
    }
}
