#include "SupervisorService.h"

#include "init.h"
#include "ClientConnection.h"
#include "ScriptController.h"
#include "DatabaseController.h"
#include "ApplicationController.h"
#include "LogController.h"
#include "FolderController.h"
#include "SnmpController.h"
#include "GroupsHelper.h"

#include "SpProto.h"

#include "InitMain.h"
#include "Log.h"
#include "qtxmlsettings.h"

#if defined(SS_NO_GUI)
#else
#include "qttranslator.h"
#endif

#include <QtCore/QFile>
#include <QtCore/QMutex>
#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtNetwork/QTcpServer>

#include <stdio.h>
#include <stdlib.h>

using namespace InitMain;

SupervisorService::SupervisorService(int argc, char** argv, const QString &name)
    : QObject(),//QtService<QApplication>(argc, argv, name),
      mManagementServer(0)
{
    Q_UNUSED(name)
//    setServiceDescription("Supervisor service. Starts CWP or FDP working place "
//                          "and control all processes used for working place.");
//    setStartupType(QtServiceController::AutoStartup);

    mManagementServer = new QTcpServer(this);
    connect(mManagementServer, SIGNAL(newConnection()), SLOT(on_newConnection()));

    mScriptController = new ScriptController(this);

    mDatabaseController = new DatabaseController(this);
    mApplicationController = new ApplicationController(mDatabaseController, this);

    mLogController = new LogController(this);

    mFolderController = new FolderController(this);

    mSnmpController = new SnmpController(this);

    connect(mApplicationController, SIGNAL(started()), SLOT(on_servicesStarted()));

    createApplication(argc, argv);
}

SupervisorService::~SupervisorService()
{
    stop();
}

void SupervisorService::createApplication(int &argc, char **argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    //QtService<QApplication>::createApplication(argc, argv);

    QThread::currentThread()->setObjectName("main");

    InitMain::setupSearchPath();

    mPathConfig = InitMain::findConfigFilePath();

    try
    {
        if ( mPathConfig.isEmpty() )
        {
            QString exString;
            QTextStream stream(&exString);
            stream << QObject::tr("Configuration file is not found.") << endl
                   << InitMain::configUsageString();

            throw exString;
        }

        QtXmlSettings settings(mPathConfig);

        // Load translations if exist.
#if defined(SS_NO_GUI)
#else
        bool isLangRussian = settings.value("language", "english").toString().toLower() == "russian";
        QtTranslator::instance()->loadTranslations(QDir("translations:"));
        QtTranslator::instance()->setCurrentLanguage(isLangRussian ? "Russian" : "English");
#endif
        if ( settings.value("log", true).toBool() )
        {
            initLog4Qt(qApp);
        }
        qLogInfo(tr("Server")) << tr(" =========> NEW_SESSION_STARTED <=========");
        qLogInfo(tr("Server")) << tr(" %1, Version %2").arg(qApp->applicationName(), qApp->applicationVersion());
        qLogInfo(tr("Server")) << tr(" Build date: %1").arg(__DATE__);
        qLogInfo(tr("Server")) << tr(" Config file: %1").arg(mPathConfig);
        qLogInfo(tr("Server")) << tr(" Protocol version: %1").arg( SupervisorProto::version() ) << "\n";
    }
    catch ( const QString &ex )
    {
        qLogWarn(tr("Server")) << ex;
    }
}

