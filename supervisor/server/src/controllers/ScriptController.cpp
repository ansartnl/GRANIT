#include "ScriptController.h"

#include "Log.h"
#include "ScriptRunner.h"

ScriptController::ScriptController(QObject *parent) :
    QObject(parent)
{
}

void ScriptController::setScriptList(const ScriptList &list)
{
    mScriptList = list;
}

void ScriptController::addScript(const QString &name, const QString &path)
{
    if (mScriptList.contains(name))
        qLogWarn(tr("ScriptController")) << tr(" Script '%1' already exists. Rewriting...")
                                            .arg(name);
    mScriptList.insert(name, path);
}

void ScriptController::startScript(const QString &name, const QString &parameter)
{
    if (!mScriptList.contains(name))
    {
        qLogWarn(tr("ScriptController")) << tr(" Didn't find script '%1'").arg(name);
        return;
    }

    if (mScriptRunners.contains(name))
    {
        stopScript(name);
        mScriptQueue.insert(name, parameter);
        return;
    }

    qLogInfo(tr("ScriptController")) << tr(" Starting script '%1'...").arg(name);
    ScriptRunner *scriptRunner = new ScriptRunner(mScriptList.value(name), QStringList() << parameter);
    connect(scriptRunner, SIGNAL(outputString(QString)), SLOT(onOutputString(QString)));

    connect(scriptRunner, &ScriptRunner::scriptStarted, this, [this, name]() {
        onScriptStarted(name);
    });

    connect(scriptRunner, &ScriptRunner::scriptFinished, this, [this, name](int exitCode, QProcess::ExitStatus exitStatus) {
        onScriptFinished(name, exitCode, exitStatus);
    });

    mScriptRunners.insert(name, scriptRunner);

    scriptRunner->start();
}

void ScriptController::stopScript(const QString &name)
{
    if (mScriptRunners.contains(name))
    {
        qLogInfo(tr("ScriptController")) << tr(" Stopping script '%1'...").arg(name);
        mScriptRunners.value(name)->stop();
    }
}

QString ScriptController::zExitStatus(QProcess::ExitStatus exitStatus)
{
    switch (exitStatus)
    {
    case QProcess::NormalExit:
        return tr("Normal Exit");
    case QProcess::CrashExit:
        return tr("Crash Exit");
    }
    return tr("Unknown Status");
}

void ScriptController::onOutputString(const QString &str)
{
    emit outputString(str);
}

void ScriptController::onScriptStarted(const QString &name)
{
    qLogInfo(tr("ScriptController")) << tr(" Script '%1' started").arg(name);
    emit scriptStarted(name);
}

void ScriptController::onScriptFinished(const QString &name, int exitCode, QProcess::ExitStatus exitStatus)
{
    delete mScriptRunners.take(name);

    qLogInfo(tr("ScriptController")) << tr(" Script '%1' finished with exit code %2 and exit status '%3'")
                                        .arg(name)
                                        .arg(exitCode)
                                        .arg(zExitStatus(exitStatus));
    emit scriptFinished(exitCode, exitStatus);

    if (mScriptQueue.contains(name))
    {
        startScript(name, mScriptQueue.take(name));
    }
}
