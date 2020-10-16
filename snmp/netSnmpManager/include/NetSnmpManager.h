#ifndef NETSNMPMANAGER_H
#define NETSNMPMANAGER_H

#include "netSnmpManager_global.h"

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>

namespace snmp_private
{
class NetSnmpManagerPrivate;
} // namespace snmp_private

namespace snmp
{

struct SnmpProgramState;
struct SnmpScript;
struct SnmpTrap;
struct SnmpVariable;

/*!
 * \class NetSnmpManager
 */

class NETSNMPMANAGERSHARED_EXPORT NetSnmpManager : public QObject
{
    Q_OBJECT
private:
    NetSnmpManager(QObject *parent = 0);
    ~NetSnmpManager();

public:
    static NetSnmpManager * instance(QObject *parent = 0);

    bool isRunning() const;

    // Для многопоточного приложения вместо removeAllHosts лучше устанавливать данное значение в true.
    void setRemoveHostsOnStop(bool remove);

public slots:
    void start();
    void stop();

    void addHost(const QString &host, const QString &community);
    void removeHost(const QString &host);
    void removeAllHosts();

    void setVariable(const QString &host, const QString &oid, const QString &value, char type);

    void requestVariable(const QString &host, QStringList oids);

signals:
    void started();
    void stopped();

    void connected(const QString &host);
    void lostConnection(const QString &host);

    void variablesReceived(QString host, QVariantMap variables);
    void trap(QString hostname, QString ip, QVariantMap variables);

    void error(const QString &text);

private:
    snmp_private::NetSnmpManagerPrivate *d;
};

} // namespace snmp

#define netSnmpManager snmp::NetSnmpManager::instance()

#endif // NETSNMPMANAGER_H