void SupervisorService::start()
{
    // If no configuration - do nothing.
    if ( mPathConfig.isEmpty() )
        return;

    QtXmlSettings &settings = *static_cast<QtXmlSettings *>( xmlSettings(mPathConfig) );

    int killTimeout = settings.value("kill_timeout_ms", 5000).toInt();

    QString activeGroupName;
    // Read information about groups.
    settings.beginGroup("groups");
    {
        foreach ( const QString &group, settings.childGroups() )
        {
            if ( !checkSettingsKey(settings, QString("%1/%2").arg(group).arg(QtXmlSettings::makeAttributeName("id")))
                 || !checkSettingsKey(settings, QString("%1/%2").arg(group).arg(QtXmlSettings::makeAttributeName("visualName"))) )
                continue;

            QString groupName = settings.value(QString("%1/%2").arg(group).arg(QtXmlSettings::makeAttributeName("id"))).toString();
            activeGroupName = groupName;
            GroupsHelper::createGroup(groupName,
                                      settings.value(QString("%1/%2").arg(group).arg(QtXmlSettings::makeAttributeName("visualName"))).toString());

            settings.beginGroup( group );
            foreach ( const QString &proc, settings.childGroups() )
            {
                GroupsHelper::addService(groupName,
                                         settings.value(proc).toString(),
                                         settings.value(QString("%1/%2").arg(proc).arg(QtXmlSettings::makeAttributeName("order"))).toInt());
            }
            settings.endGroup();
        }
    }
    settings.endGroup();

    int groupsNumber = GroupsHelper::groups().size();
    // If no defined groups - do not proceed.
    if ( groupsNumber < 1 )
    {
        qLogInfo(tr("Server")) << tr("No defined groups. Check <groups> tag in documentation.");
        throw 0;    // exit from supervisor - see main.cpp
        return;
    }

    // Read active group identifier or set the one group as active.
    if ( groupsNumber > 1 )
    {
        QString activeGroupConfig = settings.value("active_group").toString();
        if ( GroupsHelper::contains(activeGroupConfig) )
            activeGroupName = activeGroupConfig;
        else
            qLogInfo(tr("Server")) << tr("Active group is incorrect. Use '%1' group as active.").arg(activeGroupName);
    }
    GroupsHelper::setActiveGroup( activeGroupName );

    // Read database configuration.
    settings.beginGroup("databases");
    {
        foreach ( const QString &db, settings.childGroups() )
        {
            if ( !checkSettingsKey(settings, QString("%1/%2").arg(db).arg(QtXmlSettings::makeAttributeName("id"))) )
                continue;

            QString id = settings.value(QString("%1/%2").arg(db).arg(QtXmlSettings::makeAttributeName("id"))).toString();

            if ( !checkSettingsKey(settings, db + "/driver")
                 || !checkSettingsKey(settings, db + "/host")
                 || !checkSettingsKey(settings, db + "/port")
                 || !checkSettingsKey(settings, db + "/name")
                 || !checkSettingsKey(settings, db + "/user")
                 || !checkSettingsKey(settings, db + "/password") )
                continue;

            settings.beginGroup( db );

            mDatabaseController->addDatabase(id,
                                             settings.value("driver", "QOCI").toString(),
                                             settings.value("host", "127.0.0.1").toString(),
                                             settings.value("port", 1521).toInt(),
                                             settings.value("name", "XE").toString(),
                                             settings.value("user").toString(),
                                             settings.value("password").toString()
                                             );

            settings.endGroup();
        }
    }
    settings.endGroup();

    // Read database services.
    settings.beginGroup("services");
    {
        ApplicationController::TServiceOptionList services;

        foreach ( const QString &serv, settings.childGroups() )
        {
            if ( !checkSettingsKey(settings, QString("%1/%2").arg(serv).arg(QtXmlSettings::makeAttributeName("id"))) )
                continue;

            QString id = settings.value(QString("%1/%2").arg(serv).arg(QtXmlSettings::makeAttributeName("id"))).toString();

            if ( !checkSettingsKey(settings, serv + "/visualName")
                 || !checkSettingsKey(settings, serv + "/name") )
                continue;

            settings.beginGroup( serv );

            ApplicationController::ServiceOption opt;
            opt.id = id;
            opt.type = SERVICE;
            if ( settings.value("fmtp").toBool() )
                opt.type = FMTP;
            else if ( settings.value("fdp").toBool() )
                opt.type = FDP;
            else if ( settings.value("adp").toBool() )
            {
                QMap<QString, QString> specificValues;
                QString statusMonitoringPath = settings.value("status_path").toString();

                specificValues.insert("status_path", statusMonitoringPath);

                opt.type = ADP;
                opt.otherOptions = ::toString(specificValues);
            }
            opt.namePath = settings.value("name").toString();
            opt.arguments = settings.value("arguments").toString();
            opt.visualName = settings.value("visualName").toString();
            opt.description = settings.value("description").toString();
            opt.configPath = settings.value("config_path").toString();
            opt.logPath = settings.value("log_path").toString();
            opt.isRestartable = settings.value("restart").toBool();
            opt.database = settings.value("database").toString();

            services << opt;

            settings.endGroup();
        }

        mApplicationController->addServices( services );
    }
    settings.endGroup();

    // Read database processes.
    settings.beginGroup("processes");
    {
        ApplicationController::TServiceOptionList processes;

        foreach ( const QString &proc, settings.childGroups() )
        {
            if ( !checkSettingsKey(settings, QString("%1/%2").arg(proc).arg(QtXmlSettings::makeAttributeName("id"))) )
                continue;

            QString id = settings.value(QString("%1/%2").arg(proc).arg(QtXmlSettings::makeAttributeName("id"))).toString();

            if ( !checkSettingsKey(settings, proc + "/visualName")
                 || !checkSettingsKey(settings, proc + "/path") )
                continue;

            settings.beginGroup( proc );

            ApplicationController::ServiceOption opt;
            opt.id = id;
            opt.type = PROCESS;
            if ( settings.value("cwp").toBool() )
                opt.type = CWP;
            else if ( settings.value("fmtp").toBool() )
                opt.type = FMTP;
            else if ( settings.value("fdp").toBool() )
                opt.type = FDP;
            else if ( settings.value("adp").toBool() )
            {
                QMap<QString, QString> specificValues;
                QString statusMonitoringPath = settings.value("status_path").toString();

                specificValues.insert("status_path", statusMonitoringPath);

                opt.type = ADP;
                opt.otherOptions = ::toString(specificValues);
            }
            opt.namePath = settings.value("path").toString();
            opt.arguments = settings.value("arguments").toString();
            opt.visualName = settings.value("visualName").toString();
            opt.description = settings.value("description").toString();
            opt.configPath = settings.value("config_path").toString();
            opt.logPath = settings.value("log_path").toString();
            opt.isRestartable = settings.value("restart").toBool();
            opt.database = settings.value("database").toString();
            opt.killTimeout = settings.value("kill_timeout_ms", killTimeout).toInt();

            processes << opt;

            settings.endGroup();
        }

        mApplicationController->addProcesses( processes );
    }
    settings.endGroup();

    // Read script settings
    {
        settings.beginGroup("scripts");

        foreach(const QString &g, settings.childGroups())
        {
            if ((!checkSettingsKey(settings, g + "/name"))
                    || (!checkSettingsKey(settings, g + "/path")))
                continue;

            settings.beginGroup(g);
            mScriptController->addScript(settings.value("name").toString()
                                         , settings.value("path").toString());
            settings.endGroup();
        }

        settings.endGroup();
    }

    // Read SNMP settings
    {
        settings.beginGroup("snmp");

        foreach (const QString &child, settings.childGroups()) {
            settings.beginGroup(child);

            SnmpController::Agent agent;

            settings.beginGroup("variables");
            foreach (const QString &variable, settings.childGroups()) {
                settings.beginGroup(variable);
                QString name = settings.value("name").toString();
                QString oid = settings.value("oid").toString();

                if (settings.value("read", false).toBool()) {
                    agent.monitoredVariables.insert(name, oid);
                }

                if (settings.value("write", false).toBool()) {
                    SnmpController::Variable v;
                    v.oid = oid;
                    v.type =  settings.value("type", 's').toString().at(0).toLatin1();
                    agent.variables.insert(name, v);
                }

                settings.endGroup();
            }
            settings.endGroup();    // "variables"

            settings.beginGroup("traps");
            foreach (const QString &trap, settings.childGroups()) {
                settings.beginGroup(trap);
                agent.traps.insert(settings.value("name").toString(), settings.value("oid").toString());
                settings.endGroup();
            }
            settings.endGroup();    // "traps"

            QString host = settings.value("host").toString();
            agent.name = settings.value("name", host).toString();

            mSnmpController->addHost(host,
                                     settings.value("community").toString(),
                                     settings.value("period", 0).toInt(),
                                     agent);

            settings.endGroup();
        }

        settings.endGroup();
    }

    // Read Tracked folder settings
    {
        settings.beginGroup("tracked_folder");

        if(settings.value("enabled", false).toBool())
        {
            mFolderController->setMonitoredFolder(settings.value("path").toString());
            mFolderController->setCheckInterval(settings.value("period", 8).toUInt() * 1000);
        }

        settings.endGroup();
    }

    // Read log settings
    {
        settings.beginGroup("logs");

        foreach(const QString &g, settings.childGroups())
        {
            if ((!checkSettingsKey(settings, g + "/name"))
                    || (!checkSettingsKey(settings, g + "/path")))
                continue;

            settings.beginGroup(g);
            mLogController->addLog(settings.value("name").toString()
                                   , settings.value("path").toString());
            settings.endGroup();
        }

        settings.endGroup();
    }

    mDatabaseController->setMonitoredDatabases( mApplicationController->activeDatabaseList() );

    if (settings.value("autostart").toBool())
        connect(mDatabaseController, SIGNAL(ready()), mApplicationController, SLOT(start()), Qt::QueuedConnection);
    connect(mDatabaseController, SIGNAL(ready()), SLOT(on_databaseReady()), Qt::QueuedConnection);

    // Run database connection threads sequentially one by one.
    // After all connections established - run processes/services.
    mDatabaseController->start();

    // Start management server.
    {
        checkSettingsKey(settings, "network/host");
        checkSettingsKey(settings, "network/port");

        QString host = settings.value("network/host").toString();
        quint16 port = settings.value("network/port").toUInt();
        if ( mManagementServer->listen(QHostAddress(host), port) )
        {
            qLogInfo(tr("Server")) << tr(" Listening of network address '%1:%2'' started successfully. Management connection.")
                                      .arg(host).arg(port);
        } else
        {
            qLogWarn(tr("Server")) << tr(" Management host '%1' and '%2' port is not available. Listen failed")
                                      .arg(host).arg(port);
        }
    }
}

