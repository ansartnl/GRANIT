#include "NetSnmpManager.h"

#include "NetSnmpManagerPrivate.h"

namespace snmp
{

NetSnmpManager::NetSnmpManager(QObject *parent) :
    QObject(parent), d(new snmp_private::NetSnmpManagerPrivate(this))
{
    connect(d, SIGNAL(started()), SIGNAL(started()));
    connect(d, SIGNAL(stopped()), SIGNAL(stopped()));
    connect(d, SIGNAL(connected(QString)), SIGNAL(connected(QString)));
    connect(d, SIGNAL(lostConnection(QString)), SIGNAL(lostConnection(QString)));

    connect(d, SIGNAL(variablesReceived(QString,QVariantMap)), SIGNAL(variablesReceived(QString,QVariantMap)));
    connect(d, SIGNAL(trap(QString,QString,QVariantMap)), SIGNAL(trap(QString,QString,QVariantMap)));
    connect(d, SIGNAL(error(QString)), SIGNAL(error(QString)));
}

NetSnmpManager::~NetSnmpManager()
{
}

NetSnmpManager * NetSnmpManager::instance(QObject *parent)
{
    static NetSnmpManager *mNetSnmpManager = new NetSnmpManager(parent);
    return mNetSnmpManager;
}

bool NetSnmpManager::isRunning() const
{
    return d->isRunning();
}

void NetSnmpManager::setRemoveHostsOnStop(bool remove)
{
    d->setRemoveHostsOnStop(remove);
}

void NetSnmpManager::start()
{
    d->start();
}

void NetSnmpManager::stop()
{
    d->stop();
}

void NetSnmpManager::addHost(const QString &host, const QString &community)
{
    d->addHost(host, community);
}

void NetSnmpManager::removeHost(const QString &host)
{
    d->removeHost(host);
}

void NetSnmpManager::removeAllHosts()
{
    d->removeAllHosts();
}

void NetSnmpManager::setVariable(const QString &host, const QString &oid, const QString &value, char type)
{
    d->setVariable(host, oid, value, type);
}

void NetSnmpManager::requestVariable(const QString &host, QStringList oids)
{
    d->requestVariable(host, oids);
}

} // namespace snmp
