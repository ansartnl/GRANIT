#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "LogDialog.h"
#include "main.h"
#include "MonitoringSocket.h"
#include "RemoteOperationsDialog.h"
#include "LogCollectorDialog.h"
#include "FontColorsDialog.h"
#include "GroupConfigDialog.h"

#include "HostsModel.h"
#include "MonitoringModel.h"
#include "GroupsModel.h"

#include "SpProto.h"

#include "Log.h"
#include "printactionsmanager.h"

#include "DialogManager.h"
#include "UserClient.h"
#include "UserServer.h"

#include "sound.h"

#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QTextCodec>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QScrollBar>
#include <QtGui/QStyledItemDelegate>

// Application name
static const char* gWindowTitle = QT_TRANSLATE_NOOP("MainWindow", "Supervisor Monitor");
// Settings keys
static const QString gKeyVersion = "Version";
static const QString gKeySplitterState = "SplitterState";
static const QString gKeySplitter2State = "Splitter2State";
static const QString gKeySplitter3State = "Splitter3State";
static const QString gKeyServicesHeaderState = "ServicesHeaderState";
static const QString gKeyHostsHeaderState = "HostsHeaderState";
static const QString gKeyLogHeaderState = "LogHeaderState";
static const QString gKeyGroupsHeaderState = "GroupsHeaderState";
static const QString gKeyWindowState = "WindowState";
static const QString gKeyWindowGeometry = "WindowGeometry";
static const QString gKeyWindowFullscreen = "Fullscreen";
static const QString gKeyBlockAltTab = "BlockAltTab";
// needed for switching between fullscreen and windowed modes
static const QString gKeyFSA = "FSA";
static const QString gKeyWDA = "WDA";
// log columns
static const int gTime = 0;
static const int gHost = 1;
static const int gService = 2;
static const int gMessage = 3;
// Simplified view state
static bool gIsViewSimplified = false;

// Returns true if user does not belong to management group (guest).
static bool isUserGuest()
{
    UserSystem::User user = UserSystem::userClient()->user();
    if ( user.isInGroup( UserSystem::GroupAdministrators::name() )
         || user.isInGroup( UserSystem::GroupOperators::name() )
         || user.isInGroup( UserSystem::GroupAdvancedOperators::name() )
         || user.isInGroup( UserSystem::GroupRoot::name() ) )
        return false;
    return true;
}


// Delegate used to draw text without highlighting it.
class UnhighlightedDelegate : public QStyledItemDelegate
{
public:
    explicit UnhighlightedDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {}

    // Updating pallete to draw unhighlighted text.
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
                              ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        QColor colorHighlightedTextOld = opt.palette.color(cg, QPalette::HighlightedText);

        // Trick to draw unhighlighted text (modification pallete is required).
        QStyleOptionViewItemV4 *vopt = const_cast<QStyleOptionViewItemV4*>(
                    static_cast<const QStyleOptionViewItemV4*>(&option));
        if ( opt.state & QStyle::State_Selected )
        {
            QColor colorHigh = index.data(Qt::ForegroundRole).value<QColor>();
            vopt->palette.setColor(cg, QPalette::HighlightedText, colorHigh);
        }

        QStyledItemDelegate::paint(painter, option, index);

        // Restore old highlighted text color.
        vopt->palette.setColor(cg, QPalette::HighlightedText, colorHighlightedTextOld);
    }
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mResetModifyShot(2500)
{
    mSelectedSocket = 0;

    mLogSize = xmlSettings().value("event_list_size", 30).toUInt();

    ui->setupUi(this);
    setupUi();

    UnhighlightedDelegate *unhighlightedDelegate = new UnhighlightedDelegate(this);
    ui->groupsView->setItemDelegate(unhighlightedDelegate);
    ui->hostsView->setItemDelegate(unhighlightedDelegate);
    ui->servicesView->setItemDelegate(unhighlightedDelegate);

    ui->splitterServices->setChildrenCollapsible(false);

    // Init app settings
    {
        QSettings settings;
        QVariant var = settings.value("AppSettings");
        if (!var.isNull() && var.isValid())
        {
            mAppSettings.fromByteArray(var.toByteArray());
        }
    }

    connect(ui->actionServerLog, SIGNAL(triggered()), this, SLOT(onActionServerLogTriggered()));
    connect(ui->actionRestart, SIGNAL(triggered()), this, SLOT(onActionRestartTriggered()));
    connect(ui->actionSelfLog, SIGNAL(triggered()), this, SLOT(onActionSelfLogTriggered()));
    connect(ui->actionStartAll, SIGNAL(triggered()), this, SLOT(onActionStartAllTriggered()));
    connect(ui->actionStopAll, SIGNAL(triggered()), this, SLOT(onActionStopAllTriggered()));
    connect(ui->actionShutdown, SIGNAL(triggered()), this, SLOT(onActionShutdownTriggered()));
    connect(ui->actionReboot, SIGNAL(triggered()), this, SLOT(onActionRebootTriggered()));
    connect(ui->actionActivate, SIGNAL(triggered()), this, SLOT(onActionActivateTriggered()));
    connect(ui->actionRemoteOperations, SIGNAL(triggered()), this, SLOT(onActionRemoteOperationsTriggered()));
    connect(ui->actionRemoteLogs, SIGNAL(triggered()), this, SLOT(onActionLogCollectorTriggered()));
    connect(ui->actionGroupConfig, SIGNAL(triggered()), this, SLOT(onActionGroupConfigTriggered()));

    connect(ui->menuConfig, SIGNAL(triggered(QAction*)), this, SLOT(onConfigTriggered(QAction*)));

    createStatusBar();

    onLogined();

    PrintActionsManager *printActionsManager = new PrintActionsManager(this);
    connect(this, SIGNAL(print()), printActionsManager, SIGNAL(printPreview()));

    gIsViewSimplified = xmlSettings().value("simplified_view", false).toBool();
    showSimplifiedView( gIsViewSimplified );

    updateWindowTittle();

    QTimer::singleShot(mResetModifyShot, this, SLOT(resetModify()));
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("GroupData", UserSystem::userServer()->groupsToByteArray().toHex());
    settings.setValue("UserData", UserSystem::userServer()->usersToByteArray().toHex());
    delete ui;
}

void MainWindow::updateWindowTittle()
{
    setWindowTitle( tr("Supervisor Monitor") + QString(" <%1>").arg(UserSystem::userClient()->user().name()) );
}

