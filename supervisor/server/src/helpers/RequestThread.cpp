#include "RequestThread.h"

#include "ApplicationController.h"
#include "DatabaseController.h"
#include "NetController.h"
#include "ScriptController.h"
#include "LogController.h"
#include "SnmpController.h"

#include "ComputerHelper.h"
#include "GroupsHelper.h"

#include "FMTPControllers.h"
#include "FDPControllers.h"
#include "CWPController.h"
#include "ADPControllers.h"

#include "SpProto.h"
#include "Log.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>


RequestThread::RequestThread(QObject *parent)
    : QThread(parent), mAppCtrl(0), mNetController(0), mIsStopped(true)
{
    if ( objectName().isEmpty() )
        setObjectName("request_worker");
}

void RequestThread::setControllers(ApplicationController *appCtrl
                                   , DatabaseController *dbCtrl
                                   , NetController *netController
                                   , ScriptController *scriptController
                                   , LogController *logController
                                   , SnmpController *snmpController)
{
    mAppCtrl = appCtrl;
    mDbCtrl = dbCtrl;
    mNetController = netController;
    mScriptController = scriptController;
    mLogController = logController;
    mSnmpController = snmpController;
}

void RequestThread::add(QVariant v)
{
    QMutexLocker locker(&mUpdateMutex);
    mRequests.push(v);
    mWait.wakeAll();
}

void RequestThread::run()
{
    mIsStopped = false;

    qDebug() << tr("Request worker thread started successfully ...");

    QVariant request;

    forever
    {
        if ( mIsStopped )
            break;

        QMutexLocker mutexForeverLocker(&mForeverMutex);
        if ( mRequests.empty() )
            mWait.wait(&mForeverMutex);

        if ( mIsStopped )
            break;

        // Retrieve one airplane from the stack.
        {
            QMutexLocker locker(&mUpdateMutex);

            if ( mRequests.size() > 10000 )
            {
                qWarning() << tr("Processing stack of requests '%1' is too big. Optimization is required here.");
                // Cut stack as number of airplanes too big.
                mRequests.remove(1000, mRequests.size()-1000);
                qWarning() << tr("Processing stack is cut to 1000 requests number.");
            }

            request = mRequests.pop();
        }

        processRequest(request);
    }

    qDebug() << tr("Request worker thread was stopped ...");
}

void RequestThread::stop()
{
    mIsStopped = true;
    mWait.wakeAll();
}

