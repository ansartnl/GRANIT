#include "LogController.h"

#include <QtDebug>

#include <QtCore/QFile>
#include <QtCore/QStringList>

LogController::LogController(QObject *parent) :
    QObject(parent)
{
}

void LogController::addLog(const QString &name, const QString &path)
{
    mObjects.insert(name, path);
}

QStringList LogController::names() const
{
    return mObjects.keys();
}

QStringList LogController::readLog(const QString &name, int countLines) const
{
    QString logPath = mObjects[name];
    if ( !QFile::exists(logPath) )
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

QStringList LogController::readStream(const QString &filePath, int countLines) const
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

QStringList LogController::readAllStream(const QString &filePath) const
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

QStringList LogController::packLines(QStringList &lines) const
{
    QStringList log;
    while ( !lines.empty() )
        log << lines.takeLast();
    return log;
}