void MainWindow::showSimplifiedView(bool on)
{
    if ( on )
    {
        ui->menuBar->hide();
        ui->mainToolBar->hide();
        ui->hostsView->hide();
        ui->scriptWidget->hide();
        ui->logWidget->hide();
        ui->netLabel->hide();
        ui->line_2->hide();
    }
}

void MainWindow::setUserSystem()
{
    dialogManager = new DialogManager(UserSystem::userClient(), this, this);
    dialogManager->setUsersIcon(":/images/Users");
    connect(ui->actionLogin, SIGNAL(triggered()), dialogManager, SLOT(login()));
    connect(ui->actionLogout, SIGNAL(triggered()), dialogManager, SLOT(logout()));
    connect(ui->actionChangePassword, SIGNAL(triggered()), dialogManager, SLOT(changePassword()));
    connect(ui->actionUsers, SIGNAL(triggered()), dialogManager, SLOT(manageUsers()));

    connect(dialogManager, SIGNAL(logined()), this, SLOT(onLogined()));
    connect(dialogManager, SIGNAL(logouted()), this, SLOT(onLogouted()));
}

void MainWindow::requestRemoteOperations(const QDateTime &from, const QDateTime &to)
{
    for(int i = 0; i < mMonHostList.size(); i++){

        MonitoringSocket *socket = mMonHostList[i];

        for(int j = 0; j < socket->countObjects(); j++){
            if ( socket->object(j).type == MonitoredObject::CWP)
            {
                const MonitoredObject& obj = socket->object(j);
                socket->requestRemoteOperations(obj.name, from, to);
                qLogInfo(tr("Action")) << tr(" Sending Remote Operations request to ") << socket->name();
            }
        }
    }
}

QStringList MainWindow::requestRemoteLogs(const QDateTime &from, const QDateTime &to)
{
    QStringList services;
    for(int i = 0; i < mMonHostList.size(); i++){

        MonitoringSocket *socket = mMonHostList[i];
        if (socket->isConnected()) {
            for(int j = 0; j < socket->countObjects(); j++)
            {
                socket->requestRemoteLogs(from, to);
                qLogInfo(tr("Action")) << tr(" Sending Remote Logs request to ") << socket->name();
                services.append(socket->object(j).name);
            }
        }
    }
    return services;
}

void MainWindow::setupUi()
{
    // Set network channel ...

    createMonitoringSockets();

    // Create models
    {
        ui->hostsView->setModel( new HostsModel(mMonHostList, &mAppSettings, ui->hostsView) );
        ui->servicesView->setModel( new MonitoringModel(ui->servicesView) );
        ui->groupsView->setModel( new GroupsModel(ui->groupsView) );
    }
    ui->servicesView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->hostsView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->groupsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->servicesView, SIGNAL(customContextMenuRequested(QPoint))
            , SLOT(onServiceContextMenu(QPoint)));
    connect(ui->hostsView, SIGNAL(customContextMenuRequested(QPoint))
            , SLOT(onHostContextMenu(QPoint)));
    connect(ui->servicesView->selectionModel()
            , SIGNAL(currentChanged(QModelIndex,QModelIndex))
            , SLOT(onServicesViewCurrentChanged(QModelIndex,QModelIndex)));
    connect(ui->hostsView->selectionModel()
            , SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(onHostsViewCurrentChanged(QModelIndex,QModelIndex)));
    connect(ui->groupsView, SIGNAL(customContextMenuRequested(QPoint))
            , SLOT(onGroupContextMenu(QPoint)));
    connect(ui->groupsView->selectionModel()
            , SIGNAL(currentChanged(QModelIndex,QModelIndex))
            , SLOT(onGroupsViewCurrentChanged(QModelIndex,QModelIndex)));

    // View actions.
    ui->menuView->addAction( ui->mainToolBar->toggleViewAction() );

    // Restore window state.
    ui->servicesView->header()->setMovable(false);

    // Select first item in the hosts view.
    QModelIndex indexSelHost;
    if ( !mMonHostList.empty() )
    {
        indexSelHost = ui->hostsView->model()->index(0,0);
        ui->hostsView->selectRow(0);
        onHostsViewCurrentChanged(indexSelHost, indexSelHost);
    }
    // Script Widget action
    ui->scriptWidget->setVisible(false);
    actionScriptWidget = ui->scriptWidget->toggleViewAction();
    actionScriptWidget->setIcon(QIcon(":/images/Script"));
    actionScriptWidget->setToolTip(tr("Shows all scripts, available on this server"));
    ui->menuServer->addSeparator();
    ui->menuServer->addAction(actionScriptWidget);
    ui->mainToolBar->insertAction(ui->actionLog, actionScriptWidget);
    ui->mainToolBar->insertSeparator(ui->actionLog);
    // toolbar labels
    serverLabel = new QLabel(this);
    ui->mainToolBar->insertWidget(ui->actionServerLog, serverLabel);
    serviceLabel = new QLabel(this);
    ui->mainToolBar->insertWidget(ui->actionLog, serviceLabel);
    setLabelText();
    // Script Widget buttons
    connect(ui->scriptStartButton, SIGNAL(clicked()), SLOT(onStartScript()));
    connect(ui->scriptStopButton, SIGNAL(clicked()), SLOT(onStopScript()));
    connect(ui->scriptPrintButton, SIGNAL(clicked()), SLOT(onPrintScript()));

    loadSettings();

    // Request server groups for selected (monitored) host.
    if ( indexSelHost.isValid() )
    {
        int hostIndex = indexSelHost.row();
        if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
        {
            MonitoringSocket *socket = mMonHostList[hostIndex];
            socket->requestGroups();
        }
    }

    loadGroupCommandList();
}

void MainWindow::updateStartGroupUi()
{
    QModelIndexList selIndexes = ui->groupsView->selectionModel()->selectedIndexes();

    bool isDisableNeeded = false;

    if ( !selIndexes.empty() )
    {
        QAbstractItemModel *model = ui->groupsView->model();
        QModelIndex indexSelGroup = selIndexes.at(0);
        isDisableNeeded = mSelectedSocket->isConnected() && model->index(indexSelGroup.row(),0).data(Qt::UserRole).toString() == mSelectedSocket->activeGroup();
        isDisableNeeded |= !UserSystem::userClient()->user().hasPermission(RestartPermission);
    }

    ui->pushButtonStartGroup->setEnabled(!isDisableNeeded);
    ui->actionStartGroup->setEnabled(!isDisableNeeded);
}

