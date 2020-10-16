#ifndef _SUPERVISOR_SERVICE_H__
#define _SUPERVISOR_SERVICE_H__

//#include "qtservice.h"
#include "DbConnThread.h"

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QSettings>

//#include <QtGui/QApplication>

class ScriptController;
class DatabaseController;
class ApplicationController;
class LogController;
class FolderController;
class SnmpController;

class QTcpServer;

class SupervisorService : public QObject//, public QtService<QApplication>
{
    Q_OBJECT
public:
    SupervisorService(int argc, char** argv, const QString &name);
    virtual ~SupervisorService();

    inline DatabaseController * databaseController() const { return mDatabaseController; }
    inline ApplicationController * applicationController() const { return mApplicationController; }
    inline ScriptController * scriptController() const { return mScriptController; }
    inline LogController * logController() const { return mLogController; }
    inline FolderController * folderController() const { return mFolderController; }
    inline SnmpController * snmpController() const { return mSnmpController; }

signals:
    //! Signal used to do communications between Tcp connection and server modules.
    void sendCommand(QVariant);

protected:
    virtual void createApplication(int &argc, char **argv);

public:
    virtual void start();
    virtual void stop();

protected:
    bool checkSettingsKey(const QSettings &s, const QString &key);

private slots:
    void on_newConnection();
    void on_servicesStarted();
    void on_databaseReady();

private:
    /** Path to configuration file */
    QString mPathConfig;

    QTcpServer                      *mManagementServer;

    ApplicationController           *mApplicationController;
    DatabaseController              *mDatabaseController;

    ScriptController                *mScriptController;

    LogController                   *mLogController;

    FolderController                *mFolderController;

    SnmpController                  *mSnmpController;
};

#endif // _SUPERVISOR_SERVICE_H__
