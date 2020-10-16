#include "ServiceController.h"

#include "Log.h"
#include "DbConnThread.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "qt/qcorecmdlineargs_p.h"


ServiceController::ServiceController()
{
    mServicesSpy = new QTimer(this);
    connect(mServicesSpy, SIGNAL(timeout()), SLOT(on_spy_timeout()));
}

void ServiceController::setService(const QString &name)
{
    mService.reset(new QtServiceController(name));
}

QString ServiceController::serviceName() const
{
    return mService ? mService->serviceName() : QString::null;
}

bool ServiceController::isRunning() const
{
    return mService ? mService->isRunning() : false;
}

Type ServiceController::type() const
{
    return SERVICE;
}

Status ServiceController::status() const
{
    return mService ? (mService->isRunning() ? Started : Stopped) : Stopped;
}

bool ServiceController::start()
{
    // Let us receive signal after connection to database is established.
    if ( databaseChecker() )
    {
        databaseChecker()->disconnect(SIGNAL(connectionReady()), this, SLOT(on_databaseReady()));
        connect(databaseChecker(), SIGNAL(connectionReady()), SLOT(on_databaseReady()), Qt::QueuedConnection);
    }

    bool ret = false;
    if ( mService )
    {
        mIsStopped = false;
        ret = startService();
    }

    QObject::staticMetaObject.invokeMethod(mServicesSpy, "start", Qt::QueuedConnection
                                           , Q_ARG(int, 5000));
    return ret;
}

void ServiceController::restart()
{
    if ( mService )
    {
        mIsStopped = false;
        mService->stop();
        emit statusChanged(type(), name(), mService->isRunning() ? Started : Stopped);
        startService();
    }
}

bool ServiceController::stop()
{
    QObject::staticMetaObject.invokeMethod(mServicesSpy, "stop", Qt::QueuedConnection);

    // Do not need this connection any more.
    if ( databaseChecker() )
        databaseChecker()->disconnect(SIGNAL(connectionReady()), this, SLOT(on_databaseReady()));

    Controller::stop();

    bool ret = false;
    if ( mService )
    {
        qLogInfo(tr("Controller")) << tr(" Stopping service '%1' (%2).")
                                      .arg(mService->serviceName()).arg(name());

        ret = mService->stop();
        emit statusChanged(type(), name(), mService->isRunning() ? Started : Stopped);

        if ( !mService->isRunning() )
            qLogInfo(tr("Controller")) << tr(" Service '%1' (%2) was stopped.")
                                          .arg(mService->serviceName()).arg(name());
        else
            qLogInfo(tr("Controller")) << tr(" Service '%1' (%2) was not stopped and is still running.")
                                          .arg(mService->serviceName()).arg(name());
    }
    return ret;
}

bool ServiceController::startService()
{
    QtServiceController *controller = mService.data();
    Q_ASSERT(controller);

    if ( !controller->isInstalled() )
    {
        qLogWarn(tr("Controller")) << tr(" Service '%1' (%2) is not installed yet.")
                                      .arg(controller->serviceName()).arg(name());
        return false;
    }

    if ( controller->isRunning() )
    {
        emit statusChanged(type(), name(), Started);
        qLogInfo(tr("Controller")) << tr(" Service '%1' (%2) was already started.")
                                      .arg(controller->serviceName()).arg(name());
        return false;
    }

    // Schedule service to start.
    if ( databaseChecker() )
    {
        QObject::staticMetaObject.invokeMethod(databaseChecker(), "waitConnection", Qt::QueuedConnection);
//        databaseChecker()->waitConnection();
    }

    // Run service immediatelly.
    else
        on_databaseReady();

    return true;
}

void ServiceController::on_databaseReady()
{
    QtServiceController *controller = mService.data();
    Q_ASSERT(controller);

    if ( controller->isRunning() )
        return;

    QStringList argsList;

#ifdef Q_OS_WIN
        QString args(arguments().data(), arguments().length());
        argsList << qWinCmdArgs(args);
#else
        argsList << mArguments.split(QChar(' '), QString::SkipEmptyParts);
#endif

//    if ( !controller->start(argsList) || !controller->isRunning() )
    if ( !controller->start(argsList) )
    {
        emit statusChanged(type(), name(), Stopped);
        qLogWarn(tr("Controller")) << tr(" Service '%1' (%2) failed to start.")
                                      .arg(controller->serviceName()).arg(name());
        return;
    }

    emit statusChanged(type(), name(), Started);
    qLogInfo(tr("Controller")) << tr(" Service '%1' (%2) has successfully been started.")
                                  .arg(controller->serviceName()).arg(name());
}

void ServiceController::on_spy_timeout()
{
    // Start none working service.
    if ( mService && mService->isInstalled() && !mService->isRunning() )
    {
        emit statusChanged(type(), name(), Stopped);

        // Block running the service if it was stopped.
        if ( isRestartable() && !mIsStopped )
            startService();
    }
    else
    {
        // Do this because of current algorithm of starting the services.
        // This also resolves problems on Linux when services is not in the running state
        // right after it is started.
        emit statusChanged(type(), name(), Started);
    }
}

Controller::Info ServiceController::information() const
{
    Info i;
    if ( !mService )
        return i;

    i.type = type();
    i.name = name();
    i.description = mService->serviceDescription();
    i.status = status();
    i.visualName = visualName();
    return i;
}

QString ServiceController::logFilePath() const
{
    QString logPath;
    if ( !mService )
        return logPath;

    QFileInfo fi(mService->serviceFilePath());
    logPath = QString("%1%2%3.log").arg(fi.absolutePath())
            .arg(QDir::separator()).arg(fi.baseName());
    return logPath;
}
