#ifndef BASEMIBOBJECT_H
#define BASEMIBOBJECT_H

#include "BaseMibEntity.h"

#include <QtCore/QSharedPointer>

#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * Инициализацию mib переменных следует проводить в функции init (не в конструкторе!!!)
 */

class BaseMibObject : public BaseMibEntity
{
    Q_OBJECT
public:
    BaseMibObject(const QString &name_, const QString &oidString_);

    virtual const QByteArray value() const = 0;
    virtual void setValue(const QVariant &value) = 0;

signals:
    void valueChanged(const QVariant &value);

protected:
    static void registerCallback(BaseMibObject *bmo);

    static int handler(netsnmp_mib_handler *mibHandler
                       , netsnmp_handler_registration *reginfo
                       , netsnmp_agent_request_info *reqinfo
                       , netsnmp_request_info *requests);

    bool tryRegisterVariable(netsnmp_handler_registration *reginfo, netsnmp_watcher_info *winfo);

    virtual void nodeHandler(netsnmp_mib_handler *mibHandler
                             , netsnmp_handler_registration *reginfo
                             , netsnmp_agent_request_info *reqinfo
                             , variable_list *vl);
};

typedef QSharedPointer < BaseMibObject > PBaseMibObject;

#endif // BASEMIBOBJECT_H
