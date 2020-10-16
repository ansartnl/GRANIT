#include "ClientObject.h"

#include "helpers/Config.h"
#include "helpers/ConfigKeys.h"
#include "mib-objects/ProgramStateMibObject.h"
#include "mib-objects/ScriptMibObject.h"
#include "ValueMibObjectFactory.h"

#include "Log.h"
#ifdef NITA_XML
#include "nitaxmlsettings.h"
#endif

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

ClientObject::ClientObject(const QString &configFile, QObject *parent) :
    QObject(parent), mTimer(new QTimer(this))
{
    connect(mTimer, SIGNAL(timeout()), SLOT(onTimer()));
    mTimer->setInterval(config->clientTimerMs());

    QSharedPointer < QSettings > settings;
#ifdef NITA_XML
    if (configFile.endsWith("xml"))
        settings = QSharedPointer < QSettings >(new NitaXmlSettings(configFile));
    else
#endif
        settings = QSharedPointer < QSettings >(new QSettings(configFile, QSettings::IniFormat));

    setObjectName(settings->value(conf::kName).toString());
    mIdentifier = settings->value(conf::kIdentifier).toString();

    if (settings->childGroups().contains(conf::kProgramState)) {
        settings->beginGroup(conf::kProgramState);
        mProgramStateKey = settings->value(conf::kOid).toString();
        insertObject(mProgramStateKey
                     , new ProgramStateMibObject(settings->value(conf::kName).toString(), mProgramStateKey)
                     );
        settings->endGroup();
    }

    settings->beginGroup(conf::kScripts);
    foreach (const QString &group, settings->childGroups()) {
        settings->beginGroup(group);
        insertObject(settings->value(conf::kOid).toString()
                     , new ScriptMibObject(
                         settings->value(conf::kName).toString()
                         , settings->value(conf::kOid).toString()
                         , settings->value(conf::kPath).toString()
                         , settings->value(conf::kArguments).toString()
                         )
                     );
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup(conf::kReadOnly);
    readConfig(settings.data(), true);
    settings->endGroup();

    settings->beginGroup(conf::kReadWrite);
    readConfig(settings.data(), false);
    settings->endGroup();

    settings->beginGroup(conf::kTraps);
    foreach (const QString &group, settings->childGroups()) {
        settings->beginGroup(group);
        TrapMibObject *obj = new TrapMibObject(
                    settings->value(conf::kName).toString()
                    , settings->value(conf::kOid).toString()
                    , settings->value(conf::kKey).toString()
                    , config->trapsCommunity()
                    , config->trapsReceivers());
        connect(obj, SIGNAL(error(QString)), SLOT(onMibObjectError(QString)));
        mTrapObjects.insert(obj->key(), PTrapMibObject(obj));
        settings->endGroup();
    }
    settings->endGroup();
}

const QString ClientObject::identifier() const
{
    return mIdentifier;
}

void ClientObject::postInit()
{
    foreach (const PBaseMibObject &obj, mMibObjects) {
        obj->postInit();
    }
}

void ClientObject::startListening()
{
    mTimer->start();
    if (!mProgramStateKey.isEmpty())
        mMibObjects[mProgramStateKey]->setValue(ProgramStateMibObject::PS_UP);
}

void ClientObject::stopListening()
{
    mTimer->stop();
    if (!mProgramStateKey.isEmpty())
        mMibObjects[mProgramStateKey]->setValue(ProgramStateMibObject::PS_DOWN);
}

void ClientObject::changeValue(const QString &key, const QVariant &value)
{
    PBaseMibObject obj = mMibObjects[key];
    if (obj.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' value").arg(key);
        return;
    }
    if (!value.isValid()) {
        qLogWarn(objectName()) << tr("Value '%1' is not valid. Stopping to monitor it...").arg(key);
        mMibObjects.remove(key);
        return;
    }
    obj->setValue(value);
}

void ClientObject::notify(const QString &key, const snmp_interaction::Vars &vars)
{
    PTrapMibObject trap = mTrapObjects[key];
    if (trap.isNull()) {
        qLogWarn(objectName()) << tr("Error fetching '%1' trap").arg(key);
        return;
    }
    MibObjects entities;
    foreach (const snmp_interaction::Var &var, vars) {
        PBaseMibObject obj = mMibObjects[var.key];
        if (obj.isNull())
            continue;
        obj->setValue(var.value);
        entities.append(obj);
    }
    trap->notify(entities);
}

void ClientObject::readConfig(QSettings *settings, bool readOnly)
{
    foreach (const QString &group, settings->childGroups()) {
        settings->beginGroup(group);
        ValueMibObject *obj = mibObjectFactory()->create(
                    settings->value(conf::kType).toString()
                    , settings->value(conf::kName).toString()
                    , settings->value(conf::kOid).toString()
                    , settings->value(conf::kKey).toString()
                    , readOnly);
        if (obj)
            addObject(obj, readOnly);
        settings->endGroup();
    }
}

void ClientObject::addObject(ValueMibObject *obj, bool readOnly)
{
    if (!readOnly)
        connect(obj, SIGNAL(valueChanged(QVariant)), SLOT(onValueChanged(QVariant)));
    if (!mProgramStateKey.isEmpty() && mMibObjects.contains(mProgramStateKey)) {
        const ProgramStateMibObject *stateObject = qobject_cast < const ProgramStateMibObject * >(
                    mMibObjects[mProgramStateKey].data());
        if (stateObject) {
            connect(stateObject, SIGNAL(programIsDown()), obj, SLOT(setDefaultValue()));
        }
    }
    insertObject(obj->key(), obj);
}

void ClientObject::insertObject(const QString &key, BaseMibObject *obj)
{
    connect(obj, SIGNAL(error(QString)), SLOT(onMibObjectError(QString)));
    mMibObjects.insert(key, PBaseMibObject(obj));
}

void ClientObject::onTimer()
{
    const QList < QString > keys = mMibObjects.keys();
    foreach (const QString &key, keys) {
        const PBaseMibObject obj = mMibObjects[key];
        if (key != obj->oidString())
            emit getValue(key);
    }
}

void ClientObject::onValueChanged(const QVariant &value)
{
    ValueMibObject *obj = qobject_cast < ValueMibObject * >(sender());
    Q_ASSERT_X(obj, "onValueChanged", "sender() != ValueMibObject");

    emit setValue(obj->key(), value);
}

void ClientObject::onMibObjectError(const QString &text)
{
    qLogError(objectName()) << text;
}
