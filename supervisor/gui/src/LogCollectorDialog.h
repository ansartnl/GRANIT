#ifndef LOGCOLLECTORDIALOG_H
#define LOGCOLLECTORDIALOG_H

#include <QDialog>
#include <QDir>
#include <QMutex>

#include "SpProto.h"

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

public slots:
    void onRemoteLogRecieved(const QString &sender, const SupervisorProto::RemoteLog &log);

private slots:
    void onCancel();
    void onRequest();
    void onArchiveToggle(bool toggled);

    void onFinishedReceiving();
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
};

#endif // LOGCOLLECTORDIALOG_H
