#include "Controller.h"

#include "Log.h"
#include "DbConnThread.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QDate>

QStringList readStream(const QString &filePath, int countLines)
{
    Q_ASSERT(countLines);

    QStringList lines;

    QFile file(filePath);
    if ( !file.open( QIODevice::ReadOnly ) )
        return lines;

    QTextStream stream(&file);
    while ( !stream.atEnd() )
    {
        lines << stream.readLine();

        // Limit number of read lines.
        if ( countLines < lines.size() )
            lines.removeFirst();
    }

    return lines;
}

QStringList readAllStream(const QString &filePath)
{
    QStringList lines;

    QFile file(filePath);
    if ( !file.open( QIODevice::ReadOnly ) )
        return lines;

    QTextStream stream(&file);
    while ( !stream.atEnd() )
    {
        lines << stream.readLine();
    }

    return lines;
}

QStringList packLines(QStringList &lines)
{
    QStringList log;
    while ( !lines.empty() )
        log << lines.takeLast();
    return log;
}


Controller::Controller(QObject *parent)
    : QObject(parent), mIsRestartable(false), mIsStopped(true), mDbConnection(0)
{
}

const QString Controller::nextLog(QString prevName) const
{
    if(prevName.isNull())
    {
        // Check that path to log exists.
        if ( QFile::exists(mLogPath) )
            return mLogPath;
    }
    else
    {
        int n = prevName.lastIndexOf(QString("."));
        bool bOk;
        int num = prevName.mid(n+1).toInt(&bOk) + 1;
        if(!bOk)
            num = 1;

        QString logBackupPath = QString("%1.%2").arg(mLogPath).arg(num);
        if ( QFile::exists(logBackupPath) )
            return logBackupPath;
    }
    return QString::null;
}

const QDateTime Controller::lineTimeFromFile(QString &log) const
{
    QDateTime time(QDate(0,0,0));

    QFile file(log);
    file.open(QIODevice::ReadOnly);

    while(!file.atEnd())
    {
        QString str = file.readLine();
        int n = str.indexOf(" [");
        if(n != -1)
        {
            time = QDateTime::fromString(str.left(n), "dd.MM.yy hh:mm");
            if(time.isValid())
            {
                if(time.date().year() < 2000)    //19xx to 20xx correction
                    time = time.addYears(100);
                break;
            }
        }
    }
    file.close();
    return time;
}

QStringList Controller::readLog(int countLines) const
{
    QString logPath = logFilePath();
    // Check that path to log exists.
    if ( QFile::exists(mLogPath) && QFileInfo(mLogPath).isFile() )
        logPath = mLogPath;
    else if ( !QFile::exists(logPath) )
        return QStringList();

    qDebug() << "Logfile: " << logPath;

    QStringList lines;
    int currentCountLines = countLines;
    int backupIndex = 1;
    QString logBackupPath = logPath;
    while ( lines.size() < countLines || countLines == -1)
    {
        QStringList currentLines;
        if(countLines == -1)
            currentLines = readAllStream(logBackupPath);
        else
            currentLines = readStream(logBackupPath, currentCountLines);
        currentCountLines -= currentLines.size();
        qDebug() << "Lines read: " << currentLines.size();

        // Store older lines before.
        lines = currentLines + lines;

        if ( currentCountLines < 1 && countLines != -1)
            break;

        // Try to read log from the next log if not all lines are read.
        logBackupPath = QString("%1.%2").arg(logPath).arg(backupIndex++);
        if ( !QFile::exists(logBackupPath) )
            break;
    }

    return packLines(lines);
}

const QStringList Controller::readLogByTime(const QDateTime &from, const QDateTime &to, bool extractLines) const
{
    QString logPath = QString::null;
    QStringList lines;
    QStringList logsToParse;

    QDateTime first;
    QDateTime last = QDateTime::currentDateTime();
    do
    {
        logPath = nextLog(logPath);
        if(!logPath.isNull())
        {
            first = lineTimeFromFile(logPath);
            qDebug() << "Checking line: " << first;
        }
        if( !(first > to || last < from) && !logPath.isEmpty())  //check if log may contain specified period
            logsToParse.prepend(logPath);
        last = first;

    } while(last > from && !logPath.isNull());

    foreach(const QString &log, logsToParse)
    {
        QFile file(log);
        file.open(QIODevice::ReadOnly);

        QDateTime time;
        while(!file.atEnd())
        {
            const QString str = file.readLine();

            if(extractLines)
            {// checking line time
                int n = str.indexOf(" [");
                if(n != -1)
                {
                    time = QDateTime::fromString(str.left(n), "dd.MM.yy hh:mm");
                    if(time.isValid())
                    {
                        if(time.date().year() < 2000)    //19xx to 20xx correction
                            time = time.addYears(100);
                        if(time >= from && time <= to)
                            lines.append(str);
                    }
                }
            }
            else
            {// storing "as is"
                lines.append(str);
            }
        }
        file.close();
    }
    return packLines(lines);
}

bool Controller::stop()
{
    if ( mDbConnection )
    {
        // Wait until thread finishes.
        mDbConnection->stop();
        mDbConnection->wait();
    }

    mIsStopped = true;
    return true;
}

QVariant Controller::databaseCheckerUserData() const
{
    return mDbConnection ? mDbConnection->userData() : QVariant();
}

void Controller::setDatabaseChecker(DbConnThread *dbConnChecker)
{
    if ( dbConnChecker )
        mDbConnection = dbConnChecker->clone(this);
}