void MainWindow::runGroupScript(const QString &server, const QString &script)
{
    for(int i = 0; i < mMonHostList.size(); ++i)
    {
        if(server == mMonHostList.at(i)->name())
        {
            if(mMonHostList.at(i)->availableScripts().contains(script))
            {
                mMonHostList.at(i)->startScript(script, QString());
                break;
            }
        }
    }
}

void MainWindow::runGroup(const QString &server, const QString &group)
{
    for(int i = 0; i < mMonHostList.size(); ++i)
    {
        if(server == mMonHostList.at(i)->name())
        {
            for(int j = 0; j < mMonHostList.at(i)->countGroups(); ++j)
            {
                if(group == (mMonHostList.at(i)->group(j).visualName) &&
                   mMonHostList.at(i)->group(j).name != mMonHostList.at(i)->activeGroup())
                {
                    mMonHostList.at(i)->clearObjects();
                    mMonHostList.at(i)->activateGroup(mMonHostList.at(i)->group(j).name);
                    break;
                }
            }
        }
    }
}

void MainWindow::onConfigTriggered(QAction *action)
{
    QString configName = action->text();
    int res = QMessageBox::question(this,
                                    tr("Confirm"),
                                    QString(tr("Run command \"%1\"?")).arg(configName),
                                    QMessageBox::Ok | QMessageBox::Cancel,
                                    QMessageBox::Ok);

    if(res != QMessageBox::Ok)
        return;

    QHash<QString, Config> configs = GroupConfigDialog::getConfig(configName);
    QHash<QString, Config>::iterator itConfig = configs.begin();
    for(;itConfig != configs.end(); ++itConfig)
    {
        if((*itConfig).value.isEmpty())
            continue;

        if((*itConfig).isGroup)
            runGroup(itConfig.key(), (*itConfig).value);
        else
            runGroupScript(itConfig.key(), (*itConfig).value);
    }
}

void MainWindow::loadSettings()
{
    // removing unversioned settings
    QSettings settings;
    if (settings.value(gKeyVersion).toString() != qApp->applicationVersion())
    {
        settings.remove(gKeyLogHeaderState);
        settings.remove(gKeyServicesHeaderState);
        settings.remove(gKeyHostsHeaderState);
        settings.remove(gKeySplitterState);
        settings.remove(gKeySplitter2State);
        settings.remove(gKeySplitter3State);
        settings.remove(gKeyWindowGeometry);
        settings.remove(gKeyWindowState);
        settings.setValue(gKeyVersion, qApp->applicationVersion());
    }

    QVariant var = settings.value(gKeyBlockAltTab, 1);
    settings.setValue(gKeyBlockAltTab, var);
    if(var.toInt())
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    var = settings.value(gKeyWindowFullscreen, 1);
    settings.setValue(gKeyWindowFullscreen, var);
    if(var.toInt())
    {
        setWindowState(windowState() | Qt::WindowFullScreen);

        if (settings.value(gKeyFSA, false).toBool())
        {
            restoreGeometry( settings.value(gKeyWindowGeometry).toByteArray() );
        } else
        {
            settings.setValue(gKeyFSA, true);
            settings.setValue(gKeyWDA, false);
        }
    } else
    {
        if (settings.value(gKeyWDA, false).toBool())
        {
            restoreGeometry( settings.value(gKeyWindowGeometry).toByteArray() );
        } else
        {
            settings.setValue(gKeyWDA, true);
            settings.setValue(gKeyFSA, false);
        }
    }

    restoreState( settings.value(gKeyWindowState).toByteArray() );
    ui->splitter->restoreState( settings.value(gKeySplitterState).toByteArray() );
    ui->splitterServices->restoreState( settings.value(gKeySplitter2State).toByteArray() );
    ui->splitterLog->restoreState( settings.value(gKeySplitter3State).toByteArray() );
    ui->servicesView->header()->restoreState(
                settings.value(gKeyServicesHeaderState).toByteArray() );
    ui->hostsView->horizontalHeader()->restoreState(
                settings.value(gKeyHostsHeaderState).toByteArray() );
    ui->logWidget->horizontalHeader()->restoreState(
                settings.value(gKeyLogHeaderState).toByteArray() );
    ui->groupsView->header()->restoreState(
                settings.value(gKeyGroupsHeaderState).toByteArray());
}

void MainWindow::createMonitoringSockets()
{
    QSettings stgGlobal;
    stgGlobal.beginGroup("status/hosts");

    QSettings &settings = xmlSettings();
    settings.beginGroup("monitored_hosts");
    foreach ( const QString &monHost, settings.childGroups() )
    {
        settings.beginGroup(monHost);
        MonitoringSocket *socket
                = new MonitoringSocket(monHost,
                                       settings.value("name").toString(),
                                       settings.value("host").toString(),
                                       settings.value("host2", "").toString(),
                                       settings.value("port").toUInt(),
                                       this);
        socket->changeStatus( stgGlobal.value(socket->name(), true).toBool() );
        connect(socket, SIGNAL(newVariantMessage(QVariant))
                , SLOT(onVariantReceived(QVariant)), Qt::QueuedConnection);
        connect(socket, SIGNAL(eventToLog(QString,QString,QString,QColor))
                , SLOT(writeLog(QString,QString,QString,QColor)));
        connect(socket, SIGNAL(socketConnected()), SLOT(onSocketStateChanged()));
        connect(socket, SIGNAL(socketDisconnected()), SLOT(onSocketStateChanged()));
        connect(socket, SIGNAL(netStatusChanged()), SLOT(onNetStatusChanged()));
        connect(socket, SIGNAL(scriptListChanged()), SLOT(fillScriptList()));
        connect(socket, SIGNAL(scriptOutputChanged()), SLOT(onScriptOutputChanged()));
        mMonHostList << socket;
        settings.endGroup();
    }
    settings.endGroup();

    stgGlobal.endGroup();
}

bool MainWindow::confirm(const QString &title, const QString &text)
{
    return (QMessageBox::Yes == QMessageBox::question(this, title, text
                                                      , QMessageBox::Yes | QMessageBox::No));
}

bool MainWindow::isFdpSelected(bool andPassive)
{
    if (!mSelectedSocket)
        return false;
    MonitoredObject *obj = mSelectedSocket->fdp();
    if (!obj)
        return false;
    if (andPassive)
        return (obj->status == MonitoredObject::Started)
                && (obj->additionalStatus == QString("%1")
                    .arg(SupervisorProto::fdpStatus(false)));
    return true;

}

