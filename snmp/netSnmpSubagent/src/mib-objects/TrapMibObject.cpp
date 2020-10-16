#include "TrapMibObject.h"

TrapMibObject::TrapMibObject(const QString &name_, const QString &oidString_, const QString &key_
                             , const QString &community_, const QStringList &receivers_) :
    BaseMibEntity(name_, oidString_)
  , mKey(key_)
  , mCommunity(community_)
  , mReceivers(receivers_)
{
}

u_char TrapMibObject::asnType() const
{
    return ASN_OBJECT_ID;
}

char TrapMibObject::type() const
{
    return 'o';
}

const QString TrapMibObject::key() const
{
    return mKey;
}

void TrapMibObject::notify(const MibObjects &objects)
{
    foreach (const QString &receiver, mReceivers) {
        sendNotify(receiver, objects);
    }
}

void TrapMibObject::init()
{
}

void TrapMibObject::sendNotify(const QString &ip, const MibObjects &objects)
{
    static const oid sysUptimeObjid[] = { 1, 3, 6, 1, 2, 1, 1, 3, 0 };
    static const oid snmpTrapObjid[] = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };

    netsnmp_session session, *ss;
    netsnmp_pdu *pdu;

    snmp_sess_init(&session);

    // common
    session.peername = strdup(qPrintable(ip));
    session.version = SNMP_VERSION_2c;

    // version 2c
    session.community = (u_char *)(strdup(qPrintable(mCommunity)));
    session.community_len = strlen((const char *)session.community);

    SOCK_STARTUP;

    ss = snmp_add(&session, netsnmp_transport_open_client("snmptrap", session.peername), NULL, NULL);
    if (ss == NULL) {
        snmp_sess_perror("snmptrap", &session);
        emit error(tr("Error creating snmp session"));
        SOCK_CLEANUP;
        return;
    }

    pdu = snmp_pdu_create(SNMP_MSG_TRAP2);
    if (!pdu) {
        emit error(tr("Failed to create notification PDU"));
        SOCK_CLEANUP;
        return;
    }

    // sys uptime
    const QByteArray sysUptime = QByteArray::number(qlonglong(get_uptime()));
    snmp_add_var(pdu, sysUptimeObjid, OID_LENGTH(sysUptimeObjid), 't', sysUptime.data());

    // trap oid
    char objidString[100];
    snprint_objid(objidString, 100, objid(), size());
    snmp_add_var(pdu, snmpTrapObjid, OID_LENGTH(snmpTrapObjid), 'o', objidString);

    // objects
    foreach (const PBaseMibObject &obj, objects) {
        const QByteArray var = obj->value();
        snmp_add_var(pdu, obj->objid(), obj->size(), obj->type(), var.data());
    }

    snmp_send(ss, pdu);
    snmp_close(ss);
//    snmp_shutdown("snmpapp");
    SOCK_CLEANUP;
}
