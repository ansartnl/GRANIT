#include "init.h"

// UserSystem headers
#include "usTransport.h"
#include "UserClient.h"
#include "UserServer.h"

// log4qt headers
#include "log4qt.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#include "CmdLineArguments.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>

void initLog4Qt(QObject *parent)
{
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("[%d{locale:short}] %p %c %x - %m%n", parent);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    QString sLogFile = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    if(sLogFile.isEmpty())
        sLogFile = QDir("bin:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, sLogFile, true, parent);
    rootFileAppender->setName("rootFileAppender");
    rootFileAppender->setMaximumFileSize(10 * 1024 * 1024); // 10MB
    rootFileAppender->setMaxBackupIndex(10);
    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);
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