void MainWindow::onHostsViewCurrentChanged(const QModelIndex &current,
                                            const QModelIndex &)
{
    ui->servicesView->setCurrentIndex(QModelIndex());
    MonitoringModel *monitoringModel = static_cast<MonitoringModel *>( ui->servicesView->model() );
    GroupsModel *groupsModel = static_cast<GroupsModel *>( ui->groupsView->model() );
    MonitoringSocket *socket = static_cast<MonitoringSocket *>(current.internalPointer());
    mSelectedSocket = socket;
    socket->setCurrentObject(-1);
    socket->setModify(false);

    // Hide splitter widget if there are less than 2 groups.
    if ( socket->countGroups() > 1 )
        ui->splitterServices->widget(0)->show();
    else
        ui->splitterServices->widget(0)->hide();

    monitoringModel->setSocket( socket );
    monitoringModel->update();

    groupsModel->setSocket( socket );
    groupsModel->update();

    ui->labelInfo->setText(
                tr("Connection: '%1'  Host: '%2'  Host2: '%3'  Port: %4")
                .arg(socket->name().isEmpty() ? socket->id() : socket->name())
                .arg(socket->host())
                .arg(socket->host2().isEmpty() ? tr("No") : socket->host2())
                .arg(socket->port()));

    onNetStatusChanged();
    fillScriptList();
    onScriptOutputChanged();
    onSocketStateChanged();
    ui->actionLog->setEnabled(false);
    ui->actionRestart->setEnabled(false);
}

void MainWindow::onServicesViewCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        ui->actionLog->setEnabled(false);
        ui->actionRestart->setEnabled(false);
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        int serviceIndex = current.row();
        // Selected service
        if ( serviceIndex >= 0 && serviceIndex < socket->countObjects() )
        {
            socket->setCurrentObject(serviceIndex);
            ui->actionLog->setEnabled(UserSystem::userClient()->user().hasPermission(UserSystem::Read));
            ui->actionRestart->setEnabled(UserSystem::userClient()->user().hasPermission(RestartPermission));
        } else
            socket->setCurrentObject(-1);
    }
}

void MainWindow::onLogined()
{
    qLogInfo("User") << tr(" User '%1' logined").arg(UserSystem::userClient()->user().name());

    ui->actionLogout->setEnabled(true);
    if (UserSystem::userClient()->user().hasPermission(UserSystem::ChangePassword))
        ui->actionChangePassword->setEnabled(true);
    else
        ui->actionChangePassword->setEnabled(false);
    if (UserSystem::userClient()->user().hasPermission(UserSystem::ReadPermissions))
        ui->actionUsers->setEnabled(true);
    else
        ui->actionUsers->setEnabled(false);
    if (UserSystem::userClient()->user().hasPermission(UserSystem::List))
    {
        ui->actionRefresh->setEnabled(true);
        actionScriptWidget->setEnabled(true);
    } else
    {
        ui->actionRefresh->setEnabled(false);
        actionScriptWidget->setEnabled(false);
    }
    if (UserSystem::userClient()->user().hasPermission(UserSystem::Read))
    {
        ui->actionLog->setEnabled(mSelectedSocket->currentObject());
        ui->actionServerLog->setEnabled(mSelectedSocket->isConnected());
        ui->actionSelfLog->setEnabled(true);
    } else
    {
        ui->actionLog->setEnabled(false);
        ui->actionServerLog->setEnabled(false);
        ui->actionSelfLog->setEnabled(false);
    }
    if (UserSystem::userClient()->user().hasPermission(UserSystem::Execute))
        ui->actionActivate->setEnabled(isFdpSelected());
    else
        ui->actionActivate->setEnabled(false);
    if (UserSystem::userClient()->user().hasPermission(RestartPermission))
    {
        ui->actionRestart->setEnabled(mSelectedSocket->currentObject());
        ui->actionStartAll->setEnabled(mSelectedSocket->isConnected());
        ui->actionStopAll->setEnabled(mSelectedSocket->isConnected());
        updateStartGroupUi();
    } else
    {
        ui->actionRestart->setEnabled(false);
        ui->actionStartAll->setEnabled(false);
        ui->actionStopAll->setEnabled(false);

        ui->actionStartGroup->setEnabled(false);
        ui->pushButtonStartGroup->setEnabled(false);
    }
    if (UserSystem::userClient()->user().hasPermission(RunScriptPermission))
    {
        ui->scriptStartButton->setEnabled(true);
        ui->scriptStopButton->setEnabled(true);
        ui->scriptPrintButton->setEnabled(true);
    } else
    {
        ui->scriptStartButton->setEnabled(false);
        ui->scriptStopButton->setEnabled(false);
        ui->scriptPrintButton->setEnabled(false);
    }
    if (UserSystem::userClient()->user().hasPermission(RebootPermission))
    {
        ui->actionShutdown->setEnabled(mSelectedSocket->isConnected());
        ui->actionReboot->setEnabled(mSelectedSocket->isConnected());
    } else
    {
        ui->actionShutdown->setEnabled(false);
        ui->actionReboot->setEnabled(false);
    }
    { // temp
        ui->actionShutdown->setVisible(false);
        ui->actionReboot->setVisible(false);
    }
}

void MainWindow::onLogouted()
{
    qLogInfo("User") << tr(" User logouted");
    ui->actionLogout->setEnabled(false);
    ui->actionChangePassword->setEnabled(false);

    ui->actionUsers->setEnabled(false);

    ui->actionRefresh->setEnabled(false);

    ui->actionLog->setEnabled(false);
    ui->actionRestart->setEnabled(false);

    ui->actionServerLog->setEnabled(false);
    ui->actionStartAll->setEnabled(false);
    ui->actionStopAll->setEnabled(false);
    ui->actionShutdown->setEnabled(false);
    ui->actionReboot->setEnabled(false);

    ui->actionStartGroup->setEnabled(false);
    ui->pushButtonStartGroup->setEnabled(false);

    ui->actionSelfLog->setEnabled(false);

    ui->scriptWidget->setVisible(false);
    ui->scriptStartButton->setEnabled(false);
    ui->scriptStopButton->setEnabled(false);
    ui->scriptPrintButton->setEnabled(false);
    actionScriptWidget->setEnabled(false);
}

