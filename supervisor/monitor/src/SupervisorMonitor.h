#ifndef SUPERVISORMONITOR_H
#define SUPERVISORMONITOR_H

#include <QDialog>
#include <QDomNode>
#include <QPropertyAnimation>

#include "qtxmlsettings.h"
#include "connections/IConnection.h"
#include "connections/supervisorServer/MonitoringSocket.h"
#include "connections/asterix253/AsterixListener253.h"
#include "datasources/IDatasource.h"

#include "dialogs/EventLogDialog.h"
#include "src/AlarmPlayer.h"
#include "SQLiteManager.h"

#include "SupervisorWidget.h"

namespace Ui {
class SupervisorMonitor;
}

class SupervisorView;
class QMenu;
class DialogManager;

class SupervisorMonitor : public QWidget
{
    Q_OBJECT
    
public:
    explicit SupervisorMonitor(QWidget *parent = 0);
    ~SupervisorMonitor();

    void setConfigPath(const QString& path);

    IConnection* getConnectionByID(const QString& connectionID) const;

    void openFile(const QString& fileName);

    void setUserSystem();
    SQLiteManager* getSQLiteManager();
    AlarmPlayer* getAlarmPlayer();
    QPushButton* getQPushButton();

signals:
    void writeLog(const QString &host, const QString &service, const QString &message, QColor color);

protected slots:
    //! File Menu
    void onOpenFile();
    void onSave();
    void onSaveAs();
    void onRecentFileOpen();

    //! Service menu
    void onEventLog();

    //! Connection slots
    void onVariantReceived(QVariant v);
    void onSocketStateChanged();
    void onNetStatusChanged();
    void fillScriptList();
    void onScriptOutputChanged();

    void onResetSocketModify();
    void clearView();

    //! User management
    void onLogined();
    void onLogouted();

    //! logging events
    void onWriteLog(const QString &host, const QString &service, const QString &message, QColor color);

    void resetAlarm();

public slots:
    void readyRead();

private:
    void loadXMLFileData(const QString& fileName);
    void loadConnectionXMLData(const QDomNode& node);
    void loadSupervisorServerConnectionsXMLData(const QDomNode& node);
    void loadSupervisorMonitorConnectionsXMLData(const QDomNode& node);
    void loadAsterix253ConnectionsXMLData(const QDomNode& node);
    void loadDatasourceXMLData(const QDomNode& node);
    void loadScriptsXMLData(const QDomNode& node);
    void loadStylesXMLData(const QDomNode& node);
    void loadWidgetXMLData(const QDomNode& node);
    void loadMenuXMLData(const QDomNode& node);

    void saveXMLFileData(const QString& fileName);
    void saveConnectionsXMLData(QDomDocument document, QDomNode parentNode);    // unused for now
    void saveDatasourceXMLData(QDomDocument document, QDomNode parentNode);     // unused for now
    void saveWidgetXMLData(QDomDocument document, QDomNode parentNode);         // deprecated
    void saveWidgetXMLData(QDomNode parentNode);

    void updateRecentFileActions(QMenu* menu);
    void setCurrentFile(const QString &fileName);
    QMenu* createContextMenuFromXMLData(const QDomNode& node, const QString& targetConnection);

    void resetPermissions();
    void closeScriptProcessDialogs();
    void runScript(const QString &path);

protected:
    virtual void closeEvent(QCloseEvent * event);

private:
    typedef QHash<QString, QSharedPointer<IConnection> > TConnectionsHash;
    typedef QHash<QString, QSharedPointer<IDatasource> > TDatasourceHash;
    typedef QHash<QString, QDomNode> TMenuHash;
    typedef QHash<QString, SupervisorWidgetLabelStyles> TStylesHash;

    Ui::SupervisorMonitor *ui;
    SupervisorView* mSuperView;
    QtXmlSettings*   mSettings;
    QString mCurrentConfigFile;
    QDomDocument mConfigDocument;

    TDatasourceHash mDatasources;
    TMenuHash       mWidgetMenus;
    /** List of monitored hosts */
    TConnectionsHash   mConnections;

    TStylesHash mStyles;

    QPointer<EventLogDialog> mEventLogDialog;
    SQLiteManager mLogDB;
    DialogManager* mDialogManager;

    QAction* mActionOpen;
    QAction* mActionSave;
    QAction* mActionSaveAs;

    QAction* mActionLogin;
    QAction* mActionLogout;
    QAction* mActionChangePassword;
    QAction* mActionUsers;

    QAction* mActionEventLog;

    QPushButton* mButtonResetAlarm;
    QPropertyAnimation* anim2;
    QPalette myPalette;

    QMenu* mMenuTools;

    bool mEventLogControlEnabled;

    AlarmPlayer mPlayer;

    QString mLogoutScript;

    QUdpSocket *mSocket;
    QString mHostMonitor;
    quint16 mPortMonitor;
};

#endif // SUPERVISORMONITOR_H
