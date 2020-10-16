#include "SubagentObjects.h"

#include "helpers/Config.h"
#include "mib-objects/IntegerMibObject.h"
#include "ValueMibObjectFactory.h"

#include "Log.h"

#include <QtCore/QStringList>

SubagentObjects::SubagentObjects(QObject *parent) :
    QObject(parent), mPingObject(0)
{
    setObjectName("subagent object");

    mibObjectFactory(this);
}

void SubagentObjects::initSubagentObjects()
{
    const QStringList clientConfigFiles = config->clients();
    foreach (const QString &configFile, clientConfigFiles) {
        ClientObject *obj = new ClientObject(configFile, this);
        connect(obj, SIGNAL(getValue(QString)), SLOT(onGetValue(QString)));
        connect(obj, SIGNAL(setValue(QString,QVariant)), SLOT(onSetValue(QString,QVariant)));
        mClients.insert(obj->identifier(), PClientObject(obj));
    }
    mPingObject = new IntegerMibObject("ping", config->pingOid(), "", true);
}

void SubagentObjects::postInitSubagentObjects()
{
    foreach (const PClientObject &client, mClients) {
        client->postInit();
    }
    mPingObject->postInit();
}

void SubagentObjects::startListening(const QString &client)
{
    PClientObject obj = mClients[client];
    if (obj.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' while starting listening").arg(client);
        return;
    }
    obj->startListening();
}

void SubagentObjects::stopListening(const QString &client)
{
    PClientObject obj = mClients[client];
    if (obj.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' while stopping listening").arg(client);
        return;
    }
    obj->stopListening();
}

void SubagentObjects::onValueCame(const QString &client, const QString &key, const QVariant &value)
{
    PClientObject obj = mClients[client];
    if (obj.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' on value came for key '%2'").arg(client).arg(key);
        return;
    }
    obj->changeValue(key, value);
}

void SubagentObjects::onNotify(const QString &client, const QString &key, const snmp_interaction::Vars &vars)
{
    PClientObject obj = mClients[client];
    if (obj.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' on trap came for key '%2'").arg(client).arg(key);
        return;
    }
    obj->notify(key, vars);
}

void SubagentObjects::onGetValue(const QString &key)
{
    ClientObject *obj = qobject_cast < ClientObject * >(sender());
    Q_ASSERT_X(obj, "onGetValue", "sender() != ClientObject");

    emit getValue(obj->identifier(), key);
}

void SubagentObjects::onSetValue(const QString &key, const QVariant &value)
{
    ClientObject *obj = qobject_cast < ClientObject * >(sender());
    Q_ASSERT_X(obj, "onSetValue", "sender() != ClientObject");

    emit setValue(obj->identifier(), key, value);
}
