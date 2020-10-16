#ifndef FOLDERCONTROLLER_H
#define FOLDERCONTROLLER_H

#include <QObject>

class FolderController : public QObject
{
    Q_OBJECT
public:
    typedef QPair<QString, QByteArray> FileItem;

    explicit FolderController(QObject *parent = 0);

    void setMonitoredFolder(const QString& folder);
    const QString& monitoredFolder() const;

    void setCheckInterval(uint interval);
    uint checkInterval() const;

    void startMonitoring();
    void stopMonitoring();
    
protected slots:
    void timerEvent(QTimerEvent *);

signals:
    void folderItems(const QList<FolderController::FileItem> &items);

private:
    int mTimerId;
    int mCheckInterval;
    QString mMonitoredFolder;
};

#endif // FOLDERCONTROLLER_H