void RequestThread::processRequest(QVariant v)
{
    ApplicationController *actrl = mAppCtrl;

    qDebug() << "New Request: " << v.typeName();
    // Request
    if ( v.canConvert<SupervisorProto::Request::Request>() )
    {
        SupervisorProto::Request::Request req = v.value<SupervisorProto::Request::Request>();
        if ( SupervisorProto::Request::Services == req )
        {
            SupervisorProto::Services spServices;

            QList<Controller::Info> services = actrl->serviceList();
            // Convert to proto structs.
            foreach ( const Controller::Info &s, services )
            {
                SupervisorProto::Service sp;
                sp.type = (SupervisorProto::EType) s.type;
                sp.name = s.name;
                sp.description = s.description;
                sp.status = (SupervisorProto::EStatus) s.status;
                sp.visualName = s.visualName;
                spServices.append(sp);
            }

            emit sendVariant( QVariant::fromValue(spServices) );
        }
        else if ( SupervisorProto::Request::Groups == req )
        {
            GroupsHelper::TGroups groups = GroupsHelper::groupsInformation();

            SupervisorProto::Groups spGroups;
            foreach ( const GroupsHelper::GroupInfo &gi, groups )
            {
                SupervisorProto::Group g = { gi.name, gi.visualName };
                spGroups << g;
            }

            emit sendVariant( QVariant::fromValue(spGroups) );
        }
        else if ( SupervisorProto::Request::ActiveGroup == req )
        {
            SupervisorProto::ActiveGroup activeGroup = { GroupsHelper::activeGroup() };
            emit sendVariant( QVariant::fromValue(activeGroup) );
        }
        else if ( SupervisorProto::Request::AvailableNetworks == req )
        {
            NetworkList list = mNetController->getList();
            setNetList(list);
            qLogInfo(tr("Client")) << tr(" Network status changed. Number of available networks: %1")
                                      .arg(QString::number(list.count()));
        }
        else if ( SupervisorProto::Request::AvailableScripts == req )
        {
            SupervisorProto::ScriptList list;
            list.scripts = mScriptController->names();
            emit sendVariant( QVariant::fromValue(list) );
        }
        else if ( SupervisorProto::Request::Logs == req )
        {
            SupervisorProto::AvailableLogs list;
            list.names = mLogController->names();
            emit sendVariant( QVariant::fromValue(list) );
        }
    }
    // Request Status
    else if ( v.canConvert<SupervisorProto::RequestStatus>() )
    {
        SupervisorProto::RequestStatus reqStatus = v.value<SupervisorProto::RequestStatus>();

        ApplicationController::TController ctrl = actrl->findController(reqStatus.name);
        if ( !ctrl.isNull() )
        {
            emit sendStatus((Type)reqStatus.type, reqStatus.name, ctrl->status());

            if ((Type)reqStatus.type == FMTP)
            {
                EFMTPStatus statusFmtp = enDisable;
                if ( qobject_cast<FMTPProcessController *>(ctrl.data()) )
                    statusFmtp = qobject_cast<FMTPProcessController *>(ctrl.data())->fmtpStatus();
                else if ( qobject_cast<FMTPServiceController *>(ctrl.data()) )
                    statusFmtp = qobject_cast<FMTPServiceController *>(ctrl.data())->fmtpStatus();

                emit sendFmtpStatus(reqStatus.name, statusFmtp);
            }
            else if ((Type)reqStatus.type == CWP)
            {
                CWPProcessController *cwp = qobject_cast<CWPProcessController *>(ctrl.data());
                if ( cwp )
                {
                    emit sendCWPSectors(reqStatus.name, cwp->client(), cwp->sectors());
                    emit sendCWPUser(reqStatus.name, cwp->user());
                }
            }
            else if ((Type)reqStatus.type == FDP)
            {
                bool fdpStatus = false;
                if ( qobject_cast<FDPProcessController *>(ctrl.data()) )
                    fdpStatus = qobject_cast<FDPProcessController *>(ctrl.data())->fdpStatus();
                else if ( qobject_cast<FDPServiceController *>(ctrl.data()) )
                    fdpStatus = qobject_cast<FDPServiceController *>(ctrl.data())->fdpStatus();

                emit sendFdpStatus(reqStatus.name, fdpStatus);
            }
            else if ((Type)reqStatus.type == ADP)
            {
                QString adpStatus;
                if ( qobject_cast<ADPProcessController *>(ctrl.data()) )
                    adpStatus = qobject_cast<ADPProcessController *>(ctrl.data())->adpStatus();
                else if ( qobject_cast<ADPServiceController *>(ctrl.data()) )
                    adpStatus = qobject_cast<ADPServiceController *>(ctrl.data())->adpStatus();

                emit sendAdpStatus(reqStatus.name, adpStatus);
            }

            NetworkList list = mNetController->getList();
            setNetList(list);
        }
    }
    // Request special status
    else if ( v.canConvert<SupervisorProto::RequestSpecialStatus>() )
    {
        SupervisorProto::RequestSpecialStatus reqStatus = v.value<SupervisorProto::RequestSpecialStatus>();

        ApplicationController::TController ctrl = actrl->findController(reqStatus.name);
        if ( !ctrl.isNull() )
        {
            if ((Type)reqStatus.type == FMTP)
            {
                EFMTPStatus statusFmtp = enDisable;
                if ( qobject_cast<FMTPProcessController *>(ctrl.data()) )
                    statusFmtp = qobject_cast<FMTPProcessController *>(ctrl.data())->fmtpStatus();
                else if ( qobject_cast<FMTPServiceController *>(ctrl.data()) )
                    statusFmtp = qobject_cast<FMTPServiceController *>(ctrl.data())->fmtpStatus();

                emit sendFmtpStatus(reqStatus.name, statusFmtp);
            }
            else if ((Type)reqStatus.type == CWP)
            {
                CWPProcessController *cwp = qobject_cast<CWPProcessController *>(ctrl.data());
                if ( cwp )
                {
                    emit sendCWPSectors(reqStatus.name, cwp->client(), cwp->sectors());
                    emit sendCWPUser(reqStatus.name, cwp->user());
                }
            }
            else if ((Type)reqStatus.type == FDP)
            {
                bool fdpStatus = false;
                if ( qobject_cast<FDPProcessController *>(ctrl.data()) )
                    fdpStatus = qobject_cast<FDPProcessController *>(ctrl.data())->fdpStatus();
                else if ( qobject_cast<FDPServiceController *>(ctrl.data()) )
                    fdpStatus = qobject_cast<FDPServiceController *>(ctrl.data())->fdpStatus();

                emit sendFdpStatus(reqStatus.name, fdpStatus);
            }
            else if ((Type)reqStatus.type == ADP)
            {
                QString adpStatus;
                if ( qobject_cast<ADPProcessController *>(ctrl.data()) )
                    adpStatus = qobject_cast<ADPProcessController *>(ctrl.data())->adpStatus();
                else if ( qobject_cast<ADPServiceController *>(ctrl.data()) )
                    adpStatus = qobject_cast<ADPServiceController *>(ctrl.data())->adpStatus();

                emit sendAdpStatus(reqStatus.name, adpStatus);
            }

            NetworkList list = mNetController->getList();
            setNetList(list);
        }
    }
    // Request Log
    else if ( v.canConvert<SupervisorProto::RequestLog>() )
    {
        SupervisorProto::RequestLog reqLog = v.value<SupervisorProto::RequestLog>();

        SupervisorProto::Log l;
        l.type = reqLog.type;
        l.name = reqLog.name;

        if (reqLog.type == SupervisorProto::Server)
        {
            l.log = log();
        } else if (reqLog.type == SupervisorProto::LOG)
        {
            l.log = mLogController->readLog(reqLog.name);
        } else
        {
            l.log = actrl->readLog(reqLog.name);
        }
        emit sendVariant( QVariant::fromValue(l) );
    }
    // Request Remote operations
    else if ( v.canConvert<SupervisorProto::RequestRemoteOperations>() )
    {
        SupervisorProto::RequestRemoteOperations reqLog =
                v.value<SupervisorProto::RequestRemoteOperations>();

        SupervisorProto::Log l;
        l.name = reqLog.name;

        l.log = actrl->readLog(reqLog.name, reqLog.from, reqLog.to);

        SupervisorProto::RemoteOperations ro;
        ro = actrl->parseLog(l.log);

        emit sendVariant( QVariant::fromValue(ro) );
    }
    // Request Remote Logs
    else if ( v.canConvert<SupervisorProto::RequestRemoteLogs>() )
    {
        qDebug() << "Remote Logs Requested!";

        SupervisorProto::RequestRemoteLogs reqLog =
                v.value<SupervisorProto::RequestRemoteLogs>();

        const ApplicationController::TControllerLogs logs = actrl->readLog(reqLog.from, reqLog.to);

        qDebug() << "Logs collected: " << logs.count();

        foreach (const SupervisorProto::RemoteLog &log, logs) {
            emit sendVariant(QVariant::fromValue(log));
        }
    }
    // Restart service
    else if ( v.canConvert<SupervisorProto::Restart>() )
    {
        SupervisorProto::Restart restart = v.value<SupervisorProto::Restart>();
        qLogInfo(tr("Client")) << tr(" Restarting %1 ...")
                                  .arg(restart.name);
        QObject::staticMetaObject.invokeMethod(actrl, "restart", Qt::QueuedConnection, Q_ARG(QString, restart.name));
    }
    // Stop all services
    else if ( v.canConvert<SupervisorProto::StopAll>() )
    {
        QObject::staticMetaObject.invokeMethod(actrl, "stop", Qt::QueuedConnection);
    }
    // Start all services
    else if ( v.canConvert<SupervisorProto::StartAll>() )
    {
        QObject::staticMetaObject.invokeMethod(actrl, "start", Qt::QueuedConnection);
    }
    // Activate FDP
    else if ( v.canConvert<SupervisorProto::ActivateFdp>() )
    {
        SupervisorProto::ActivateFdp a = v.value<SupervisorProto::ActivateFdp>();

        ApplicationController::TController ctrl = actrl->findController(a.name);
        if ( !ctrl.isNull() && FDP == ctrl->type() )
        {
            if ( qobject_cast<FDPProcessController *>(ctrl.data()) )
            {
                QObject::staticMetaObject.invokeMethod(qobject_cast<FDPProcessController *>(ctrl.data()),
                                                       "activate", Qt::QueuedConnection);
            }
            else if ( qobject_cast<FDPServiceController *>(ctrl.data()) )
            {
                QObject::staticMetaObject.invokeMethod(qobject_cast<FDPServiceController *>(ctrl.data()),
                                                       "activate", Qt::QueuedConnection);
            }
        }
    }
    // Start sript
    else if ( v.canConvert<SupervisorProto::StartScript>() )
    {
        SupervisorProto::StartScript a = v.value<SupervisorProto::StartScript>();
        QObject::staticMetaObject.invokeMethod(mScriptController, "startScript"
                                               , Qt::QueuedConnection
                                               , Q_ARG(const QString &, a.name)
                                               , Q_ARG(const QString &, a.parameter));
    }
    // Stop sript
    else if ( v.canConvert<SupervisorProto::StopScript>() )
    {
        SupervisorProto::StopScript a = v.value<SupervisorProto::StopScript>();
        QObject::staticMetaObject.invokeMethod(mScriptController, "stopScript"
                                               , Qt::QueuedConnection
                                               , Q_ARG(const QString &, a.name));
    }
    // Shutdown computer
    else if ( v.canConvert<SupervisorProto::Shutdown>() )
    {
        shutdown();
    }
    // Reboot computer
    else if ( v.canConvert<SupervisorProto::Reboot>() )
    {
        reboot();
    }
    // Set SNMP variable
    else if ( v.canConvert<SupervisorProto::SetSnmpVariable>() )
    {
        SupervisorProto::SetSnmpVariable a = v.value<SupervisorProto::SetSnmpVariable>();
        QObject::staticMetaObject.invokeMethod(mSnmpController, "setVariable"
                                               , Qt::QueuedConnection
                                               , Q_ARG(const QString &, a.agent)
                                               , Q_ARG(const QString &, a.variable)
                                               , Q_ARG(const QString &, a.value));
    }
}

