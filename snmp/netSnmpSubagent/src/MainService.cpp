#include "MainService.h"

#include "main.h"
#include "helpers/Config.h"
#include "InternalServer.h"
#include "ServiceStarter.h"
#include "Subagent.h"

#include "ColorConsoleLayout.h"
#include "InitMain.h"
#include "Log.h"

// log4qt headers

#ifndef LOG4QT_LINKED_JUDICIOUSLY
#include "colorconsoleappender.h"
#include "log4qt.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#else
#include <Log4Qt/colorconsoleappender.h>
#include <Log4Qt/log4qt.h>
#include <Log4Qt/logmanager.h>
#include <Log4Qt/patternlayout.h>
#include <Log4Qt/rollingfileappender.h>
#endif

#include <QtCore/QDir>
#include <QtCore/QTranslator>

MainService::MainService(int argc, char **argv, const QString &name) :
    QtService < QCoreApplication >(argc, argv, name), mServiceStater(new ServiceStarter(this))
{
    setObjectName("service");

    setServiceDescription(tr("Net-SNMP subagent service."));
    setStartupType(QtServiceController::AutoStartup);
}

MainService::~MainService()
{
    qLogInfo(objectName()) << tr("Destroyed");
}

void MainService::createApplication(int &argc, char **argv)
{
    QtService < QCoreApplication >::createApplication(argc, argv);

    InitMain::setupSearchPath();

    QDir qmDir("translations:");
    QStringList qmFiles = qmDir.entryList(QStringList("*.qm"), QDir::Files | QDir::Readable);
    foreach (const QString &file, qmFiles) {
        QTranslator *translator = new QTranslator(this);
        translator->load(file, qmDir.absolutePath());
        QCoreApplication::installTranslator(translator);
    }

    if (!loadConfig())
        throw QString();

    SubAgent *sa = new SubAgent(this);
    InternalServer *is = new InternalServer(this);

    mServiceStater->installService(sa);
    mServiceStater->installService(is);

    connect(is, SIGNAL(handshakeCame(QString)), sa, SIGNAL(startListening(QString)));
    connect(is, SIGNAL(clientDisconnected(QString)), sa, SIGNAL(stopListening(QString)));
    connect(sa, SIGNAL(getValue(QString,QString)), is, SLOT(getValue(QString,QString)));
    connect(is, SIGNAL(valueCame(QString,QString,QVariant)), sa, SIGNAL(valueCame(QString,QString,QVariant)));
    connect(sa, SIGNAL(setValue(QString,QString,QVariant)), is, SLOT(setValue(QString,QString,QVariant)));
    connect(is, SIGNAL(notifyCame(QString,QString,snmp_interaction::Vars))
            , sa, SIGNAL(notify(QString,QString,snmp_interaction::Vars)));
}

void MainService::start()
{
    initLog4Qt();

    qLogInfo(objectName()) << tr("=========> Service started <=========");
    qLogInfo(objectName()) << tr("%1, Version %2").arg(qApp->applicationName()
                                                        , qApp->applicationVersion());
    qLogInfo(objectName()) << tr("Build date: %1\n").arg(__DATE__);

    mServiceStater->start();
}

void MainService::stop()
{
    mServiceStater->stop();

    qLogInfo(objectName()) << tr("Service stopped");
    using namespace Log4Qt;
    LogManager::rootLogger()->removeAllAppenders();
}

bool MainService::loadConfig()
{
    QStringList argList = qApp->arguments();
    argList.removeAt(0);

    foreach (const QString &arg, argList) {
        QFileInfo fi(arg);
        if (fi.exists()) {
            Config::init(fi.absoluteFilePath());
            return true;
        }
    }

    QString configPath = InitMain::findConfigFilePath(QDir("bin:").absolutePath(),
                                       QFileInfo(qApp->applicationFilePath()).baseName());
    if (configPath.isEmpty())
        configPath = InitMain::findConfigFilePath();

    if (configPath.isEmpty()) {
        qDebug() << tr("Configuration file was not found.");
        qDebug() << tr("Creating default config.");
        createDefaultConfig();
        return false;
    }

    Config::init(configPath);
    return true;
}

void MainService::createDefaultConfig()
{
    const QString configFileName = QFileInfo(qApp->applicationFilePath()).baseName() + ".xml";
    QString configPath = QDir("bin:").absoluteFilePath(configFileName);
    if (QDir("etc:").exists())
        configPath = QDir("etc:").absoluteFilePath(configFileName);

    Config::init(configPath);
}

void MainService::initLog4Qt()
{
    QString patternString = "[%d{yyyy.MM.dd hh:mm:ss}] %p %c - %m%n";
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    PatternLayout *patternLayout = new PatternLayout(patternString, this);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, config->logPath()
                                                                    , true, this);
    rootFileAppender->setName("rootFileAppender");

    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);

    ColorConsoleLayout *colorConsoleLayout = new ColorConsoleLayout(patternString, this);
    colorConsoleLayout->setName("colorConsoleLayout");
    colorConsoleLayout->activateOptions();

    ColorConsoleAppender *colorConsoleAppender = new ColorConsoleAppender(
                colorConsoleLayout, ColorConsoleAppender::STDERR_TARGET, this);
    colorConsoleAppender->setName("colorConsoleAppender");
    colorConsoleAppender->activateOptions();
    LogManager::rootLogger()->addAppender(colorConsoleAppender);

    Log4Qt::Level logLevel;
    int iLevel = config->logLevel();

    switch (iLevel) {
        case 0: logLevel = Log4Qt::Level::OFF_INT; break;
        case 1: logLevel = Log4Qt::Level::FATAL_INT; break;
        case 2: logLevel = Log4Qt::Level::ERROR_INT; break;
        case 3: logLevel = Log4Qt::Level::WARN_INT; break;
        case 4: logLevel = Log4Qt::Level::INFO_INT; break;
        case 5: logLevel = Log4Qt::Level::DEBUG_INT; break;
        default: logLevel = Log4Qt::Level::ALL_INT; break;
    }

    LogManager::rootLogger()->setLevel(logLevel);
}
