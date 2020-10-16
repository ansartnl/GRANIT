#include <QApplication>
#include <QMessageBox>
#include "SupervisorView.h"

#include "main.h"
#include "CmdLineArguments.h"
#include "InitMain.h"
#include "Log.h"

#include "log4qt.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"

#include "SupervisorMonitor.h"

#include "usUsers.h"
#include "usGroups.h"
#include "usTransport.h"
#include "UserClient.h"
#include "UserServer.h"
#include "LoginDialog.h"

QSettings* globalSettings(const QString & pathFile)
{
    static QSettings* gStg = new QSettings(pathFile, QSettings::IniFormat, qApp);
    return gStg;
}

QSettings& xmlSettings(const QString &pathConfig)
{
    static QtXmlSettings xmlStg(pathConfig);
    return xmlStg;
}

SupervisorMonitor* supervisorMonitor()
{
    static SupervisorMonitor* m = new SupervisorMonitor();
    return m;
}


SMCLogManager &logManager()
{
    static SMCLogManager manager;
    return manager;
}

QScriptEngine* scriptEngine()
{
    static QScriptEngine* e = new QScriptEngine();
    return e;
}

QString & buildDate()
{
    static QString date(__DATE__);
    return date;
}

void fillGroups()
{
    UserSystem::userServer()->addGroup(new UserSystem::GroupRoot);
    UserSystem::userServer()->addGroup(new UserSystem::GroupAdministrators);
    UserSystem::userServer()->addGroup(new UserSystem::GroupOperators);
    //UserSystem::userServer()->addGroup(new UserSystem::GroupUsers);
}

void setUserServer()
{
    QSettings settings;
    UserSystem::userServer()->fromByteArray(QByteArray::fromHex(settings.value("GroupData", "").toByteArray())
                                            , QByteArray::fromHex(settings.value("UserData", "").toByteArray()));
    if (UserSystem::userServer()->groupsIsEmpty())
        fillGroups();
}

void initUserSystem(QObject *parent)
{
    UserSystem::Transport *clientTransport = new UserSystem::Transport(parent);
    UserSystem::Transport *serverTransport = new UserSystem::Transport(parent);

    QObject::connect(serverTransport, SIGNAL(send(QVariant)), clientTransport, SLOT(onDataReceived(QVariant)));
    QObject::connect(clientTransport, SIGNAL(send(QVariant)), serverTransport, SLOT(onDataReceived(QVariant)));

    UserSystem::userClient(clientTransport, parent);
    UserSystem::userServer(parent);
    UserSystem::userServer()->addTransport(serverTransport);
}

void initLog4Qt(QObject *parent)
{
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("[%d{locale:short}] %p %c %x - %m%n", parent);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    QString sLogName = globalSettings()->value("LogFileName",
                                               QFileInfo(qApp->applicationFilePath()).baseName()).toString();

    if(!sLogFile.isEmpty() && !sLogFile.endsWith('/'))
        sLogFile.append('/');

    sLogFile += sLogName + QDate::currentDate().toString("_yyyyMMdd") + ".log";

    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, sLogFile, true, parent);
    rootFileAppender->setName("rootFileAppender");
    uint fileSize = globalSettings()->value("LogFileSize", 50).toUInt();
    rootFileAppender->setMaximumFileSize(fileSize * 1024 * 1024); // 50MB
    rootFileAppender->setMaxBackupIndex(0);
    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName( "atm" );
    QCoreApplication::setApplicationName("monitor");

    // Init QSettings static accessor.
    globalSettings(QApplication::applicationDirPath() + "/" +
                   QApplication::applicationName() + ".settings");

    InitMain::setupSearchPath();

    //a.setWindowIcon(QIcon(QDir("images:").absoluteFilePath("SupervisorGui.ico")));

//    QtTranslator::instance()->loadTranslations(QDir("translations:"));
//    QtTranslator::instance()->loadLanguageFromSettings();

    QString pathConfigFile = InitMain::findConfigFilePath();

    // Initialize xml settings technique.
    xmlSettings(pathConfigFile);

    logManager().initLog4Qt(qApp);
    if(globalSettings()->value("EnableAsterixLog", false).toBool())
        logManager().initAsterixLogger();
    //initLog4Qt(qApp);

    qLogInfo("main") << " --------------------------------------- ";
    qLogInfo("main") << QObject::tr(" Application ") << qApp->applicationName() << QObject::tr(" started");
    qLogInfo("main") << QObject::tr(" Program version: ") << qApp->applicationVersion();
    qLogInfo("main") << QObject::tr(" Build date: ") << buildDate();
    qLogInfo("main") << QObject::tr(" Config file: ") << pathConfigFile;

    initUserSystem(qApp);
    setUserServer();

    CmdLineArguments &cmdLine = CmdLineArguments::instance();

    QSettings settings;

    LoginDialog dialog;
    dialog.setUserClient(UserSystem::userClient());
    QString username = cmdLine.username();
    bool tryLoginWithoutDialog = !username.isEmpty();
    if ( username.isEmpty() )
        username = settings.value(s_userName, "root").toString();

    // Try to login without popping up the modal dialog.
    if ( !tryLoginWithoutDialog || !dialog.attemptToLogin(username, cmdLine.password()) )
    {
        dialog.setUserName(username);
    }

    settings.setValue(s_userName, dialog.userName());

    supervisorMonitor()->setUserSystem();
    supervisorMonitor()->show();

    // Make sure configuration exists.
    if ( !pathConfigFile.isEmpty() )
        supervisorMonitor()->openFile(pathConfigFile);

    int result = a.exec();

    settings.setValue("GroupData", UserSystem::userServer()->groupsToByteArray().toHex());
    settings.setValue("UserData", UserSystem::userServer()->usersToByteArray().toHex());

    qLogInfo("main") << QObject::tr(" Finished");

    delete scriptEngine();
    delete supervisorMonitor();

    return result;
}

