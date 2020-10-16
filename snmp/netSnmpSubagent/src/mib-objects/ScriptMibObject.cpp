#include "ScriptMibObject.h"

#include "Log.h"

ScriptMibObject::ScriptMibObject(const QString &name_, const QString &oidString_
                                 , const QString &path_, const QString &args_) :
    BaseMibObject(name_, oidString_), mScriptRunner(new ScriptRunner(path_, args_)), mValue(RS_STOPPED)
{
    connect(mScriptRunner.data(), SIGNAL(finished()), SLOT(onScriptFinished()));
    connect(mScriptRunner.data(), SIGNAL(error(QString)), SLOT(onScriptError(QString)));
}

u_char ScriptMibObject::asnType() const
{
    return ASN_INTEGER;
}

char ScriptMibObject::type() const
{
    return 'i';
}

const QByteArray ScriptMibObject::value() const
{
    return QByteArray::number(mValue);
}

void ScriptMibObject::setValue(const QVariant &value)
{
    Q_UNUSED(value)
    qLogWarn(objectName()) << tr("Cannot set value to a script");
}

void ScriptMibObject::init()
{
    tryRegisterVariable(
                netsnmp_create_handler_registration(qPrintable(mibName()), handler
                                                    , objid(), size(), HANDLER_CAN_RWRITE)
                , netsnmp_create_watcher_info(&mValue, sizeof(mValue), asnType(), WATCHER_MAX_SIZE));

    registerCallback(this);
}

void ScriptMibObject::nodeHandler(netsnmp_mib_handler *, netsnmp_handler_registration *
                                  , netsnmp_agent_request_info *, variable_list *)
{
    if (mValue == RS_RUN) {
        qLogDebug(objectName()) << tr("Script started");
        mValue = RS_RUNNING;
        mScriptRunner->start();
    }
}

void ScriptMibObject::onScriptFinished()
{
    mValue = RS_STOPPED;
    qLogDebug(objectName()) << tr("Script finished");
}

void ScriptMibObject::onScriptError(const QString &text)
{
    qLogWarn(objectName()) << tr("Script error: '%1'").arg(text);
}
