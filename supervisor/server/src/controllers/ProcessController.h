#ifndef _PROCESS_CONTROLLER_H__
#define _PROCESS_CONTROLLER_H__

#include "Controller.h"

#include <QtCore/QProcess>

class RestartInterval;

//! Controller for monitoring an executable process.
class ProcessController : public Controller
{
    Q_OBJECT

public:
    ProcessController();

    //! Set path to the process with possible arguments.
    void setPath(const QString &path);

    void setKillTimeout(int timeout);

    //! Retrieve current controller info.
    virtual Info information() const;

    //! Type associated with service controller.
    virtual Type type() const;

    //! Retrieve current status.
    virtual Status status() const;

    //! Check if process is running.
    virtual bool isRunning() const;

public slots:
    //! Starts the process.
    virtual bool start();
    //! Restarts the process.
    virtual void restart();
    //! Stops the process.
    virtual bool stop();

protected:
    //! Returns path to log file according to the controller.
    virtual QString logFilePath() const;

protected slots:
    virtual void on_databaseReady();

private slots:
    void on_started();
    void on_finished(int, QProcess::ExitStatus);
    void on_error(QProcess::ProcessError);

    void runProcess();

private:
    void stopProcess();

protected:
    QProcess                *mProcess;
    RestartInterval         *mRestartInterval;
    QTimer                  *mRestartIntervalTimer;

    /** Path to process executable */
    QString                 mPath;
    int                     mKillTimeout;
};

#endif // _PROCESS_CONTROLLER_H__