void MainWindow::writeLog(const QString &host, const QString &service
                          , const QString &message, QColor color)
{
    ui->logWidget->insertRow(0);
    ui->logWidget->setItem(0, gTime
                           , new QTableWidgetItem(QDateTime::currentDateTime()
                                                  .toString("yyyy.MM.dd hh:mm:ss")));
    ui->logWidget->setItem(0, gHost, new QTableWidgetItem(host));
    ui->logWidget->setItem(0, gService, new QTableWidgetItem(service));
    ui->logWidget->setItem(0, gMessage, new QTableWidgetItem(message));
    ui->logWidget->item(0, gMessage)->setForeground(QBrush(color));
    while (ui->logWidget->rowCount() > mLogSize)
        ui->logWidget->removeRow(mLogSize);
    ui->logWidget->setCurrentCell(0, 0);

    // Alert when failure happened.
    if ( color == Qt::red )
        sound::play( xmlSettings().value("alert").toString() );
}

void MainWindow::onVariantReceived(QVariant v)
{
    if ( v.canConvert<SupervisorProto::Log>() )
    {
        SupervisorProto::Log log = v.value<SupervisorProto::Log>();
        qLogInfo(tr("Receive")) << tr(" Log received: ") << log.name;

        if ( !log.log.isEmpty() )
        {
            LogDialog *logDialog = new LogDialog(this);
            logDialog->setTitle( QString("%1: '%2'")
                                 .arg(toString(log.type))
                                 .arg(log.name) );
            logDialog->setLogText( log.log );
            logDialog->show();
        }
    }
    else if ( v.canConvert<SupervisorProto::Groups>() )
    {
        if ( sender() == mSelectedSocket )
        {
            SupervisorProto::Groups groups = v.value<SupervisorProto::Groups>();

            if ( groups.size() < 2 )
                ui->splitterServices->widget(0)->hide();
            else
            {
                QAbstractItemModel *model = ui->groupsView->model();
                ui->groupsView->selectionModel()->select(
                            QItemSelection(model->index(0,0), model->index(0,model->columnCount()-1)),
                            QItemSelectionModel::ClearAndSelect );
                ui->splitterServices->widget(0)->show();
            }

            ui->actionStartGroup->setDisabled(false);
            ui->pushButtonStartGroup->setDisabled(false);
        }
    }
    else if ( v.canConvert<SupervisorProto::ActiveGroup>() )
    {
        if ( sender() == mSelectedSocket )
        {
            setDisabled(false);
            updateStartGroupUi();
        }
    }
    else if ( v.canConvert<SupervisorProto::NetList>() )
    {
        onNetStatusChanged();
    }
    else if(v.canConvert<SupervisorProto::RemoteOperations>()){
        const SupervisorProto::RemoteOperations op = v.value<SupervisorProto::RemoteOperations>();
        emit RemoteOperationsRecieved(op);
    }
    else if(v.canConvert<SupervisorProto::RemoteLog>()){
        const SupervisorProto::RemoteLog log = v.value<SupervisorProto::RemoteLog>();
        MonitoringSocket *socket = (MonitoringSocket*)sender();
        emit RemoteLogRecieved(QString("%1/%2/").arg(socket->host()).arg(socket->name()), log);
    }
}

void MainWindow::onSocketStateChanged()
{
    MonitoringSocket *socket = qobject_cast<MonitoringSocket *>( sender() );

    if ( socket && socket == mSelectedSocket && !mSelectedSocket->isConnected() )
    {
        socket->clearGroups();
        ui->splitterServices->widget(0)->hide();
    }

    if (mSelectedSocket->isConnected())
    {
        ui->actionServerLog->setEnabled(UserSystem::userClient()->user().hasPermission(UserSystem::Read));
        ui->actionStartAll->setEnabled(UserSystem::userClient()->user().hasPermission(RestartPermission));
        ui->actionStopAll->setEnabled(UserSystem::userClient()->user().hasPermission(RestartPermission));
        ui->actionShutdown->setEnabled(UserSystem::userClient()->user().hasPermission(RebootPermission));
        ui->actionReboot->setEnabled(UserSystem::userClient()->user().hasPermission(RebootPermission));
        updateStartGroupUi();
    } else
    {
        ui->actionServerLog->setEnabled(false);
        ui->actionStartAll->setEnabled(false);
        ui->actionStopAll->setEnabled(false);
        ui->actionShutdown->setEnabled(false);
        ui->actionReboot->setEnabled(false);
        ui->actionStartGroup->setEnabled(false);
        ui->pushButtonStartGroup->setEnabled(false);

        // In case when interface was blocked - unblock it.
        setEnabled(true);
    }
}

void MainWindow::onNetStatusChanged()
{
    if (mSelectedSocket->availableNets().isEmpty())
    {
        ui->netLabel->setText("<html><font color='gray'>"
                              + tr("No available networks")
                              + "</font></html>");
    } else if (mSelectedSocket->availableNets().count() == 1)
    {
        ui->netLabel->setText("<html><font color='gray'>"
                              + tr("Available networks:")
                              + "<br>"
                              + mSelectedSocket->availableNets().at(0)
                              + "<br>"
                              + tr("Reserve network is not available")
                              + "</font></html>");
    } else
    {
        ui->netLabel->setText(tr("Available networks:") + "\n"
                              + mSelectedSocket->availableNets().join("\n"));
    }
}

void MainWindow::onScriptOutputChanged()
{
    ui->scriptLog->setText(mSelectedSocket->scriptOutput().join("\n"));
    ui->scriptLog->verticalScrollBar()->setValue(
                ui->scriptLog->verticalScrollBar()->maximum());

}

void MainWindow::fillScriptList()
{
    ui->scriptList->clear();
    ui->scriptList->insertItems(0, mSelectedSocket->availableScripts());
    ui->scriptList->setCurrentRow(0);
}

