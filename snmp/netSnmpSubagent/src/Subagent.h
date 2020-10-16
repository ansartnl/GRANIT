#ifndef SUBAGENT_H
#define SUBAGENT_H

#include "Service.h"

#include "Notify.h"

#include <QtCore/QReadWriteLock>

class SubagentLoop;
class SubagentObjects;

class SubAgent : public Service
{
    Q_OBJECT
public:
    explicit SubAgent(QObject *parent = 0);

signals:
    void startListening(const QString &client);
    void stopListening(const QString &client);
    void getValue(const QString &client, const QString &key);
    void valueCame(const QString &client, const QString &key, const QVariant &value);
    void setValue(const QString &client, const QString &key, const QVariant &value);
    void notify(const QString &client, const QString &key, const snmp_interaction::Vars &vars);

protected:
    void pStart();
    void pStop();

private:
    bool init();

    const QString demonName() const;

private slots:
    void finalize();

private:
    SubagentLoop *mLoop;
    SubagentObjects *mObjects;
};

#endif // SUBAGENT_H
