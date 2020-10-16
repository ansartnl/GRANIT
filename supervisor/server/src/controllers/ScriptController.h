#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QProcess>

// key = name, value = path
typedef QMap<QString, QString> ScriptList;

class ScriptRunner;

class ScriptController : public QObject
{
    Q_OBJECT
public:
    explicit ScriptController(QObject *parent = 0);

    void setScriptList(const ScriptList &list);
    void addScript(const QString &name, const QString &path);
    inline QString path(const QString &name) const { return mScriptList.value(name); }
    inline QStringList names() const { return mScriptList.keys(); }

public slots:
    void startScript(const QString &name, const QString &parameter = QString());
    void stopScript(const QString &name);

signals:
    void outputString(const QString &str);
    void scriptStarted(const QString &name);
    void scriptFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    QString zExitStatus(QProcess::ExitStatus exitStatus);

protected slots:
    void onOutputString(const QString &str);
    void onScriptStarted(const QString &name);
    void onScriptFinished(const QString &name, int exitCode, QProcess::ExitStatus exitStatus);

protected:
    ScriptList mScriptList;

    QMap<QString, ScriptRunner*> mScriptRunners;
    QMap<QString, QString> mScriptQueue;
};

#endif // SCRIPTCONTROLLER_H