void MainWindow::changeEvent(QEvent * event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        updateWindowTittle();
        setLabelText();
        on_actionRefresh_triggered();
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::setLabelText()
{
    serverLabel->setText(tr(" Server: "));
    serverLabel->setToolTip(tr("Server actions"));
    serviceLabel->setText(tr(" Service: "));
    serviceLabel->setToolTip(tr("Service actions"));
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QSettings regSettings;
    regSettings.setValue(gKeySplitterState, ui->splitter->saveState());
    regSettings.setValue(gKeySplitter2State, ui->splitterServices->saveState());
    regSettings.setValue(gKeySplitter3State, ui->splitterLog->saveState());
    regSettings.setValue(gKeyServicesHeaderState, ui->servicesView->header()->saveState());
    regSettings.setValue(gKeyHostsHeaderState, ui->hostsView->horizontalHeader()->saveState());
    regSettings.setValue(gKeyLogHeaderState, ui->logWidget->horizontalHeader()->saveState());
    regSettings.setValue(gKeyWindowState, saveState());
    regSettings.setValue(gKeyWindowGeometry, saveGeometry());
    regSettings.setValue(gKeyGroupsHeaderState, ui->groupsView->header()->saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::onServiceContextMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction( ui->actionRefresh );
    QModelIndex index = ui->servicesView->indexAt(pos);
    // Do not accept any command except 'Refresh' in simplified view mode.
    if ( !gIsViewSimplified && index.isValid() )
    {
        menu.addSeparator();
        menu.addAction( ui->actionLog );
        menu.addAction( ui->actionRestart );

        if ( mSelectedSocket && index.row() < mSelectedSocket->countObjects() )
        {
            const MonitoredObject &obj = mSelectedSocket->object( index.row() );

            // FDP special action support.
            if ( MonitoredObject::FDP == obj.type )
            {
                ui->actionActivate->setEnabled(false);
                if (UserSystem::userClient()->user().hasPermission(UserSystem::Execute))
                    ui->actionActivate->setEnabled( isFdpSelected() );

                menu.addAction( ui->actionActivate );
            }

            if ( !isUserGuest() )
            {
                menu.addSeparator();
                ui->actionProcessStatus->setText( obj.isOn ? tr("Status Off") : tr("Status On") );
                menu.addAction( ui->actionProcessStatus );
            }
        }
    }
    menu.exec( QCursor::pos() );
}

void MainWindow::onHostContextMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction( ui->actionRefresh );
    QModelIndex index = ui->hostsView->indexAt(pos);
    if ( index.isValid() )
    {
        menu.addSeparator();
        menu.addAction( ui->actionServerLog );
        menu.addSeparator();
        menu.addAction( ui->actionStartAll );
        menu.addAction( ui->actionStopAll );
        menu.addSeparator();
        menu.addAction( actionScriptWidget );
        if ( mSelectedSocket )
        {
            mSelectedSocket->setModify(false);
            UserSystem::User user = UserSystem::userClient()->user();
            if ( user.isInGroup( UserSystem::GroupAdministrators::name() )
                 || user.isInGroup( UserSystem::GroupOperators::name() )
                 || user.isInGroup( UserSystem::GroupAdvancedOperators::name() )
                 || user.isInGroup( UserSystem::GroupRoot::name() ) )
            {
                menu.addSeparator();
                ui->actionHostStatus->setText( mSelectedSocket->isStatusOn() ? tr("Status Off") : tr("Status On") );
                menu.addAction( ui->actionHostStatus );
            }
        }
    }
    menu.exec( QCursor::pos() );
}

void MainWindow::onGroupContextMenu(const QPoint &pos)
{
    QAbstractItemModel *model = ui->groupsView->model();
    QModelIndex index = ui->groupsView->indexAt(pos);
    // Display context menu for active group only.
    if ( index.isValid()
         && mSelectedSocket->activeGroup() == model->index(index.row(), 0).data(Qt::UserRole).toString() )
    {
        QMenu menu;
        menu.addAction( ui->actionStartAll );
        menu.addAction( ui->actionStopAll );
        menu.exec( QCursor::pos() );
    }
}

void MainWindow::on_actionLog_triggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Log' triggered");

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not receive log: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
        if ( selIndexes.size() < 1 )
            return;

        int serviceIndex = selIndexes.at(0).row();
        // Selected service
        if ( serviceIndex >= 0 && serviceIndex < socket->countObjects() )
        {
            const MonitoredObject& obj = socket->object(serviceIndex);
            socket->requestLog(obj.type, obj.name);
            qLogInfo(tr("Action")) << tr(" Sending log request to ") << socket->name();
        }
    }
}

void MainWindow::onActionServerLogTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Server Log' triggered");

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not receive server log: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->requestLog(MonitoredObject::Server, socket->name());
        qLogInfo(tr("Action")) << tr(" Sending server log request to ") << socket->name();
    }
}

void MainWindow::on_actionRefresh_triggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Refresh' triggered");
    // Clear gui.
    ui->labelInfo->setText("");
    MonitoringModel *monitoringModel = static_cast<MonitoringModel *>( ui->servicesView->model() );
    monitoringModel->setSocket(0);
    monitoringModel->update();
    GroupsModel *groupsModel = static_cast<GroupsModel *>( ui->groupsView->model() );
    groupsModel->setSocket(0);
    groupsModel->update();

    // Update all monitored objects ...
    for ( TMonitoringSocketList::iterator it = mMonHostList.begin();
          it != mMonHostList.end(); ++it )
    {
        MonitoringSocket *socket = *it;
        socket->clear();

        // Request objects info.
        socket->requestObjects();
        socket->requestAllStatuses();
        socket->requestGroups();
        socket->requestScripts();
    }

    // Update monitored object list for selected host.
    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( !selIndexes.empty() )
        onHostsViewCurrentChanged(selIndexes.at(0), selIndexes.at(0));

    // Update hosts view.
    static_cast<HostsModel *>( ui->hostsView->model() )->update();
    QTimer::singleShot(mResetModifyShot, this, SLOT(resetModify()));
}

void MainWindow::onActionRestartTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Restart' triggered");

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not restart: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
        if ( selIndexes.size() < 1 )
            return;

        int serviceIndex = selIndexes.at(0).row();
        // Selected service
        if ( serviceIndex >= 0 && serviceIndex < socket->countObjects() )
        {
            const MonitoredObject& obj = socket->object(serviceIndex);
            if (confirm(tr("Restart...")
                        , tr("Do you really want to restart '%1'?")
                        .arg(obj.visualName())))
            {
                if (socket->restart(obj.type, obj.name))
                {
                    qLogInfo(tr("Action")) << tr(" Restarting %1 %2 %3").arg(socket->name()
                                                                         , obj.name
                                                                         , toString(obj.type));
                } else
                {
                    qLogInfo(tr("Action")) << tr(" Could not restart %1 %2 %3").arg(socket->name()
                                                                                    , obj.name
                                                                                    , toString(obj.type));
                }
            }
        }
    }
}

void MainWindow::onActionSelfLogTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Self log' triggered");

    LogDialog *logDialog = new LogDialog(this);
    logDialog->setTitle(QString());
    QFile logFile(QDir("bin:").absoluteFilePath(QFileInfo(qApp->applicationFilePath()).baseName()) + ".log");
    logFile.open(QFile::ReadOnly);
    QStringList log;
    while (!logFile.atEnd())
    {
        log << logFile.readLine().trimmed();
    }
    logFile.close();
    logDialog->setLogText(log);
    logDialog->show();
}

