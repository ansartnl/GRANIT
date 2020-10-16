#include "NetSnmpClientPrivate.h"

#include "INetSnmpDataSource.h"
#include "RequestHandler.h"

#include "LocalSocket.h"
#include "Notify.h"
#include "PingPong.h"
#include "Pinger.h"

#include <QtCore/QTimer>

namespace snmp_private
{

NetSnmpClientPrivate * privateClient(QObject *parent)
{
    static NetSnmpClientPrivate *mPrivateClient = new NetSnmpClientPrivate(parent);
    return mPrivateClient;
}

NetSnmpClientPrivate::NetSnmpClientPrivate(QObject *parent) :
    QObject(parent), mSocket(new local_connection::LocalSocket(this)), mPinger(new Pinger(this))
{
    connect(mSocket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(mSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(mSocket, SIGNAL(readyRead(QVariant)), SLOT(onSocketReadyRead(QVariant)));

    requestHandler(this);
    connect(requestHandler(), SIGNAL(info(QString,QString)), SLOT(onRequestInfo(QString,QString)));
    connect(requestHandler(), SIGNAL(error(QString,QString)), SLOT(onRequestError(QString,QString)));

    connect(mPinger, SIGNAL(sendPing(QString)), SLOT(onSendPing()));
    connect(mPinger, SIGNAL(timeout(QString)), SLOT(onTimeout()));
    connect(this, SIGNAL(pingCame(QString)), mPinger, SLOT(pingCame(QString)));
}

void NetSnmpClientPrivate::setDataSource(snmp::INetSnmpDataSource *dataSource_)
{
    mDataSource = dataSource_;
    connectToServer();
}

snmp::INetSnmpDataSource * NetSnmpClientPrivate::dataSource() const
{
    return mDataSource;
}

void NetSnmpClientPrivate::notify(const QString &key, const snmp::Vars &vars)
{
    snmp_interaction::Vars vars2;
    foreach (const snmp::Var &var, vars) {
        vars2 << snmp_interaction::Var(var.key, var.value);
    }
    mSocket->send(QVariant::fromValue(snmp_interaction::Notify(key, vars2)));
}

void NetSnmpClientPrivate::onHandshakeCame()
{
    mPinger->addItem(LOCAL_SERVER_NAME);
}

void NetSnmpClientPrivate::connectToServer()
{
    mSocket->connectToServer(LOCAL_SERVER_NAME);
}

void NetSnmpClientPrivate::onSocketConnected()
{
    mDataSource->writeLogInfo(tr("Connected to SNMP subagent"));
}

void NetSnmpClientPrivate::onSocketDisconnected()
{
    mPinger->removeItem(LOCAL_SERVER_NAME);

    mDataSource->writeLogInfo(tr("Disconnected from SNMP subagent"));
    connectToServer();
}

void NetSnmpClientPrivate::onSocketError(const QString &text)
{
    mDataSource->writeLogError(tr("Connection error: %1").arg(text));

    if (!mSocket->isConnected())
        QTimer::singleShot(2000, this, SLOT(connectToServer()));
}

void NetSnmpClientPrivate::onSocketReadyRead(const QVariant &data)
{
    request_handler::PRequestProcessor command = requestHandler()->createProcessor(data);

    const request_handler::Reply reply = command->execute(request_handler::Request(data, mSocket));
    foreach (const QVariant &d, reply.data) {
        mSocket->send(d);
    }
}

void NetSnmpClientPrivate::onRequestInfo(const QString &object, const QString &text)
{
    mDataSource->writeLogInfo(tr("%1 - %2").arg(object).arg(text));
}

void NetSnmpClientPrivate::onRequestError(const QString &object, const QString &text)
{
    mDataSource->writeLogError(tr("%1 - %2").arg(object).arg(text));
}

void NetSnmpClientPrivate::onSendPing()
{
    mSocket->send(snmp_interaction::Ping());
}

void NetSnmpClientPrivate::onTimeout()
{
    if (!mSocket->objectName().isEmpty())
        mDataSource->writeLogError(tr("Ping timeout"));

    mSocket->disconnectFromServer();
}

} // namespace snmp_private
