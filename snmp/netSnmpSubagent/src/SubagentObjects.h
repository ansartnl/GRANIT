#ifndef SUBAGENTOBJECTS_H
#define SUBAGENTOBJECTS_H

#include "ClientObject.h"

#include <QtCore/QHash>

class IntegerMibObject;

class SubagentObjects : public QObject
{
    Q_OBJECT
public:
    explicit SubagentObjects(QObject *parent = 0);

    void initSubagentObjects();
    void postInitSubagentObjects();

signals:
    void getValue(const QString &client, const QString &key);
    void setValue(const QString &client, const QString &key, const QVariant &value);

public slots:
    void startListening(const QString &client);
    void stopListening(const QString &client);
    void onValueCame(const QString &client, const QString &key, const QVariant &value);
    void onNotify(const QString &client, const QString &key, const snmp_interaction::Vars &vars);

private slots:
    void onGetValue(const QString &key);
    void onSetValue(const QString &key, const QVariant &value);

private:
    QHash < QString, PClientObject > mClients;
    IntegerMibObject *mPingObject;
};

#endif // SUBAGENTOBJECTS_H
