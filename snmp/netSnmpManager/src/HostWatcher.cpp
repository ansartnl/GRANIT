
#include <QtCore/QStringList>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QtConcurrentRun>
#else
#include <QtConcurrent/QtConcurrentRun>
#endif

#include "HostWatcher.h"

#include "SnmpHelper.h"

namespace snmp_private
{

QMutex HostWatcher::mutex;

HostWatcher::HostWatcher(const QString &host, const QString &community, QObject *parent) :
    QObject(parent), mHost(host), mCommunity(community), mSession(0)
{
}

const QString HostWatcher::host() const
{
    return mHost;
}

bool HostWatcher::isRunning() const
{
    return (mSession);
}

void HostWatcher::getVariables(const QStringList &oids)
{
    QMutexLocker locker(&mutex);

    if (!isRunning() || oids.isEmpty())
        return;

    snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);

    foreach (const QString &o, oids) {
        size_t requestOidLength = MAX_OID_LEN;
        oid requestOid[MAX_OID_LEN];
        read_objid(qPrintable(o), requestOid, &requestOidLength);

        snmp_add_null_var(pdu, requestOid, requestOidLength);
    }

    snmp_pdu *response = 0;
    int snmpStatus = snmp_synch_response(mSession, pdu, &response);

    processResponse(snmpStatus, response);
}

bool HostWatcher::start()
{
    init_snmp(qPrintable(host()));
    snmp_session session;
    snmp_sess_init(&session);

    // common
    session.peername = strdup(qPrintable(host()));
    session.retries = kRetries;
    session.timeout = kTimeoutMicroSeconds;

    session.version = SNMP_VERSION_2c;

    // version 2c
    session.community = (u_char *)(strdup(qPrintable(mCommunity)));
    session.community_len = strlen((const char *)session.community);

    // version 3 - does not work
    session.isAuthoritative = true;
    session.securityName = strdup("aero");
    session.securityNameLen = strlen(session.securityName);
    session.securityAuthProto = usmHMACSHA1AuthProtocol;
    session.securityAuthProtoLen = USM_AUTH_PROTO_SHA_LEN;
    strcpy((char *)(session.securityAuthKey), "q1q1q1q1");
    session.securityAuthKeyLen = strlen((const char *)session.securityAuthKey);
    session.securityPrivProto = usmDESPrivProtocol;
    session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    strcpy((char *)(session.securityPrivKey), "q1q1q1q1");
    session.securityPrivKeyLen = strlen((const char *)session.securityPrivKey);
    session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

    SOCK_STARTUP;
    mSession = snmp_open(&session);
    if (!mSession) {
        emit error(tr("Error opening snmp to '%1'").arg(host()));
        SOCK_CLEANUP;
        return false;
    }
    return true;
}

void HostWatcher::stop()
{
    snmp_close(mSession);
    SOCK_CLEANUP;
    mSession = 0;
}

void HostWatcher::requestVariables(const QStringList &oids)
{
    //getVariables(oids);
    QtConcurrent::run(this, &HostWatcher::getVariables, oids);
}

void HostWatcher::setVariable(const QString &oid, const QString &value, char type)
{
    QtConcurrent::run(this, &HostWatcher::changeValue, oid, value, type);
}

void HostWatcher::changeValue(const QString &oid, const QString &value, char type)
{
    size_t requestOidLength = MAX_OID_LEN;
    ::oid requestOid[MAX_OID_LEN];
    read_objid(qPrintable(oid), requestOid, &requestOidLength);

    snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_SET);
    if (snmp_add_var(pdu, requestOid, requestOidLength, type, qPrintable(value)) != STAT_SUCCESS) {
        emit error(tr("Error creating pdu"));
        return;
    }
    snmp_pdu *response = 0;

    int snmpStatus = snmp_synch_response(mSession, pdu, &response);
    if (!response) {
        emit error(tr("Null response"));
        return;
    }
    if (snmpStatus != STAT_SUCCESS || response->errstat != SNMP_ERR_NOERROR) {
        if (snmpStatus == STAT_SUCCESS) {
            emit error(tr("Error in packet. Reason: %1.").arg(snmp_errstring(response->errstat)));
        } else if(snmpStatus == STAT_TIMEOUT) {
            emit error(tr("Timeout: No response from %1.").arg(host()));
        } else {
            snmp_sess_perror("snmp get", mSession);
        }
    }
    if (response) {
        snmp_free_pdu(response);
    }
}

void HostWatcher::processResponse(int snmpStatus, snmp_pdu *response)
{
    QVariantMap result;

    if (!response) {
        emit error(tr("Null response"));
        emit variablesReceived(result);
        return;
    }

    if (snmpStatus == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
        for (variable_list *vl = response->variables; vl; vl = vl->next_variable) {
            if (vl->type != kNoMoreVariables && vl->type != SNMP_NOSUCHOBJECT) {
                QString oid = oidToString(vl->name_loc, vl->name_length);
                result.insert(oid, toVariable(*vl).value);
            }
        }
    }
    else {
        if (snmpStatus == STAT_SUCCESS) {
            emit error(tr("Error in packet. Reason: %1.").arg(snmp_errstring(response->errstat)));
        } else if(snmpStatus == STAT_TIMEOUT) {
            emit error(tr("Timeout: No response from %1.").arg(host()));
        } else {
            snmp_sess_perror("snmp get", mSession);
        }
    }

    if (response) {
        snmp_free_pdu(response);
    }

    emit variablesReceived(result);
}

snmp::SnmpVariable HostWatcher::toVariable(const variable_list &vl)
{
    snmp::BaseSnmpVariable base = snmp_helper::getSnmpInfo(host(), vl.name, vl.name_length);

    static const size_t buffLen = 30000;
    static char buff[buffLen];
    snprint_value(buff, buffLen, vl.name, vl.name_length, &vl);
    QString valueString(buff);

    const snmp_helper::Value value = snmp_helper::valueByType(valueString, vl.type);


    snmp::SnmpVariable variable(base);
    variable.writable = snmp_helper::isWritable(vl.name, vl.name_length);
    variable.value = value.first;
    variable.type = value.second;

    return variable;
}

QString HostWatcher::oidToString(oid *data, int length)
{
    QString result;
    for (int i = 0; i < length; ++i) {
        result.append(".");
        result.append(QString::number(data[i]));
    }

    return result;
}

} // namespace snmp_private
