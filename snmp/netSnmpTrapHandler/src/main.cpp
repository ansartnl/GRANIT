#include "TrapInteraction.h"

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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QStringList>

#include <QtNetwork/QUdpSocket>

void initLog();

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    InitMain::setupCodec();
    InitMain::setupSearchPath();

    initLog();

    QStringList arguments = QCoreApplication::arguments();
    if (arguments[0].contains(QFileInfo(qApp->applicationFilePath()).baseName())) {
        // removing first "program path" argument, if exists
        arguments.removeFirst();
    }

    if (arguments.count() < 4) {
        qLogError("main") << QObject::tr("Arguments: '%1'. Need: 'trap host \"ip\" list_of_vars'")
                             .arg(arguments.join(", "));
        return 1;
    }

    qLogInfo("main") << arguments.join(" ");

    //UDP: [127.0.0.1]:41374->[127.0.0.1]
    QRegExp ipRexp("(\\w+):\\s"                     // protocol
                   "\\[([\\d\\.]+)\\]:\\d{1,5}\\->" // from
                   "\\[([\\d\\.]+)\\](?::\\d{1,5})?" // to
                   );
    if (!ipRexp.exactMatch(arguments[2])) {
        qLogError("main") << QObject::tr("'%1' does not match expected ip pattern").arg(arguments[2]);
        return 2;
    }
    trap_interaction::Trap trap(ipRexp.cap(2), arguments[1], arguments[0]);
    for (int i = 3; i < arguments.count(); ++i) {
        QString argument = arguments[i];
        trap.variables.insert(argument.section('=', 0, 0, QString::SectionSkipEmpty)
                              , argument.section('=', 1, 1, QString::SectionSkipEmpty));
    }

    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << trap;

    QUdpSocket socket;
    socket.writeDatagram(ba, QHostAddress::LocalHost, TRAP_UDP_PORT);

    return 0;
}

void initLog()
{
    QDir binDir("bin:");

    QString logPath;
    if (QDir("log:").exists())
        logPath = QDir("log:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";
    else
        logPath = binDir.absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log";

    QString patternString = "[%d{yyyy.MM.dd hh:mm:ss}] %p %c - %m%n";
    using namespace Log4Qt;
    LogManager::setHandleQtMessages(true);

    PatternLayout *patternLayout = new PatternLayout(patternString);
    patternLayout->setName("patternLayout");
    patternLayout->activateOptions();

    RollingFileAppender *rootFileAppender = new RollingFileAppender(patternLayout, logPath, true);
    rootFileAppender->setName("rootFileAppender");

    rootFileAppender->activateOptions();
    LogManager::rootLogger()->addAppender(rootFileAppender);

    ColorConsoleLayout *colorConsoleLayout = new ColorConsoleLayout(patternString);
    colorConsoleLayout->setName("colorConsoleLayout");
    colorConsoleLayout->activateOptions();

    ColorConsoleAppender *colorConsoleAppender = new ColorConsoleAppender(
                colorConsoleLayout, ColorConsoleAppender::STDERR_TARGET);
    colorConsoleAppender->setName("colorConsoleAppender");
    colorConsoleAppender->activateOptions();
    LogManager::rootLogger()->addAppender(colorConsoleAppender);

    LogManager::rootLogger()->setLevel(Log4Qt::Level::ALL_INT);
}
