#ifndef _PROCESS_CONTROLLER_H__
#define _PROCESS_CONTROLLER_H__

#include "Controller.h"

#include "enumstreaming.h"

#include <QtCore/QProcess>

// we don't like including windows.h in header - it causes compile errors
typedef void * HANDLE;
typedef char * LPSTR;
typedef unsigned long DWORD;

class QWinProcessStarter;

QT_DECLARE_ENUM_STREAMING(QProcess::ExitStatus)

//! Controller for monitoring an executable process.
class ProcessController : public Controller
{
    Q_OBJECT
public:
    ProcessController();

    //! Set path to the process with possible arguments and user, to run as
    void setPath(const QString &path, const QString &arguments = QString()
            , const QString &user = QString());

    //! Retrieve current controller info.
    virtual Info information() const;

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

    QString userName();
    LPSTR getErrorMessage(DWORD err = (DWORD)-1);

private slots:
    void on_started();
    void on_finished(int, QProcess::ExitStatus);
    void on_error(int error);

    void runProcess();

private:
    QTimer *startProcessTimer;
    QString mUserName;
    QString mUserNeeded;
    HANDLE hToken;
    QWinProcessStarter *starter;

    /** Path to process executable */
    QString                 mPath;
    /** Arguments for command line */
    QString                 mArguments;
    /** Running state of the process */
    bool                    mIsRunning;
    /** */
    bool mRestartNeeded;
};

#endif // _PROCESS_CONTROLLER_H__
