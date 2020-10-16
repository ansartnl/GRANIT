#include "ApplicationController.h"

#include "ClientConnection.h"
#include "GroupsHelper.h"

#include "DatabaseController.h"
#include "ServiceController.h"
#include "ProcessController.h"
#include "FMTPControllers.h"
#include "FDPControllers.h"
#include "ADPControllers.h"
#include "CWPController.h"

#include "Log.h"

#include <QtCore/QMutexLocker>

ApplicationController::ApplicationController(DatabaseController *dbController, QObject *parent)
    : QObject(parent), mDbControllerRef(dbController)
{
    Q_ASSERT(mDbControllerRef);
}

void ApplicationController::addServices(const TServiceOptionList &scList)
{
    foreach ( const ServiceOption &opt, scList )
    {
        DbConnThread *databaseChecker = mDbControllerRef->database(opt.database);

        TController ctrl;
        if ( FMTP == opt.type )
            ctrl = TController(new FMTPServiceController(databaseChecker ? databaseChecker->databaseConnectionName() : 0));
        else if ( FDP == opt.type )
            ctrl = TController(new FDPServiceController);
        else if ( ADP == opt.type )
            ctrl = TController(new ADPServiceController);
        else
            ctrl = TController(new ServiceController);

        ServiceController *service = static_cast<ServiceController *>(ctrl.data());
        service->setDatabaseChecker( databaseChecker );
        service->setName(opt.id);
        service->setService(opt.namePath);
        service->setArguments(opt.arguments);
        service->setConfigPath(opt.configPath);
        service->setLogPath(opt.logPath);
        service->setVisualName(opt.visualName);
        service->setDescription(opt.description);
        service->setRestartable(opt.isRestartable);

        mControllers[opt.id] = ctrl;

        // Special initializations ...
        if ( FMTP == opt.type )
            static_cast<FMTPServiceController *>(ctrl.data())->setConfigName(QString());
        else if ( ADP == opt.type )
            static_cast<ADPServiceController *>(ctrl.data())->startWatcher(opt.otherOptions);
    }
}

void ApplicationController::addProcesses(const TServiceOptionList &scList)
{
    foreach ( const ServiceOption &opt, scList )
    {
        DbConnThread *databaseChecker = mDbControllerRef->database(opt.database);

        TController ctrl;
        if ( CWP == opt.type )
            ctrl = TController(new CWPProcessController);
        else if ( FMTP == opt.type )
            ctrl = TController(new FMTPProcessController(databaseChecker ? databaseChecker->databaseConnectionName() : 0));
        else if ( FDP == opt.type )
            ctrl = TController(new FDPProcessController);
        else if ( ADP == opt.type )
            ctrl = TController(new ADPProcessController);
        else
            ctrl = TController(new ProcessController);

        ProcessController *process = static_cast<ProcessController *>(ctrl.data());
        process->setDatabaseChecker( databaseChecker );
        process->setName(opt.id);
        process->setPath(opt.namePath);
        process->setKillTimeout(opt.killTimeout);
        process->setArguments(opt.arguments);
        process->setConfigPath(opt.configPath);
        process->setLogPath(opt.logPath);
        process->setVisualName(opt.visualName);
        process->setDescription(opt.description);
        process->setRestartable(opt.isRestartable);

        mControllers[opt.id] = ctrl;

        // Special initializations ...
        if ( CWP == opt.type )
            static_cast<CWPProcessController *>(ctrl.data())->setConfigPath(opt.configPath);
        else if ( FMTP == opt.type )
            static_cast<FMTPProcessController *>(ctrl.data())->setConfigName(QString());
        else if ( ADP == opt.type )
            static_cast<ADPProcessController *>(ctrl.data())->startWatcher(opt.otherOptions);
    }
}

void ApplicationController::connectTo(ClientConnection *clientConn)
{
    for ( TControllerMap::iterator it = mControllers.begin(); it != mControllers.end(); ++it )
    {
        TController ctrl = *it;
        Q_ASSERT( !ctrl.isNull() );

        clientConn->connect(ctrl.data(), SIGNAL(statusChanged(Type, const QString &, Status)),
                            SLOT(setStatus(Type, const QString &, Status)));

        if ( FMTP == ctrl->type() )
        {
            clientConn->connect(ctrl.data(),
                                SIGNAL(fmtpStatusChanged(QString,EFMTPStatus)),
                                SLOT(setFmtpStatus(QString,EFMTPStatus)));
        }
        else if ( FDP == ctrl->type() )
        {
            clientConn->connect(ctrl.data(), SIGNAL(fdpStatusChanged(QString,bool)),
                                SLOT(setFdpStatus(QString,bool)));
        }
        else if ( ADP == ctrl->type() )
        {
            clientConn->connect(ctrl.data(), SIGNAL(adpStatusChanged(QString,QString)),
                                SLOT(setAdpStatus(QString,QString)));
        }
        else if ( CWP == ctrl->type() )
        {
            clientConn->connect(ctrl.data(), SIGNAL(cwpSectorsChanged(QString,QString,QStringList)),
                                SLOT(setCWPSectors(QString,QString,QStringList)));
            clientConn->connect(ctrl.data(), SIGNAL(cwpUserChanged(QString,QString)),
                                SLOT(setCWPUser(QString,QString)));
        }
    }
}

