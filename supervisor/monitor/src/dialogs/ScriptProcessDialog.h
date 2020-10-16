#ifndef SCRIPTPROCESSDIALOG_H
#define SCRIPTPROCESSDIALOG_H

#include <QDialog>
#include <QAction>

#include "SpProto.h"

class QModelIndex;
class MonitoringSocket;

namespace Ui {
class ScriptProcessDialog;
}

class ScriptProcessDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ScriptProcessDialog(QWidget *parent = 0);
    ~ScriptProcessDialog();

    void setNetworks(const QStringList& networks);
    void setGroups(const SupervisorProto::Groups& groups);
    void setServices(const SupervisorProto::Services& services);
    void setScripts(const SupervisorProto::ScriptList& scripts);
    void setScripts(const QStringList& scripts);

    void setDataConnection(MonitoringSocket *socket);

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:
//    void onGroupLabelClicked();
    void onStartScript();
    void onStopScript();
    void onServiceContextMenu(const QPoint& pos);
    void onServicesViewCurrentChanged(const QModelIndex &current, const QModelIndex &);

    void onRefresh();
    void onServiceLog();
    void onServiceRestart();
    void onServiceStatusToggle();

    void onVariantRecieved(QVariant v);
    void updateActiveGroupName();
    void onScriptOutputChanged();
    void onGroupActionTriggered();
    void on_pushButton_clicked();

private:
    void setupActions();
    bool confirm(const QString &title, const QString &text);

private:
    Ui::ScriptProcessDialog *ui;
    MonitoringSocket* mCurrentSocket;
    QAction* mActionRefresh;
    QAction* mActionServiceLog;
    QAction* mActionServiceRestart;
    QAction* mActionServiceStatusToggle;
};

#endif // SCRIPTPROCESSDIALOG_H
