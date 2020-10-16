#ifndef NETSNMPMANAGERPRIVATE_H
#define NETSNMPMANAGERPRIVATE_H

#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QVariantMap>

class QTimer;
class QUdpSocket;

namespace snmp
{

struct SnmpProgramState;
struct SnmpScript;
struct SnmpTrap;
struct SnmpVariable;

} // namespace snmp

namespace snmp_private
{

class HostWatcher;

class NetSnmpManagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit NetSnmpManagerPrivate(QObject *parent = 0);

    void setRemoveHostsOnStop(bool remove);

    void start();
    void stop();

    bool isRunning() const;

    void addHost(const QString &host, const QString &community);
    void removeHost(const QString &host);
    void removeAllHosts();

    void setVariable(const QString &host, const QString &oid, const QString &value, char type);

    void requestVariable(const QString &host, const QStringList &oids);

signals:
    void started();
    void stopped();

    void connected(const QString &host);
    void lostConnection(const QString &host);

    void variablesReceived(QString host, QVariantMap variables);
    void trap(QString hostname, QString ip, QVariantMap variables);

    void error(const QString &text);

private:
    HostWatcher * newWatcher(const QString &host, const QString &community);

    typedef QHash < QString, HostWatcher * > Hosts;
    Hosts & hosts();

    static const int kMaxNumberOfLostPings = 1;
    void checkPing();

    void receive(const QByteArray &ba);

private slots:
    void pAddHost(const QString &host, const QString &community);

    void onPingCame();
    void onVariablesReceived(QVariantMap variables);
    void onHostError(const QString &text);

    void onTrapReadyRead();

private:
    bool mRunning;
    bool mRemoveHostsOnStop;

    QMutex mutex;
    Hosts mHosts;

    QMap < QString, int > mPingsLost;

    QUdpSocket *mTrapSocket;
};

} // namespace snmp_private

#endif // NETSNMPMANAGERPRIVATE_H
