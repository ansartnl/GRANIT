#include <QtCore/QTimer>

#include "SnmpController.h"
#include "Log.h"

SnmpController::SnmpController(QObject *parent) :
    QObject(parent)
{
    connect(netSnmpManager, SIGNAL(variablesReceived(QString,QVariantMap)), SLOT(onVariablesReceived(QString,QVariantMap)));
    connect(netSnmpManager, SIGNAL(trap(QString,QString,QVariantMap)), SLOT(onTrap(QString,QString,QVariantMap)));
    connect(netSnmpManager, SIGNAL(error(QString)), SLOT(onError(QString)));
}

void SnmpController::addHost(const QString &host, const QString &community, int period, const Agent &agent)
{
    netSnmpManager->addHost(host, community);

    mAgents.insert(host, agent);

    if (period != 0) {
        QTimer *timer = new QTimer(this);
        timer->setInterval(period * 1000);
        connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));
        mTimers.insert(timer, host);
    }
}

void SnmpController::start()
{
    if (!netSnmpManager->isRunning()) {
        netSnmpManager->start();

        foreach (QTimer *timer, mTimers.keys()) {
            timer->start();
        }
    }

    QMap<QString, Agent>::const_iterator it = mAgents.constBegin();
    while (it != mAgents.constEnd()) {
        netSnmpManager->requestVariable(it.key(), it.value().monitoredVariables.values());
        ++it;
    }
}

void SnmpController::stop()
{
    if (netSnmpManager->isRunning()) {
        netSnmpManager->stop();

        foreach (QTimer *timer, mTimers.keys()) {
            timer->stop();
        }
    }
}

void SnmpController::setVariable(const QString &agent, const QString &variable, const QString &value)
{
    QMap<QString, Agent>::const_iterator it = mAgents.constBegin();
    while (it != mAgents.constEnd()) {
        if (it.value().name == agent)
            break;

        ++it;
    }

    if (it == mAgents.constEnd()) {
        qLogWarn(tr("SnmpController")) << tr(" Unable to find agent '%1'").arg(agent);
        return;
    }

    if (!it.value().variables.contains(variable)) {
        qLogWarn(tr("SnmpController")) << tr(" Unable to find variable '%1'").arg(variable);
        return;
    }

    Variable v = it->variables.value(variable);
    netSnmpManager->setVariable(it.key(), v.oid, value, v.type);
}

void SnmpController::onTimeout()
{
    QTimer *timer = qobject_cast<QTimer*>(sender());
    Q_ASSERT_X(timer, "SnmpController::onTimeout", "sender is not QTimer");

    QString host = mTimers.value(timer);

    netSnmpManager->requestVariable(host, mAgents.value(host).monitoredVariables.values());
}

void SnmpController::onVariablesReceived(QString host, QVariantMap variables)
{
    if (!mAgents.contains(host)) {
        qLogWarn(tr("SnmpController")) << tr(" Unable to find agent for host %1").arg(host);
        return;
    }

    const Agent &agent = mAgents[host];

    QVariantMap vars;
    QVariantMap::const_iterator it = variables.constBegin();
    while (it != variables.constEnd()) {
        QString name = agent.monitoredVariables.key(it.key());
        if (!name.isEmpty()) {
            vars.insert(name, it.value());
        }
        ++it;
    }

    emit variablesReceived(agent.name, vars);
}

void SnmpController::onTrap(QString hostname, QString ip, QVariantMap variables)
{
    QString key;
    if (mAgents.contains(hostname)) {
        key = hostname;
    }
    else if (mAgents.contains(ip)) {
        key = ip;
    }
    else {
        qLogWarn(tr("SnmpController")) << tr(" Unable to find agent for hostname %1 or ip %2").arg(hostname).arg(ip);
        return;
    }

    const Agent &agent = mAgents[key];

    QVariantMap vars;
    QVariantMap::const_iterator it = variables.constBegin();
    while (it != variables.constEnd()) {
        QString name = agent.traps.key(it.key());
        if (!name.isEmpty()) {
            vars.insert(name, it.value());
        }
        ++it;
    }

    emit trap(agent.name, vars);
}

void SnmpController::onError(QString error)
{
    qLogWarn() << error;
}
