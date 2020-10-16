#include "init.h"

#include "qtxmlsettings.h"

// log4qt headers
#include "log4qt.h"
#include "logmanager.h"
#include "patternlayout.h"
#include "rollingfileappender.h"
#include "consoleappender.h"
#include "CmdLineArguments.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QTextCodec>


QSettings* xmlSettings(const QString &pathFile)
{
    static QtXmlSettings *gXmlStg = new QtXmlSettings(pathFile, qApp);
    return gXmlStg;
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
    if(sLogFile.isEmpty())
        sLogFile = QDir("bin:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, sLogFile, true, parent);
    rootFileAppender->setName("rootFileAppender");
    rootFileAppender->setMaximumFileSize(10 * 1024 * 1024); // 10MB
    rootFileAppender->setMaxBackupIndex(10);
    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);

    ConsoleAppender *consoleAppender = new ConsoleAppender(patternLayout, ConsoleAppender::STDERR_TARGET, parent);
    consoleAppender->setName("consoleAppender");
#ifdef Q_OS_WIN
    consoleAppender->setEncoding(QTextCodec::codecForName("cp866"));
#else
    consoleAppender->setEncoding(QTextCodec::codecForName("utf8"));
#endif
    consoleAppender->activateOptions();
    LogManager::rootLogger()->addAppender(consoleAppender);
}

QMap<QString, QString> toMap(const QString &str)
{
    QMap<QString, QString> vals;

    QStringList strValues = str.split(";", QString::SkipEmptyParts);
    foreach ( const QString &kv, strValues )
    {
        QStringList keyValue = kv.split("=", QString::SkipEmptyParts);
        if ( keyValue.size() == 2 )
            vals.insert(keyValue[0], keyValue[1]);
    }

    return vals;
}

QString toString(const QMap<QString, QString> &vals)
{
    QStringList strValues;

    for ( QMap<QString, QString>::const_iterator itc = vals.constBegin(); itc != vals.constEnd(); ++itc )
    {
        strValues << QString("%1=%2").arg(itc.key()).arg(*itc);
    }

    return strValues.join(";");
}