void MainWindow::onActionStartAllTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Start All' triggered");

    if (!confirm(tr("Starting all..."), tr("Do you really want to start all services and programs on selected server?")))
        return;

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not start all: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->startAll();
        qLogInfo(tr("Action")) << tr(" Starting all at ") << socket->name();
    }
}

void MainWindow::onActionStopAllTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Stop All' triggered");

    if (!confirm(tr("Stopping all..."), tr("Do you really want to stop all services and programs on selected server?")))
        return;

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not stop all: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->stopAll();
        qLogInfo(tr("Action")) << tr(" Stopping all at ") << socket->name();
    }
}

void MainWindow::onActionShutdownTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Shutdown' triggered");

    if (!confirm(tr("Shutting down..."), tr("Do you really want to shutdown selected server?")))
        return;

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not shutdown: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->shutdown();
        qLogInfo(tr("Action")) << tr(" Shutting down ") << socket->name();
    }
}

void MainWindow::onActionRebootTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Reboot' triggered");

    if (!confirm(tr("Rebooting..."), tr("Do you really want to reboot selected server?")))
        return;

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not reboot: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->reboot();
        qLogInfo(tr("Action")) << tr(" Rebooting ") << socket->name();
    }
}

void MainWindow::onActionActivateTriggered()
{
    QModelIndexList selIndexes = ui->servicesView->selectionModel()->selectedRows();
    if ( !mSelectedSocket || selIndexes.size() < 1 )
        return;

    int row = selIndexes.at(0).row();
    if ( row < 0 || row >= mSelectedSocket->countObjects() )
        return;

    const MonitoredObject &obj = mSelectedSocket->object(row);

    qLogInfo(tr("Action")) << tr(" Action 'Activate' triggered for '%1' process/service").arg(obj.visualName());

    if ( MonitoredObject::FDP == obj.type )
    {
        mSelectedSocket->activateFdp();
        qLogInfo(tr("Action")) << tr(" Sending activate request to '%1' process/service of '%2' host")
                                  .arg(obj.visualName()).arg(mSelectedSocket->name());
    }
}

void MainWindow::onActionRemoteOperationsTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Remote Operations' triggered");

    CRemoteOperationsDialog* dialog = new CRemoteOperationsDialog(this);
    connect(this, SIGNAL(RemoteOperationsRecieved(SupervisorProto::RemoteOperations))
            , dialog, SLOT(onRemoteOperationsRecieved(SupervisorProto::RemoteOperations)));
    dialog->show();

}

void MainWindow::onActionGroupConfigTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Group Config' triggered");

    GroupConfigDialog* dialog = new GroupConfigDialog(this);

    connect(dialog, SIGNAL(requestServers()), this, SLOT(onServersRequested()));
    connect(dialog, SIGNAL(requestGroups(QString)), this, SLOT(onGroupsRequested(QString)));
    connect(dialog, SIGNAL(requestScripts(QString)), this, SLOT(onScriptsRequested(QString)));
    connect(dialog, SIGNAL(finished(int)), this, SLOT(loadGroupCommandList()));

    connect(this, SIGNAL(serversResponse(QStringList)), dialog, SLOT(setServers(QStringList)));
    connect(this, SIGNAL(groupsResponse(QStringList)), dialog, SLOT(setAvailableOptions(QStringList)));
    connect(this, SIGNAL(scriptsResponse(QStringList)), dialog, SLOT(setAvailableOptions(QStringList)));

    dialog->init();
    dialog->show();

}

void MainWindow::onActionLogCollectorTriggered()
{
    qLogInfo(tr("Action")) << tr(" Action 'Log Collector' triggered");

    CLogCollectorDialog* dialog = new CLogCollectorDialog(this);
    connect(this, SIGNAL(RemoteLogRecieved(QString,SupervisorProto::RemoteLog))
            , dialog, SLOT(onRemoteLogRecieved(QString,SupervisorProto::RemoteLog)));

    dialog->setFlashPath(xmlSettings().value(QString("flash_path"), QString("/")).toString());
    dialog->show();
}

void MainWindow::onStartScript()
{
    qLogInfo(tr("Action")) << tr(" Action 'Start script' triggered");

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not start script: selIndexes.size() < 1");
        return;
    }
    if ( !ui->scriptList->currentIndex().isValid() )
    {
        qLogWarn(tr("Action")) << tr(" Can not start script: no script was selected");
        QMessageBox::warning(this, tr("Error"), tr("You must selet script first"));
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->startScript(ui->scriptList->currentItem()->text()
                            , ui->scriptParameter->text());
        qLogInfo(tr("Action")) << tr(" Sending start script request to ") << socket->name();
    }
}

void MainWindow::onStopScript()
{
    qLogInfo(tr("Action")) << tr(" Action 'Stop script' triggered");

    QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
    {
        qLogWarn(tr("Action")) << tr(" Can not stop script: selIndexes.size() < 1");
        return;
    }

    int hostIndex = selIndexes.at(0).row();
    // Selected host
    if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mMonHostList[hostIndex];

        socket->stopScript();
        qLogInfo(tr("Action")) << tr(" Sending stop script request to ") << socket->name();
    }
}

void MainWindow::onPrintScript()
{
    ui->scriptLog->setFocus();
    emit print();
}

