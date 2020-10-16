#include "main.h"

#include "init.h"
#include "MainWindow.h"
#include "ExtendedStyle.h"
#include "CmdLineArguments.h"

#include "InitMain.h"
#include "Log.h"
#include "qttranslator.h"
#include "qtxmlsettings.h"

#include "SpProto.h"
#include "version.h"

#include <QtCore/QDir>

#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

#include "usUsers.h"
#include "usGroups.h"
#include "UserClient.h"
#include "UserServer.h"
#include "LoginDialog.h"

QSettings& xmlSettings(const QString &pathConfig)
{
    static QtXmlSettings xmlStg(pathConfig);
    return xmlStg;
}

void setUserServer();
void fillGroups();

QString & buildDate()
{
    static QString date(__DATE__);
    return date;
}

void loadExtendedStyles()
{
    QString pathExtendedStyleFile = CmdLineArguments::instance().extendedStyle();
    // Load extended styles from the command line argument or from the 'etc/%AppName%' file.
    if ( pathExtendedStyleFile.isEmpty() )
        pathExtendedStyleFile = QDir("etc:").absoluteFilePath(qApp->applicationName()+".es");

    if ( QFile(pathExtendedStyleFile).exists() )
    {
        ExtendedStyle es;
        es.loadFromFile(pathExtendedStyleFile);
        qApp->setPalette(es.palette);
        qApp->setStyle(es.style);
    }
}

int main(int argc, char *argv[])
{
    InitMain::setupCodec();

    QApplication a(argc, argv);

    a.setOrganizationName( "ITOS" );
    a.setApplicationName( "SupervisorMonitor" );
    a.setApplicationVersion( VERSION_FULL );

    InitMain::setupSearchPath();

    a.setWindowIcon(QIcon(QDir("images:").absoluteFilePath("SupervisorGui.ico")));

    QtTranslator::instance()->loadTranslations(QDir("translations:"));
    QtTranslator::instance()->loadLanguageFromSettings();

    QString pathConfigFile = InitMain::findConfigFilePath();

    // Make sure configuration exists.
    if ( pathConfigFile.isEmpty() )
    {
        QMessageBox::warning(0, QObject::tr("Configuration"),
                             QObject::tr("Configuration file is not found.\n\n") +
                             InitMain::configUsageString());
        return 0;
    }

    loadExtendedStyles();

    // Initialize xml settings technique.
    xmlSettings(pathConfigFile);

    if (xmlSettings().value("log", true).toBool())
    {
        initLog4Qt(qApp);
    }
    qLogInfo("main") << " --------------------------------------- ";
    qLogInfo("main") << QObject::tr(" Application ") << qApp->applicationName() << QObject::tr(" started");
    qLogInfo("main") << QObject::tr(" Program version: ") << qApp->applicationVersion();
    qLogInfo("main") << QObject::tr(" Build date: ") << buildDate();
    qLogInfo("main") << QObject::tr(" Config file: ") << pathConfigFile;
    qLogInfo("main") << QObject::tr(" Protocol version: %1").arg( SupervisorProto::version() );

    // Init main app font
    {
        QSettings settings;
        QVariant var = settings.value("AppSettings");
        if (!var.isNull() && var.isValid())
        {
            AppSettings appsettings;
            appsettings.fromByteArray(var.toByteArray());
            qApp->setFont(appsettings.getFont());
        }
    }

    QtTranslator::instance()->setCurrentLanguage(
                xmlSettings().value("language", "russian").toString().toLower() == "english"
                ? QString("English") : QString("Русский") );

    int ret = 0;

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

    bool isLoginAccepted = true;
    // Try to login without popping up the modal dialog.
    if ( !tryLoginWithoutDialog || !dialog.attemptToLogin(username, cmdLine.password()) )
    {
        dialog.setUserName(username);
        // Login is not successfull or there are no command line arguments specified.
        // Login with the modal dialog.
        if (dialog.exec() != QDialog::Accepted)
            isLoginAccepted = false;
    }

    if ( isLoginAccepted )
    {
        settings.setValue(s_userName, dialog.userName());
        MainWindow w;
        w.setUserSystem();
        w.show();
        ret = a.exec();
    }

    qLogInfo("main") << QObject::tr(" Finished");
    return ret;
}

void setUserServer()
{
    QSettings settings;
    UserSystem::userServer()->fromByteArray(QByteArray::fromHex(settings.value("GroupData", "").toByteArray())
                                            , QByteArray::fromHex(settings.value("UserData", "").toByteArray()));
    if (UserSystem::userServer()->groupsIsEmpty())
        fillGroups();
}

void fillGroups()
{
    UserSystem::userServer()->addGroup(new UserSystem::GroupAdministrators);
    UserSystem::userServer()->addGroup(new UserSystem::GroupOperators);
    UserSystem::userServer()->addGroup(new UserSystem::GroupUsers);
}
