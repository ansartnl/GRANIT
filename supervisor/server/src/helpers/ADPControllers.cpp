#include "ADPControllers.h"

#include "Log.h"
#include "init.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "qt/qcorecmdlineargs_p.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#endif


/////////////////////////////////////////////////////////////////////////////////
// ADPControllerHandler implementation.

ADPControllerHandler::ADPControllerHandler(QObject *parent)
    : QObject(parent), mControllerRef(0), mWatchFolderTimer(0)
{
    mWatchFolderTimer = new QTimer(this);
    mWatchFolderTimer->setInterval( 1000 );
    connect(mWatchFolderTimer, SIGNAL(timeout()), this, SLOT(onWatchTimeout()));

    mControllerRef = qobject_cast<Controller *>(parent);
}

void ADPControllerHandler::start(const QString &path)
{
    mArgumentsController = mControllerRef->arguments();

    mMonitoringPath = path;
    mWatchFolderTimer->start();
}

void ADPControllerHandler::onWatchTimeout()
{
    if ( mMonitoringPath.isEmpty() )
        return;

    QFileInfoList files = QDir(mMonitoringPath).entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if ( !files.empty() )
    {
        const QFileInfo &fi = files.at(0);
        QString currentStatus = fi.fileName();

        if ( currentStatus != mStatus )
        {
            mStatus = currentStatus;
            emit statusChanged(mControllerRef->name(), currentStatus);

            // Only if service was not stopped we deal with it's working mode.
            if ( mControllerRef->status() == Started )
                mControllerRef->restart();
        }
    }
}

void ADPControllerHandler::update()
{
    QFileInfoList files = QDir(mMonitoringPath).entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    if ( !files.empty() )
    {
        const QFileInfo &fi = files.at(0);
        QString currentStatus = fi.fileName();

        // Update internal status and report about it ...

        if ( currentStatus != mStatus )
        {
            mStatus = currentStatus;
            emit statusChanged(mControllerRef->name(), currentStatus);
        }

        // Update arguments of controller - to start it in proper mode ...

        if ( mStatus == "master" )
        {
            // Say that we need to restart server as master - use original arguments.
            mControllerRef->setArguments( mArgumentsController );

            qLogInfo(tr("ADPController")) << tr(" Service is about to start as master");
        }
        else
        {
            // Say that we need to restart server as slave.
            mControllerRef->setArguments( mArgumentsController + QString(" -s") );

            qLogInfo(tr("ADPController")) << tr(" Service is about to start as slave");
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////
// ADPProcessController implementation.

ADPProcessController::ADPProcessController()
    : mHandler(0)
{
    mHandler = new ADPControllerHandler(this);
    connect(mHandler, SIGNAL(statusChanged(QString, QString)),
            SIGNAL(adpStatusChanged(QString, QString)));
}

Type ADPProcessController::type() const
{
    return ADP;
}

void ADPProcessController::startWatcher(const QString &params)
{
    QMap<QString, QString> opts = ::toMap( params );

    if ( opts.contains("status_path") )
        mHandler->start( opts.value("status_path") );
    else
    {
        qLogError(tr("Controller")) << tr(" Tag 'status_path' is not defined in config file for ADP process. Status monitoring stopped");
    }
}

void ADPProcessController::on_databaseReady()
{
    mHandler->update();
    ProcessController::on_databaseReady();
}


/////////////////////////////////////////////////////////////////////////////////
// ADPServiceController implementation.

ADPServiceController::ADPServiceController()
    : mHandler(0)
{
    mHandler = new ADPControllerHandler(this);
    connect(mHandler, SIGNAL(statusChanged(QString, QString)),
            SIGNAL(adpStatusChanged(QString, QString)));
}

Type ADPServiceController::type() const
{
    return ADP;
}

void ADPServiceController::startWatcher(const QString &params)
{
    QMap<QString, QString> opts = ::toMap( params );

    if ( opts.contains("status_path") )
        mHandler->start( opts.value("status_path") );
    else
    {
        qLogWarn(tr("Controller")) << tr(" Tag 'status_path' is not defined in config file for ADP process. Status monitoring stopped");
    }
}

void ADPServiceController::on_databaseReady()
{
    mHandler->update();
    ServiceController::on_databaseReady();
}
