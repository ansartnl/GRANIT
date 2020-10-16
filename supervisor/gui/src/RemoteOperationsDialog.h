#ifndef REMOTEOPERATIONSDIALOG_H
#define REMOTEOPERATIONSDIALOG_H

#include <QDialog>
#include <QMutex>
#include <QSortFilterProxyModel>
#include "OperationsModel.h"
#include "SpProto.h"
#include "OperationsSortFilterModel.h"

namespace Ui {
class CRemoteOperationsDialog;
}

class CRemoteOperationsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRemoteOperationsDialog(QWidget *parent = 0);
    ~CRemoteOperationsDialog();

protected:
    virtual void closeEvent(QCloseEvent *e);

public slots:
    void onRemoteOperationsRecieved(const SupervisorProto::RemoteOperations &op);

private slots:
    void onClose();
    void onFilter();
    void onPrint();
    void onCurrentUserChanged(QString user);

signals:
    void print();

private:
    COperationsModel::TOperationsList parse(const QStringList &data);

    Ui::CRemoteOperationsDialog *ui;

    COperationsSortFilterModel *proxyModel;
    QMutex mMutex;
    QDateTime mCurFrom;
    QDateTime mCurTo;
};

#endif // REMOTEOPERATIONSDIALOG_H