void MainWindow::on_pushButtonStartGroup_clicked()
{
    QString messageTittle = tr("Group activation");

    QModelIndexList selIndexes = ui->groupsView->selectionModel()->selectedIndexes();
    if ( selIndexes.empty() )
    {
        QMessageBox::information(this, messageTittle,
                                 tr("There is no selected group to start. Select desired group from the list please."));
        return;
    }

    QModelIndex index = selIndexes.at(0);

    GroupsModel *groupsModel = static_cast<GroupsModel *>( ui->groupsView->model() );
    MonitoringSocket *monSocket = groupsModel->socket();

    QString groupName = groupsModel->data(index, Qt::UserRole).toString();
    // Selected group is already active - do nothing.
    if ( groupName == monSocket->activeGroup() )
        return;

    QString visualNameActiveGroup;
    int indexActiveGroup = monSocket->findGroupIndex(monSocket->activeGroup());
    if ( indexActiveGroup != -1 )
        visualNameActiveGroup = monSocket->group(indexActiveGroup).visualName;

    if ( QMessageBox::question(this, messageTittle,
                               tr("Would you really like to start new group '%1'?\n").arg(groupsModel->data(index).toString())
                               + tr("All processes/services for active group '%1' will be stopped.").arg(visualNameActiveGroup),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes )
    {
        QModelIndexList selIndexes = ui->hostsView->selectionModel()->selectedIndexes();
        if ( selIndexes.size() < 1 )
        {
            qLogDebug() << "No selected host to finish 'activate group' operation.";
            return;
        }

        ui->actionStartGroup->setDisabled(true);
        ui->pushButtonStartGroup->setDisabled(true);
        setDisabled(true);

        monSocket->clearObjects();
        monSocket->activateGroup(groupName);

        qLogInfo(tr("Action")) << tr(" Request to start '%1' group sent").arg(groupName);
    }
}

void MainWindow::on_actionProcessStatus_triggered()
{
    Q_ASSERT(ui->servicesView->selectionModel());

    QModelIndexList selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
        return;

    int serviceIndex = selIndexes.at(0).row();
    // Selected service
    if ( mSelectedSocket && serviceIndex >= 0 && serviceIndex < mSelectedSocket->countObjects() )
    {
        const MonitoredObject& obj = mSelectedSocket->object(serviceIndex);
        const_cast<MonitoredObject *>(&obj)->isOn = !obj.isOn;

        QSettings s;
        s.beginGroup("status/groups");
        s.setValue(obj.name, obj.isOn);
        s.endGroup();

        qLogInfo(tr("Action")) << tr(" Status of monitored object '%1' has been changed to '%2'")
                                  .arg(obj.visualName()).arg(obj.isOn ? tr("On") : tr("Off"));
    }
}

void MainWindow::on_actionHostStatus_triggered()
{
    if ( mSelectedSocket )
    {
        mSelectedSocket->changeStatus( !mSelectedSocket->isStatusOn() );

        QSettings s;
        s.beginGroup("status/hosts");
        s.setValue(mSelectedSocket->name(), mSelectedSocket->isStatusOn());
        s.endGroup();

        qLogInfo(tr("Action")) << tr(" Status of host '%1' has been changed to '%2'")
                                  .arg(mSelectedSocket->name()).arg(mSelectedSocket->isStatusOn() ? tr("On") : tr("Off"));
    }
}

void MainWindow::on_actionStartGroup_triggered()
{
    on_pushButtonStartGroup_clicked();
}

void MainWindow::onGroupsViewCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    bool isDisableNeeded = false;

    GroupsModel *groupsModel = static_cast<GroupsModel *>( ui->groupsView->model() );
    MonitoringSocket *monSocket = groupsModel->socket();

    if ( !current.isValid() || current.row() >= monSocket->countGroups() )
        isDisableNeeded = true;
    else
    {
        if ( groupsModel->data(groupsModel->index(current.row(),0), Qt::UserRole).toString() == monSocket->activeGroup() )
            isDisableNeeded = true;
    }

    isDisableNeeded |= !UserSystem::userClient()->user().hasPermission(RestartPermission);

    ui->pushButtonStartGroup->setDisabled(isDisableNeeded);
    ui->actionStartGroup->setDisabled(isDisableNeeded);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About %1 ...").arg(tr(gWindowTitle))
                       , tr("%1, Version %2\n\n").arg(tr(gWindowTitle), qApp->applicationVersion())
                       + tr("Build date: %1\n\n").arg(buildDate())
                       );
//                       + QApplication::translate("MainWindow", "Copyright \302\251 2010 ITOS. All rights reserved.\n\n"
//                                                 , 0, QApplication::UnicodeUTF8)
//                       + QApplication::translate("MainWindow", "This program is protected by copyright law and international treaties. Unauthorized reproduction or distribution of this program, or any portion of it, may result in severe civil and criminal penalties, and will be prosecuted to the maximum extent possible under the law.", 0, QApplication::UnicodeUTF8));
}

void MainWindow::createStatusBar()
{
    // clock
    m_pClockLabel = new QLabel("99.99.99 99:99:99", this);
    m_pClockLabel->setAlignment(Qt::AlignRight);
    m_pClockLabel->setMinimumSize(m_pClockLabel->sizeHint());
    statusBar()->addPermanentWidget(m_pClockLabel);
    m_pClockTimer = new QTimer(this);
    connect(m_pClockTimer, SIGNAL(timeout()), this, SLOT(onClockTimer()));
    m_pClockTimer->start(1000);
    m_pClockLabel->setText("");
}

void MainWindow::onClockTimer()
{
    QDateTime dt = QDateTime::currentDateTime();
    m_pClockLabel->setText(dt.toString("dd.MM.yyyy hh:mm:ss"));
}

void MainWindow::on_actionFont_triggered()
{
    FontColorsDialog dlg(mAppSettings, this);
    if(dlg.exec() == QDialog::Accepted)
    {
        mAppSettings = dlg.getAppSettings();
        QSettings settings;
        settings.setValue("AppSettings", mAppSettings.toByteArray());

        qApp->setFont(mAppSettings.getFont());
    }
}

void MainWindow::resetModify()
{
    foreach(MonitoringSocket *socket, mMonHostList)
        socket->setModify(false);
    static_cast<HostsModel *>( ui->hostsView->model() )->update();
}

void MainWindow::onServersRequested()
{
    QStringList result;
    foreach(MonitoringSocket *socket, mMonHostList)
    {
        result.append(socket->name());
    }
    emit serversResponse(result);
}

void MainWindow::onGroupsRequested(QString host)
{
    QStringList result;
    QString item;
    foreach(MonitoringSocket *socket, mMonHostList)
    {
        if(socket->name() == host)
        {
            for(int i = 0; i < socket->countGroups(); ++i)
            {
                item = socket->group(i).visualName;
                result.append(item);
            }
        }
    }
    result.removeDuplicates();
    emit groupsResponse(result);
}

void MainWindow::onScriptsRequested(QString host)
{
    QStringList result;
    foreach(MonitoringSocket *socket, mMonHostList)
    {
        if(socket->name() == host)
            result.append(socket->availableScripts());
    }
    result.removeDuplicates();
    emit scriptsResponse(result);
}

void MainWindow::loadGroupCommandList()
{
    ui->menuConfig->clear();
    foreach(QString config, GroupConfigDialog::getConfigNames())
    {
        ui->menuConfig->addAction(config);
    }
}
