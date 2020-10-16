#ifndef NETSNMPCLIENTPRIVATE_H
#define NETSNMPCLIENTPRIVATE_H

#include "Vars.h"

#include <QtCore/QObject>

namespace snmp
{

class INetSnmpDataSource;

} // namespace snmp

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class Pinger;

namespace snmp_private
{

class NetSnmpClientPrivate;

NetSnmpClientPrivate * privateClient(QObject *parent = 0);

class NetSnmpClientPrivate : public QObject
{
    Q_OBJECT

public:
    explicit NetSnmpClientPrivate(QObject *parent = 0);

    void setDataSource(snmp::INetSnmpDataSource *dataSource_);
    snmp::INetSnmpDataSource * dataSource() const;

    void notify(const QString &key, const snmp::Vars &vars);

public slots:
    void onHandshakeCame();

signals:
    void pingCame(const QString &socketName);

private slots:
    void connectToServer();

    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(const QString &text);
    void onSocketReadyRead(const QVariant &data);

    void onRequestInfo(const QString &object, const QString &text);
    void onRequestError(const QString &object, const QString &text);

    void onSendPing();
    void onTimeout();

private:
    snmp::INetSnmpDataSource *mDataSource;
    local_connection::LocalSocket *mSocket;
    Pinger *mPinger;
};

} // namespace snmp_private

#endif // NETSNMPCLIENTPRIVATE_H
