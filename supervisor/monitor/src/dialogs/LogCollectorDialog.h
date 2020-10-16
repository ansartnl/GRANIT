#ifndef LOGCOLLECTORDIALOG_H
#define LOGCOLLECTORDIALOG_H

#include <QDialog>
#include <QDir>
#include <QMutex>

#include "SpProto.h"

class MonitoringSocket;

namespace Ui {
class CLogCollectorDialog;
}

class CLogCollectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLogCollectorDialog(QWidget *parent = 0);
    ~CLogCollectorDialog();

    void setFlashPath(QString path)
    {mFlashPath = path;}

    void setSocket(MonitoringSocket* socket);

public slots:
    void onRemoteLogRecieved(const QString &sender, const SupervisorProto::RemoteLog &log);

private slots:
    void onCancel();
    void onRequest();
    void onArchiveToggle(bool toggled);

    void onSaveToFlashDialogFinished(int result);

private:
    void enableControls(bool enable);

private:
    Ui::CLogCollectorDialog *ui;

    bool mUseArchive;
    QStringList mRequestedServices;
    QStringList mLogDetails;
    QDir    mLogDir;
    QMutex  mMutex;
    QString     mFlashPath;
    MonitoringSocket* mSocket;
};

#endif // LOGCOLLECTORDIALOG_H
