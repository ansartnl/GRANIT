#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>

#include <QtGui/QColor>
#include <QtGui/QMainWindow>
#include "SpProto.h"

#include "AppSettings.h"

class QLabel;
class QModelIndex;

namespace Ui {
    class MainWindow;
}
class MonitoringSocket;

namespace UserSystem
{
class UserClient;
}
class DialogManager;

//! Main window GUI interface.
/*! Class creates:
    - menu
    - toolbar
    - statusbar

    Main window consist of 2 sections: host list and monitored object information.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setUserSystem();

    void requestRemoteOperations(const QDateTime &from, const QDateTime &to);
    QStringList requestRemoteLogs(const QDateTime &from, const QDateTime &to);

signals:
    void print();

    void RemoteOperationsRecieved(const SupervisorProto::RemoteOperations &op);
    void RemoteLogRecieved(const QString &sender, const SupervisorProto::RemoteLog &log);

    void serversResponse(const QStringList& servers);
    void groupsResponse(const QStringList& groups);
    void scriptsResponse(const QStringList& scripts);
protected:
    virtual void changeEvent(QEvent * event);
    void setLabelText();
    virtual void closeEvent(QCloseEvent * event);

private:
    void setupUi();
    void loadSettings();
    void createMonitoringSockets();
    void createStatusBar();
    bool confirm(const QString &title, const QString &text);
    bool isFdpSelected(bool andPassive = true);
    //! Show simplified (with hidden controls) main window.
    void showSimplifiedView(bool on);
    //! Update window title (show user name in the tittle).
    void updateWindowTittle();
    //! Update state of 'start group action'.
    void updateStartGroupUi();

    void runGroupScript(const QString& server, const QString& script);
    void runGroup(const QString& server, const QString &group);

private slots:
    void onConfigTriggered(QAction* action);
    void on_actionAbout_triggered();
    void on_actionLog_triggered();
    void on_actionRefresh_triggered();
    void on_actionStartGroup_triggered();
    void on_actionProcessStatus_triggered();
    void on_actionHostStatus_triggered();
    void onActionServerLogTriggered();
    void onActionRestartTriggered();
    void onActionSelfLogTriggered();
    void onActionStartAllTriggered();
    void onActionStopAllTriggered();
    void onActionShutdownTriggered();
    void onActionRebootTriggered();
    void onActionActivateTriggered();
    void onActionRemoteOperationsTriggered();
    void onActionGroupConfigTriggered();
    void onActionLogCollectorTriggered();
    void onStartScript();
    void onStopScript();
    void onPrintScript();
    void on_actionFont_triggered();

    void on_pushButtonStartGroup_clicked();

    void onServiceContextMenu(const QPoint&);
    void onHostContextMenu(const QPoint&);
    void onGroupContextMenu(const QPoint&);

    void onVariantReceived(QVariant);
    void onSocketStateChanged();
    void onNetStatusChanged();
    void onScriptOutputChanged();
    void fillScriptList();

    void onHostsViewCurrentChanged(const QModelIndex &current, const QModelIndex &);
    void onServicesViewCurrentChanged(const QModelIndex &current, const QModelIndex &);
    void onGroupsViewCurrentChanged(const QModelIndex &current, const QModelIndex &);

    void onLogined();
    void onLogouted();

    void onClockTimer();

    void writeLog(const QString &host, const QString &service
                  , const QString &message, QColor color = Qt::black);

    void resetModify();

    void onServersRequested();
    void onGroupsRequested(QString host);
    void onScriptsRequested(QString host);

    void loadGroupCommandList();
private:
    typedef QList<MonitoringSocket*>    TMonitoringSocketList;

    Ui::MainWindow *ui;
    QAction *actionScriptWidget;
    DialogManager *dialogManager;

    /** List of monitored hosts */
    TMonitoringSocketList   mMonHostList;

    MonitoringSocket *mSelectedSocket;

    int mLogSize;

    /** Toolbar labels */
    QLabel *serverLabel;
    QLabel *serviceLabel;

    /** Status bar */
    QLabel* m_pClockLabel;
    QTimer* m_pClockTimer;

    AppSettings mAppSettings;
    int mResetModifyShot;
};

#endif // MAINWINDOW_H