void SupervisorService::stop()
{
    qLogInfo(tr("Server")) << tr(" Stop command received");
    mManagementServer->close();

    mDatabaseController->disconnect();
    mDatabaseController->stop();

    // Stop all process/services in reverse order they defined in the config.
    mApplicationController->stop();

    qLogInfo(tr("Server")) << tr(" Stopping finished");
}

bool SupervisorService::checkSettingsKey(const QSettings &s, const QString &key)
{
    if (!s.contains(key))
    {
        if (s.group().isEmpty())
            qLogError(tr("Server")) << tr(" Could not find key '%1' in config")
                                      .arg(key);
        else
            qLogError(tr("Server")) << tr(" Could not find key '%1' in config")
                                      .arg(s.group() + "/" + key);
        return false;
    }
    return true;
}

void SupervisorService::on_newConnection()
{
    while ( mManagementServer->hasPendingConnections() )
    {
        QTcpSocket *s = mManagementServer->nextPendingConnection();

        ClientConnection *clientConnection = new ClientConnection(this, s, this);
        mApplicationController->connectTo(clientConnection);

        connect(this, SIGNAL(sendCommand(QVariant)), clientConnection, SLOT(on_commandReceived(QVariant)));

        qLogInfo(tr("Server")) << tr(" New connection from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort());
    }
}

void SupervisorService::on_servicesStarted()
{
    // Let all Tcp clients know about new services monitorring by the server.
    SupervisorProto::Request::Request req = SupervisorProto::Request::Services;
    emit sendCommand( QVariant::fromValue(req) );
}

void SupervisorService::on_databaseReady()
{
    // Let all Tcp clients know about possible changes of an active group.
    SupervisorProto::Request::Request req = SupervisorProto::Request::ActiveGroup;
    emit sendCommand( QVariant::fromValue(req) );
}
