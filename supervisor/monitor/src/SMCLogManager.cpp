#include "SMCLogManager.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include "main.h"

#include "CmdLineArguments.h"
#include "log4qt.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"

SMCLogManager::SMCLogManager(QObject *parent) :
    QObject(parent)
{
    QTimer::singleShot( (86400 - qAbs(QTime::currentTime().secsTo(QTime(0,0,1)))) * 1000, this, SLOT(updateLogs()));
}

SMCLogManager::~SMCLogManager()
{
    if(mAsterixLog.isOpen())
        mAsterixLog.close();
}

void SMCLogManager::setUseAsterixLog(bool use)
{
    mUseAsterixLog = use;
    if(mUseAsterixLog)
    {
        initAsterixLogger();
    }
    else
    {
        if(mAsterixLog.isOpen())
            mAsterixLog.close();
    }
}

void SMCLogManager::initLog4Qt(QObject *parent)
{
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    setupLogger(parent);

}

void SMCLogManager::initAsterixLogger()
{
    mUseAsterixLog = true;

    if(mAsterixLog.isOpen())
        mAsterixLog.close();

    mAsterixLog.setFileName(getAsterixLogFileName());
    if(mAsterixLog.open(QIODevice::WriteOnly | QIODevice::Append))
        qDebug() << "Failed to open Asterix Log file";
}

QString SMCLogManager::getLogFileName() const
{
    QString logPath = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    QString logName = globalSettings()->value("LogFileName",
                                            QFileInfo(qApp->applicationFilePath()).baseName()).toString();

    if(!logPath.isEmpty() && !logPath.endsWith('/'))
        logPath.append('/');

    logPath += logName + QDate::currentDate().toString("_yyyyMMdd") + ".log";

    return logPath;
}

QString SMCLogManager::getAsterixLogFileName() const
{
    QString logPath = CmdLineArguments::instance(QCoreApplication::arguments()).log();
    QString logName = globalSettings()->value("LogFileName",
                                            QFileInfo(qApp->applicationFilePath()).baseName()).toString();

    if(!logPath.isEmpty() && !logPath.endsWith('/'))
        logPath.append('/');

    logPath += logName + QDate::currentDate().toString("_asterix_yyyyMMdd") + ".log";

    return logPath;
}

void SMCLogManager::setupLogger(QObject *parent)
{
    using namespace Log4Qt;
    // Format log message "date time [thread] logger level - message \newline"
    PatternLayout *patternLayout = new PatternLayout("[%d{locale:short}] %p %c %x - %m%n", parent);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    QString logPath = getLogFileName();

    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, logPath, true, parent);
    rootFileAppender->setName("rootFileAppender");
    uint fileSize = globalSettings()->value("LogFileSize", 50).toUInt();
    rootFileAppender->setMaximumFileSize(fileSize * 1024 * 1024); // 50MB
    rootFileAppender->setMaxBackupIndex(0);
    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);
}

void SMCLogManager::updateLogs()
{

    QTimer::singleShot( (86400 - qAbs(QTime::currentTime().secsTo(QTime(0,0,1)))) * 1000, this, SLOT(updateLogs()));
    Log4Qt::LogManager::rootLogger()->removeAllAppenders();

    setupLogger(qApp);
    if(mUseAsterixLog)
        initAsterixLogger();
}

void SMCLogManager::asterixLog(const QString &sender, const QByteArray &data)
{
    if(mAsterixLog.isOpen() && mUseAsterixLog)
    {
        QString line = QString("[%1] %2 - ")
                            .arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"))
                            .arg(sender);
        for(int i = 0; i < data.size(); ++i)
        {
            line += QString("|%1 ")
                            .arg(QString::number((quint8)data.at(i), 16));
        }
        line += '\n';
        mAsterixLog.write(line.toUtf8());
        mAsterixLog.flush();
    }
}
