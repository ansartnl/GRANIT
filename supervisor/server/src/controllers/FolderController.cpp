#include "FolderController.h"

#include <QDir>

FolderController::FolderController(QObject *parent) :
    QObject(parent), mTimerId(0), mCheckInterval(8000)
{
}

void FolderController::setMonitoredFolder(const QString &folder)
{
    mMonitoredFolder = folder;
}

const QString &FolderController::monitoredFolder() const
{
    return mMonitoredFolder;
}

void FolderController::setCheckInterval(uint interval)
{
    mCheckInterval = interval;
}

uint FolderController::checkInterval() const
{
    return mCheckInterval;
}

void FolderController::startMonitoring()
{
    if (mTimerId)
        stopMonitoring();
    mTimerId = startTimer(mCheckInterval);
}

void FolderController::stopMonitoring()
{
    killTimer(mTimerId);
    mTimerId = 0;
}

void FolderController::timerEvent(QTimerEvent *)
{
    QList<FileItem> items;

    QDir directory = QDir(mMonitoredFolder);
    if(directory.exists())
    {
        const QStringList files = directory.entryList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(const QString &name : files)
        {
            auto item = qMakePair(name, QByteArray());

            QFile file(directory.absoluteFilePath(name));
            if (file.open(QIODevice::ReadOnly))
            {
                item.second = file.readAll();
                file.close();
            }

            items.append(item);
        }
    }

    emit folderItems(items);
}
