#ifndef HOSTWATCHER_H
#define HOSTWATCHER_H

#include "SnmpVariable.h"

#include <QtCore/QObject>
#include <QtCore/QMutex>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/types.h>

struct snmp_session;

namespace snmp_private
{

class HostWatcher : public QObject
{
    Q_OBJECT
public:
    static const int kRetries = 1;
    static const long kTimeoutMicroSeconds = 0.1 * 1000 * 1000;
    static const int kNoMoreVariables = 130;

    explicit HostWatcher(const QString &host, const QString &community, QObject *parent = 0);

    const QString host() const;
    bool isRunning() const;

    static QString oidToString(oid *data, int length);

public slots:
    bool start();
    void stop();

    void requestVariables(const QStringList &oids);
    void setVariable(const QString &oid, const QString &value, char type);

signals:
    void variablesReceived(QVariantMap);
    void error(const QString &text);

private:
    void getVariables(const QStringList &oids);

    void changeValue(const QString &oid, const QString &value, char type);
    void processResponse(int snmpStatus, snmp_pdu *response);

    snmp::SnmpVariable toVariable(const variable_list &vl);

private:
    QString mHost;
    QString mCommunity;

    snmp_session *mSession;

    static QMutex mutex;
};

} // namespace snmp_private

#endif // HOSTWATCHER_H
