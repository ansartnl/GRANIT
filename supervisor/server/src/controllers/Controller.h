#ifndef _CONTROLLER_H__
#define _CONTROLLER_H__

#include "common.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QDateTime>

class DbConnThread;

QStringList readStream(const QString &filePath, int countLines);
QStringList packLines(QStringList &lines);


//! Base controller declaration.
class Controller : public QObject
{
    Q_OBJECT

public:
    //! Controller information
    struct Info
    {
        Type    type;
        QString name;
        QString description;
        Status  status;
        QString visualName;
    };

    //! Type associated with this controller.
    virtual Type type() const = 0;

    //! Retrieve current controller info.
    virtual Info information() const = 0;

    //! Retrieve current status.
    virtual Status status() const = 0;

    //! Name associated with this controller.
    inline const QString& name() const { return mName; }
    inline void setName(const QString &n) { mName = n; }

    //! Visual controller name.
    inline QString visualName() const { return mVisualName; }
    inline void setVisualName(const QString &visName) { mVisualName = visName; }

    //! Current description of this controller.
    inline QString description() const { return mDescription; }
    inline void setDescription(const QString &desc) { mDescription = desc; }

    //! Command line arguments.
    inline QString arguments() const { return mArguments; }
    inline void setArguments(const QString &args = QString()) { mArguments = args; }

    //! Read log file associated with this controller.
    /*!
        \param countLines Number of read lines. If -1 - read all log.
    */
    virtual QStringList readLog(int countLines = 300) const;

    //! Read log file associated with this controller starting from specified date/time.
    /*!
        \param from - a moment to start reading log from
        \param to   - a moment to end reading
        \param extractLines - if true - return lines from specified period,
        otherwise - returns list of log files containing theese lines
    */
    const QStringList readLogByTime(const QDateTime &from, const QDateTime &to, bool extractLines = true) const;

    //! Check if service or process is running.
    virtual bool isRunning() const = 0;

    //! Set additional relative log search pathes.
    /*! Relative path string separated by semicolon: ";".
    */
    void setLogPath(const QString &path) { mLogPath = path; }
    const QString& logPath() { return mLogPath; }

    //! Set path to configuration file.
    void setConfigPath(const QString &path) { mConfigPath = path; }
    const QString& configPath() const { return mConfigPath; }

    //! True if controller needs to monitore the restart.
    void setRestartable(bool on) { mIsRestartable = on; }

    //! Set reference to database connection checker object.
    void setDatabaseChecker(DbConnThread *dbConnChecker);

    //! User data of database checker object.
    QVariant databaseCheckerUserData() const;

public slots:
    //! Starts service.
    virtual bool start() = 0;
    //! Restarts service.
    virtual void restart() = 0;
    //! Stops specified service.
    virtual bool stop();

signals:
    void statusChanged(Type type, const QString &name, Status status);

protected:
    Controller(QObject *parent=0);

    const QString nextLog(QString prevName) const;

    const QDateTime lineTimeFromFile(QString &log) const;

    //! Returns path to log file according to the controller.
    virtual QString logFilePath() const = 0;

    //! Returns true if restart is required for controller.
    bool isRestartable() const { return mIsRestartable; }

    //! Reference to current database checker.
    DbConnThread* databaseChecker() const { return mDbConnection; }

    //! True if database connection checker is null.
    bool isDatabaseCheckerNull() const { return mDbConnection == 0; }


    /** Controller identifier (name) */
    QString         mName;
    /** List of relative log pathes to the current folder of controller or supervisor server */
    QString         mLogPath;
    /** Configuration file path */
    QString         mConfigPath;
    /** True if restart is needed */
    bool            mIsRestartable;
    /** Visual name of the controller */
    QString         mVisualName;
    /** Description of the controller */
    QString         mDescription;
    /** Arguments for command line */
    QString         mArguments;

    /** True when service was stopped */
    bool            mIsStopped;

private:
    /** Cloned instance of database thread */
    DbConnThread    *mDbConnection;
};

#endif // _CONTROLLER_H__