SupervisorProto::RemoteOperations ApplicationController::parseLog(QStringList &log)
{
    SupervisorProto::RemoteOperations operations;
    QString currentUser("none");

    qDebug() << "Parsing log with " << log.count() << " lines";
    for(int i = log.count() -1; i >= 0; i--){
        QString item = log[i];
    //foreach(QString item, log){
        if(item.contains(tr("<usr>"))){
            int idx_from = item.indexOf(tr("<usr>")) + 5;
            int idx_to = item.indexOf(tr("</usr>"));
            currentUser = item.mid(idx_from, idx_to - idx_from);

            if(item.contains(tr("<login>"))){
                if(!operations.names.contains(currentUser))
                    operations.names.append(currentUser);
            }
            else{
                currentUser = tr("none");
            }

            qDebug() << "User: " << currentUser;
        }
        else if( item.contains("<AC>")){
            QStringList parsed = item.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            parsed.removeAt(2);//[hex]
            parsed.removeAt(2);//INFO
            parsed.removeAt(2);//root
            parsed.removeAt(2);// -
            parsed.removeAt(2);//<AC>
            parsed.removeAt(2);//CODE=
            parsed.removeAt(3);//set
            parsed.removeAt(4);//in
            parsed[0] += tr(" ") + parsed[1];
            parsed.removeAt(1);
            QString res = currentUser + "|" + parsed.join(QString("|"));
            operations.log.append(res);

            qDebug() << "Line parsed:" << res;
        }
    }
    return operations;
}

ApplicationController::TController ApplicationController::findController(const QString &id) const
{
    if ( mControllers.contains(id) )
        return mControllers[id];
    return TController();
}

bool ApplicationController::containsService(const QString &id) const
{
    return mControllers.contains(id);
}

bool ApplicationController::isRunning(const QString &id)
{
    if ( mControllers.contains(id) )
        return mControllers[id]->isRunning();
    return false;
}

void ApplicationController::restart(const QString &id)
{
    QMutexLocker locker(&mMutex);

    if ( mControllers.contains(id) )
        return mControllers[id]->restart();
}

bool ApplicationController::stop(const QString &id)
{
    QMutexLocker locker(&mMutex);

    if ( mControllers.contains(id) )
        return mControllers[id]->stop();
    return false;
}

void ApplicationController::stop()
{
    QMutexLocker locker(&mMutex);

    QList<QString> stopOrders = GroupsHelper::orderedServices();
    // No processes/services associated with this database.
    if ( stopOrders.empty() )
        return;

    qLogInfo(tr("Client")) << tr(" Stopping all processes and services of '%1' group").arg(GroupsHelper::activeGroup());

    // Order of stopping should be reversed.
    for(int k = 0; k < (stopOrders.size()/2); k++) stopOrders.swap(k,stopOrders.size()-(1+k));

    // Stop all processes and services in the reverse order they were started.
    foreach ( const QString &id, stopOrders )
    {
        if ( !mControllers.contains(id) )
            continue;

        TController ctrl = mControllers[id];
        if ( ctrl->isRunning() )
            ctrl->stop();
    }

    emit stopped();

    qLogInfo(tr("Client")) << tr(" Processes and services were stopped");
}

void ApplicationController::start()
{
    QMutexLocker locker(&mMutex);

    QList<QString> startOrders = GroupsHelper::orderedServices();
    // No processes/services associated with active group.
    if ( startOrders.empty() )
        return;

    qLogInfo(tr("Client")) << tr(" Starting processes and services of '%1' group").arg(GroupsHelper::activeGroup());

    // Start all processes and services in the order from config file.
    foreach ( const QString &id, startOrders )
    {
        if ( !mControllers.contains(id) )
            continue;

        TController ctrl = mControllers[id];
        ctrl->start();
    }

    emit started();

//    qLogInfo(tr("Client")) << tr(" All processes and services started");
}

QList<Controller::Info> ApplicationController::serviceList() const
{
    QMutexLocker locker(&mMutex);

    // Collect information about services in the special order.
    QList<Controller::Info> services;

    QStringList servicesNames = GroupsHelper::orderedServices();

    foreach ( const QString &id, servicesNames )
    {
        if ( !mControllers.contains(id) )
            continue;

        TController ctrl = mControllers[id];
        if ( !ctrl.isNull() )
            services << ctrl->information();
    }

    return services;
}

QStringList ApplicationController::activeDatabaseList() const
{
    QSet<QString> dbIdList;

    QStringList servicesNames = GroupsHelper::orderedServices();

    foreach ( const QString &id, servicesNames )
    {
        if ( !mControllers.contains(id) )
            continue;

        TController ctrl = mControllers[id];
        if ( !ctrl.isNull() && !ctrl->databaseCheckerUserData().toString().isEmpty() )
            dbIdList << ctrl->databaseCheckerUserData().toString();
    }

    return dbIdList.toList();
}

QStringList ApplicationController::readLog(const QString &id, int countLines) const
{
    QMutexLocker locker(&mMutex);

    if ( mControllers.contains(id) )
        return mControllers[id]->readLog(countLines);
    return QStringList();
}

QStringList ApplicationController::readLog(const QString &id, const QDateTime &from, const QDateTime &to, bool extractLines) const
{
    QMutexLocker locker(&mMutex);

    if ( mControllers.contains(id) )
        return mControllers[id]->readLogByTime(from, to, extractLines);
    return QStringList();
}

ApplicationController::TControllerLogs ApplicationController::readLog(const QDateTime &from, const QDateTime &to) const
{
    QMutexLocker locker(&mMutex);

    ApplicationController::TControllerLogs logs;
    foreach (const TController &contr, mControllers) {
        SupervisorProto::RemoteLog log;
        log.service = contr->name();
        log.log = contr->readLogByTime(from, to, false);
        logs.append(log);
    }
    return logs;
}