void RequestThread::setNetList(const NetworkList &list)
{
    SupervisorProto::NetList r;
    foreach (const QNetworkInterface &interface, list)
    {
        QString s_int = interface.humanReadableName();
        QString sep = " (";
        foreach (const QNetworkAddressEntry &address, interface.addressEntries())
        {
            if (address.ip().toIPv4Address())
            {
                s_int += (sep + address.ip().toString());
                sep = ", ";
            }
        }
        if (sep == ", ")
            s_int += ")";
        r.list.append(s_int);
    }

    emit sendVariant( QVariant::fromValue(r) );
}

void RequestThread::shutdown()
{
    qLogInfo(tr("Client")) << tr(" Shutting down computer...");
    QObject::staticMetaObject.invokeMethod(mAppCtrl, "stop", Qt::QueuedConnection);
    ComputerHelper::shutdown();
}

void RequestThread::reboot()
{
    qLogInfo(tr("Client")) << tr(" Rebooting computer...");
    QObject::staticMetaObject.invokeMethod(mAppCtrl, "stop", Qt::QueuedConnection);
    ComputerHelper::reboot();
}

QStringList RequestThread::log(int countLines)
{
    QStringList lines;
    int currentCountLines = countLines;
    int backupIndex = 1;
    QString logPath = QDir("bin:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
    QString logBackupPath = logPath;
    while ( lines.size() < countLines )
    {
        QStringList currentLines = readStream(logBackupPath, currentCountLines);
        currentCountLines -= currentLines.size();

        // Store older lines before.
        lines = currentLines + lines;

        if ( currentCountLines < 1 )
            break;

        // Try to read log from the next log if not all lines are read.
        logBackupPath = QString("%1.%2").arg(logPath).arg(backupIndex++);
        if ( !QFile::exists(logBackupPath) )
            break;
    }

    return packLines(lines);
}
