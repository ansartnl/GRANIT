#ifndef SNMPCONTROLLER_H
#define SNMPCONTROLLER_H

#include <QtCore/QObject>

#include "NetSnmpManager.h"

class QTimer;

class SnmpController : public QObject
{
    Q_OBJECT
public:
    struct Variable
    {
        QString oid;
        char type;
    };

    struct Agent
    {
        QString name;

        // mapping variable names to oids
        QMap<QString, Variable> variables;
        QMap<QString, QString> monitoredVariables;
        QMap<QString, QString> traps;
    };

    explicit SnmpController(QObject *parent = 0);

    void addHost(const QString &host, const QString &community, int period, const Agent &agent);

public slots:
    void start();
    void stop();
    void setVariable(const QString &agent, const QString &variable, const QString &value);

signals:
    void variablesReceived(QString agent, QVariantMap variables);
    void trap(QString agent, QVariantMap variables);

private slots:
    void onTimeout();
    void onVariablesReceived(QString host, QVariantMap variables);
    void onTrap(QString hostname, QString ip, QVariantMap variables);
    void onError(QString error);

private:
    QMap<QTimer*, QString> mTimers;

    QMap<QString /*host*/, Agent> mAgents;
};

#endif // SNMPCONTROLLER_H
