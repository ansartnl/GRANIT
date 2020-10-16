#include "ProcessController.h"

#include "init.h"

#include "Log.h"
#include "DbConnThread.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "qt/qcorecmdlineargs_p.h"

#include "RestartInterval.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#endif

ProcessController::ProcessController()
    : mProcess(0), mKillTimeout(5000)
{
    int minInterval = xmlSettings()->value("restart_timeout_ms", 1000).toInt();
    mRestartInterval = new RestartInterval(minInterval, 20000, 5000, this);

    mRestartIntervalTimer = new QTimer(this);
    mRestartIntervalTimer->setInterval(20000);
    mRestartIntervalTimer->setSingleShot(true);
    connect(mRestartIntervalTimer, SIGNAL(timeout()), mRestartInterval, SLOT(reset()));
}

void ProcessController::setPath(const QString &path)
{
    if ( path == mPath )
        return;

    try
    {
        if ( !QFile::exists(path) )
            throw tr(" '%1' does not exist on the path: %2").arg(name()).arg(path);

        QFileInfo fi(path);
        if ( !fi.isExecutable() )
            throw tr(" '%1' is not executable file: %2").arg(name()).arg(path);

        if ( !mProcess )
        {
            mProcess = new QProcess(this);
            mProcess->setEnvironment( QProcess::systemEnvironment() );

            connect(mProcess, SIGNAL(started()), SLOT(on_started()));
            connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
                    SLOT(on_finished(int, QProcess::ExitStatus)), Qt::DirectConnection);
            connect(mProcess, SIGNAL(error(QProcess::ProcessError)),
                    SLOT(on_error(QProcess::ProcessError)), Qt::DirectConnection);
        }

        mPath = path;
    }
    catch ( const QString &ex )
    {
        qLogWarn(tr("Controller")) << ex;
    }
}

void ProcessController::setKillTimeout(int timeout)
{
    mKillTimeout = timeout;
}

Type ProcessController::type() const
{
    return PROCESS;
}

Status ProcessController::status() const
{
    return mProcess && QProcess::Running == mProcess->state() ? Started : Stopped;
}

bool ProcessController::isRunning() const
{
    return mProcess ? QProcess::Running == mProcess->state() : false;
}

void ProcessController::restart()
{
    mIsStopped = false;

    if ( mProcess && QProcess::Running == mProcess->state() )
        stopProcess();
    else
        start();
}

bool ProcessController::stop()
{
    // Do not need this connection any more.
    if ( databaseChecker() )
        databaseChecker()->disconnect(SIGNAL(connectionReady()), this, SLOT(on_databaseReady()));

    Controller::stop();

    // Close the process.
    if ( isRunning() )
    {
        qLogInfo(tr("Controller")) << tr(" Stopping process '%1' (%2).")
                                      .arg(QFileInfo(mPath).fileName()).arg(name());

        stopProcess();
        return true;
    }
    return false;
}

bool ProcessController::start()
{
    // Let us receive signal after connection to database is established.
    if ( databaseChecker() )
    {
        databaseChecker()->disconnect(SIGNAL(connectionReady()), this, SLOT(on_databaseReady()));
        connect(databaseChecker(), SIGNAL(connectionReady()), SLOT(on_databaseReady()), Qt::QueuedConnection);
    }

    mIsStopped = false;
    runProcess();
    return ( mProcess );
}

void ProcessController::runProcess()
{
    if ( mIsStopped || isRunning() )
        return;

    // Schedule process to start.
    if ( databaseChecker() )
    {
        QObject::staticMetaObject.invokeMethod(databaseChecker(), "waitConnection", Qt::QueuedConnection);
//        databaseChecker()->waitConnection();
    }

    // Run process immediatelly.
    else
        on_databaseReady();
}

void ProcessController::on_databaseReady()
{
    if ( mProcess && !isRunning() )
    {
        QStringList argsList;

#ifdef Q_OS_WIN
        QString args(arguments().data(), arguments().length());
        argsList = qWinCmdArgs(args);
#else
        argsList = mArguments.split(QChar(' '), QString::SkipEmptyParts);
#endif

        mProcess->start(mPath, argsList);
        mProcess->closeReadChannel(QProcess::StandardOutput);
        mProcess->closeReadChannel(QProcess::StandardError);
    }
}

void ProcessController::on_started()
{
    mRestartIntervalTimer->start();
    emit statusChanged(type(), name(), Started);
    qLogInfo(tr("Controller")) << tr(" %1 process has successfully started").arg(name());
}

void ProcessController::on_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    mRestartIntervalTimer->stop();

    if ( QProcess::CrashExit == exitStatus )
        qLogInfo(tr("Controller")) << tr(" %1 process crashed").arg(name());
    else if ( QProcess::NormalExit == exitStatus )
        qLogInfo(tr("Controller")) << tr(" %1 process finished with exit code '%2'").arg(name()).arg(exitCode);

//    QByteArray stdError = mProcess->readAllStandardError();
//    if ( !stdError.isEmpty() )
//        qLogError(tr("Controller")) << stdError;

    emit statusChanged(type(), name(), Stopped);

    if ( isRestartable() ) {
        QTimer::singleShot(mRestartInterval->nextInterval(), this, SLOT(runProcess()));
    }
}

void ProcessController::on_error(QProcess::ProcessError error)
{
    QString errorString;
    if ( QProcess::FailedToStart == error )
        errorString = QObject::tr(" %1 process failed to start").arg(name());
    else if ( QProcess::Crashed == error )
        errorString = QObject::tr(" %1 process crashed").arg(name());
    else if ( QProcess::Timedout == error )
        errorString = QObject::tr(" %1 process timed out").arg(name());
    else if ( QProcess::ReadError == error )
        errorString = QObject::tr(" Reading error in the %1 process").arg(name());
    else if ( QProcess::WriteError == error )
        errorString = QObject::tr(" Writing error it the %1 process").arg(name());
    else
        errorString = QObject::tr(" %1 process: Unknown error").arg(name());

    qLogWarn(tr("Controller")) << errorString;
}

Controller::Info ProcessController::information() const
{
    Info i;
    i.type = type();
    i.name = name();
    i.description = description();
    i.status = status();
    i.visualName = visualName();
    return i;
}

QString ProcessController::logFilePath() const
{
    QFileInfo fi(mPath);
    QString logPath = QString("%1%2%3.log").arg(fi.absolutePath())
            .arg(QDir::separator()).arg(fi.baseName());
    return logPath;
}

void ProcessController::stopProcess()
{
    mProcess->terminate();
#ifdef Q_OS_WIN
    PostThreadMessage(mProcess->pid()->dwThreadId, WM_QUIT, 0, 0);
#endif

    if (!mProcess->waitForFinished(mKillTimeout))
        mProcess->kill();
}
