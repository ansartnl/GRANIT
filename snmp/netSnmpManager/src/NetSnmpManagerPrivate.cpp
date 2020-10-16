#include "NetSnmpManagerPrivate.h"

#include "HostWatcher.h"
#include "SnmpHelper.h"

#include "TrapInteraction.h"

#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include <QtNetwork/QUdpSocket>

namespace snmp_private
{

NetSnmpManagerPrivate::NetSnmpManagerPrivate(QObject *parent) :
    QObject(parent), mRunning(false), mRemoveHostsOnStop(false), mTrapSocket(new QUdpSocket(this))
{
    connect(mTrapSocket, SIGNAL(readyRead()), SLOT(onTrapReadyRead()));

    qRegisterMetaTypeStreamOperators < snmp::SnmpVariable >();
    qRegisterMetaTypeStreamOperators < snmp::SnmpScript >();
    qRegisterMetaTypeStreamOperators < snmp::SnmpProgramState >();
    qRegisterMetaTypeStreamOperators < snmp::SnmpTrap >();
}

void NetSnmpManagerPrivate::setRemoveHostsOnStop(bool remove)
{
    mRemoveHostsOnStop = remove;
}

void NetSnmpManagerPrivate::start()
{
    mRunning = true;

    foreach (HostWatcher *host, hosts()) {
        host->start();
    }

    bool ok = mTrapSocket->bind(QHostAddress::LocalHost, TRAP_UDP_PORT,
                                QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    emit started();

    if (!ok) {
        emit error(QString("Unable to open port %1: %2").arg(TRAP_UDP_PORT).arg(mTrapSocket->errorString()));
    }
}

void NetSnmpManagerPrivate::stop()
{
    mRunning = false;

    foreach (HostWatcher *host, hosts()) {
        host->stop();
    }
    if (mRemoveHostsOnStop)
        removeAllHosts();
    mTrapSocket->close();
    emit stopped();
}

bool NetSnmpManagerPrivate::isRunning() const
{
    return mRunning;
}

void NetSnmpManagerPrivate::addHost(const QString &host, const QString &community)
{
    staticMetaObject.invokeMethod(this, "pAddHost", Qt::QueuedConnection,
                                  Q_ARG(QString, host),
                                  Q_ARG(QString, community));
}

void NetSnmpManagerPrivate::removeHost(const QString &host)
{
    if (!hosts().contains(host))
        return;
    HostWatcher *hw = hosts().take(host);
    disconnect(hw, 0, 0, 0);
    if (hw->isRunning())
        hw->stop();
    hw->deleteLater();
}

void NetSnmpManagerPrivate::removeAllHosts()
{
    const QList < QString > ips = hosts().keys();
    foreach (const QString &ip, ips)
        removeHost(ip);
}

void NetSnmpManagerPrivate::setVariable(const QString &host, const QString &oid, const QString &value, char type)
{
    if (hosts().contains(host)) {
        hosts()[host]->setVariable(oid, value, type);
    }
}

void NetSnmpManagerPrivate::requestVariable(const QString &host, const QStringList &oids)
{
    if (hosts().contains(host)) {
        hosts()[host]->requestVariables(oids);
    }
}

HostWatcher * NetSnmpManagerPrivate::newWatcher(const QString &host, const QString &community)
{
    HostWatcher *hw = new HostWatcher(host, community, this);

    connect(hw, SIGNAL(variablesReceived(QVariantMap)), SLOT(onVariablesReceived(QVariantMap)));
    connect(hw, SIGNAL(error(QString)), SLOT(onHostError(QString)));

    return hw;
}

NetSnmpManagerPrivate::Hosts & NetSnmpManagerPrivate::hosts()
{
    QMutexLocker locker(&mutex);
    return mHosts;
}

void NetSnmpManagerPrivate::checkPing()
{
    const QList < QString > hostAddresses = mPingsLost.keys();
    foreach (const QString &host, hostAddresses) {
        const int v = mPingsLost.value(host, 0);
        if (v > kMaxNumberOfLostPings) {
            emit lostConnection(host);
        } else {
            mPingsLost.insert(host, v + 1);
        }
    }
}

void NetSnmpManagerPrivate::receive(const QByteArray &ba)
{
    QDataStream stream(ba);
    trap_interaction::Trap trapVar;
    stream >> trapVar;

    size_t objidSize = MAX_OID_LEN;
    oid objid[MAX_OID_LEN];

    QVariantMap variables;
    const QStringList keys = trapVar.variables.keys();
    foreach (const QString &key, keys) {
        objidSize = MAX_OID_LEN;
        read_objid(qPrintable(key), objid, &objidSize);

        variables.insert(HostWatcher::oidToString(objid, objidSize),
                         trapVar.variables.value(key));
    }

    emit trap(trapVar.hostname, trapVar.ip, variables);
}

void NetSnmpManagerPrivate::pAddHost(const QString &host, const QString &community)
{
    hosts().insert(host, newWatcher(host, community));
    if (isRunning())
        hosts()[host]->start();
}

void NetSnmpManagerPrivate::onPingCame()
{
    HostWatcher *hw = qobject_cast < HostWatcher * >(sender());
    const QString host = hw->host();
    const int v = mPingsLost.value(host, kMaxNumberOfLostPings + 1);
    mPingsLost.insert(host, 0);

    if (v > kMaxNumberOfLostPings)
        emit connected(host);
}

void NetSnmpManagerPrivate::onVariablesReceived(QVariantMap variables)
{
    HostWatcher *hw = qobject_cast < HostWatcher * >(sender());
    Q_ASSERT_X(hw, "NetSnmpManagerPrivate::onVariablesReceived", "sender is not HostWatcher");
    emit variablesReceived(hw->host(), variables);
}

void NetSnmpManagerPrivate::onHostError(const QString &text)
{
    HostWatcher *hw = qobject_cast < HostWatcher * >(sender());
    Q_ASSERT_X(hw, "NetSnmpManagerPrivate::onHostError", "sender is not HostWatcher");
    emit error(tr("Host '%1' error: '%2'").arg(hw->host()).arg(text));
}

void NetSnmpManagerPrivate::onTrapReadyRead()
{
    while (mTrapSocket->hasPendingDatagrams()) {
        qint64 size = mTrapSocket->pendingDatagramSize();
        QScopedArrayPointer < char > buffer(new char[size]);
        mTrapSocket->readDatagram(buffer.data(), size);
        receive(QByteArray(buffer.data(), size));
    }
}

} // namespace snmp_private
