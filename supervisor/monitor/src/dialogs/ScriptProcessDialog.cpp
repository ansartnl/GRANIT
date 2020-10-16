#include "ScriptProcessDialog.h"
#include "ui_ScriptProcessDialog.h"

#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>

#include "src/connections/supervisorServer/MonitoringSocket.h"
#include "src/models/MonitoringModel.h"
#include "Log.h"
#include "LogDialog.h"

ScriptProcessDialog::ScriptProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptProcessDialog),
    mCurrentSocket(0)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_OS_WIN
    setWindowFlags(Qt::WindowStaysOnTopHint);
#endif

    ui->servicesView->setModel( new MonitoringModel(ui->servicesView) );
    ui->servicesView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->servicesView->horizontalHeader()->setStretchLastSection(true);

    QSettings settings;
    restoreGeometry( settings.value("ScriptProcessDialogGeometry").toByteArray() );
    ui->servicesView->horizontalHeader()->restoreState(settings.value("ScriptProcessViewState").toByteArray() );

    setupActions();

    connect(ui->servicesView, SIGNAL(customContextMenuRequested(QPoint))
            , SLOT(onServiceContextMenu(QPoint)));
    connect(ui->servicesView->selectionModel()
            , SIGNAL(currentChanged(QModelIndex,QModelIndex))
            , SLOT(onServicesViewCurrentChanged(QModelIndex,QModelIndex)));

    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->buttonStartScript, SIGNAL(clicked()), this, SLOT(onStartScript()));
    connect(ui->buttonStopScript, SIGNAL(clicked()), this, SLOT(onStopScript()));
    connect(ui->labelGroup, SIGNAL(clicked()), this, SLOT(onGroupLabelClicked()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onGroupLabelClicked()));

}

ScriptProcessDialog::~ScriptProcessDialog()
{
    delete ui;
}

void ScriptProcessDialog::setNetworks(const QStringList &networks)
{
    ui->labelNetworks->setText(QString("Available Networks: %1")
                               .arg(networks.join(", ")));
}

void ScriptProcessDialog::setGroups(const SupervisorProto::Groups &groups)
{
    foreach(QAction* action, ui->labelGroup->actions())
        ui->labelGroup->removeAction(action);

    SupervisorProto::Groups tmp = groups;
    if(mCurrentSocket)
    {
        int idx = mCurrentSocket->findGroupIndex(mCurrentSocket->activeGroup());
        if(idx >= 0 && idx < mCurrentSocket->countGroups())
            tmp.move(idx, 0);
    }

    foreach(const SupervisorProto::Group& group, tmp)
    {
        QAction* action = new QAction(group.visualName, ui->labelGroup);
        action->setData(group.name);
        connect(action, SIGNAL(triggered()), this, SLOT(onGroupActionTriggered()));
        ui->labelGroup->addAction(action);
    }

    updateActiveGroupName();
}

void ScriptProcessDialog::setServices(const SupervisorProto::Services &services)
{
    Q_UNUSED(services)
}

void ScriptProcessDialog::setScripts(const SupervisorProto::ScriptList &scripts)
{
    ui->comboScripts->clear();
    ui->comboScripts->addItems(scripts.scripts);
}

void ScriptProcessDialog::setScripts(const QStringList &scripts)
{
    ui->comboScripts->clear();
    ui->comboScripts->addItems(scripts);
}

void ScriptProcessDialog::setDataConnection(MonitoringSocket *socket)
{
    if(mCurrentSocket)
    {
        mCurrentSocket->disconnect(this);
    }
    mCurrentSocket = socket;

    SupervisorProto::Groups groups;
    for(int i = 0; i < mCurrentSocket->countGroups(); ++i)
    {
        SupervisorProto::Group group;
        group.name = mCurrentSocket->group(i).name;
        group.visualName = mCurrentSocket->group(i).visualName;
        groups.append(group);
    }
    setGroups(groups);
    setNetworks(mCurrentSocket->availableNets());
    setScripts(mCurrentSocket->availableScripts());
    setWindowTitle(mCurrentSocket->name());

    connect(mCurrentSocket, SIGNAL(newVariantMessage(QVariant)), this, SLOT(onVariantRecieved(QVariant)));
    connect(mCurrentSocket, SIGNAL(scriptOutputChanged()), SLOT(onScriptOutputChanged()));

    mCurrentSocket->setModify(false);
    onRefresh();
}

void ScriptProcessDialog::closeEvent(QCloseEvent *event)
{
    QSettings regSettings;
    regSettings.setValue("ScriptProcessDialogGeometry", saveGeometry());
    regSettings.setValue("ScriptProcessViewState", ui->servicesView->horizontalHeader()->saveState());
    QDialog::closeEvent(event);
}

//void ScriptProcessDialog::onGroupLabelClicked()
//{
//    ui->labelGroup->showContextMenu(QCursor::pos());
//}

void ScriptProcessDialog::on_pushButton_clicked()
{
    ui->labelGroup->showContextMenu(QCursor::pos());
}

void ScriptProcessDialog::onStartScript()
{
    // Selected host
    {
        MonitoringSocket *socket = mCurrentSocket;

        socket->startScript(ui->comboScripts->currentText()
                            , ui->lineArguments->text());
        qLogInfo(tr("Action")) << tr(" Sending start script request to ") << socket->name();
    }
}

void ScriptProcessDialog::onStopScript()
{
    MonitoringSocket *socket = mCurrentSocket;

    socket->stopScript(ui->comboScripts->currentText());
    qLogInfo(tr("Action")) << tr(" Sending stop script request to ") << socket->name();
}

void ScriptProcessDialog::onServiceContextMenu(const QPoint &pos)
{
    QMenu menu;
    //menu.addAction( ui->actionRefresh );
    QModelIndex index = ui->servicesView->indexAt(pos);

    if ( index.isValid() )
    {
        //menu.addSeparator();
        menu.addAction( mActionServiceLog );
        menu.addAction( mActionServiceRestart );

        if ( mCurrentSocket && index.row() < mCurrentSocket->countObjects() )
        {
            const MonitoredObject &obj = mCurrentSocket->object( index.row() );

            // FDP special action support.
            if ( MonitoredObject::FDP == obj.type )
            {
//                ui->actionActivate->setEnabled(false);
//                if (UserSystem::userClient()->user().hasPermission(UserSystem::Execute))
//                    ui->actionActivate->setEnabled( isFdpSelected() );

//                menu.addAction( ui->actionActivate );
            }

            //if ( !isUserGuest() )
            {
                menu.addSeparator();
                mActionServiceStatusToggle->setText( obj.isOn ? tr("Status Off") : tr("Status On") );
                menu.addAction( mActionServiceStatusToggle );
            }
        }
    }
    menu.exec( QCursor::pos() );
}

void ScriptProcessDialog::onServicesViewCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    Q_UNUSED(current)
}

void ScriptProcessDialog::onRefresh()
{
    MonitoringModel *monitoringModel = static_cast<MonitoringModel *>( ui->servicesView->model() );
    monitoringModel->setSocket( 0 );
    monitoringModel->update();

    monitoringModel->setSocket( mCurrentSocket );
    monitoringModel->update();
}

void ScriptProcessDialog::onServiceLog()
{
    // Selected host
    {
        MonitoringSocket *socket = mCurrentSocket;

        QModelIndexList selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
        if ( selIndexes.isEmpty() )
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

void ScriptProcessDialog::onServiceRestart()
{
    // Selected host
    //if ( hostIndex >= 0 && hostIndex < mMonHostList.size() )
    {
        MonitoringSocket *socket = mCurrentSocket;

        QModelIndexList selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
        if ( selIndexes.isEmpty() )
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

void ScriptProcessDialog::onServiceStatusToggle()
{
    QModelIndexList selIndexes = ui->servicesView->selectionModel()->selectedIndexes();
    if ( selIndexes.size() < 1 )
        return;

    int serviceIndex = selIndexes.at(0).row();
    // Selected service
    if ( mCurrentSocket && serviceIndex >= 0 && serviceIndex < mCurrentSocket->countObjects() )
    {
        const MonitoredObject& obj = mCurrentSocket->object(serviceIndex);
        const_cast<MonitoredObject *>(&obj)->isOn = !obj.isOn;

        QSettings s;
        s.beginGroup("status/groups");
        s.setValue(obj.name, obj.isOn);
        s.endGroup();

        qLogInfo(tr("Action")) << tr(" Status of monitored object '%1' has been changed to '%2'")
                                  .arg(obj.visualName()).arg(obj.isOn ? tr("On") : tr("Off"));
    }
}

void ScriptProcessDialog::onVariantRecieved(QVariant v)
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
        setGroups(v.value<SupervisorProto::Groups>());
    }
    else if ( v.canConvert<SupervisorProto::ActiveGroup>() )
    {
        updateActiveGroupName();
    }
    else if ( v.canConvert<SupervisorProto::ScriptList>() )
    {
        setScripts(v.value<SupervisorProto::ScriptList>());
    }
    else if ( v.canConvert<SupervisorProto::NetList>() )
    {
        setNetworks(v.value<SupervisorProto::NetList>().list);
    }

    if(mCurrentSocket)
        mCurrentSocket->setModify(false);
}

void ScriptProcessDialog::updateActiveGroupName()
{
    if(!mCurrentSocket)
        return;

    int idx = mCurrentSocket->findGroupIndex(mCurrentSocket->activeGroup());
    if(mCurrentSocket->countGroups() == 1 || mCurrentSocket->countGroups() == 0)
    {
        ui->pushButton->hide();
        return;
    }
    if(idx >= 0 && idx < mCurrentSocket->countGroups())
    {
//        ui->labelGroup->setText(mCurrentSocket->group(idx).visualName);
        ui->labelGroup->setText(QString());
        ui->pushButton->setText(mCurrentSocket->group(idx).visualName);
    }
    else
    {
//        ui->labelGroup->setText(QString());
        ui->pushButton->setText(QString());
    }
}

void ScriptProcessDialog::onScriptOutputChanged()
{
    ui->scriptLog->setText(mCurrentSocket->scriptOutput().join("\n"));
    ui->scriptLog->verticalScrollBar()->setValue(
                ui->scriptLog->verticalScrollBar()->maximum());
}

void ScriptProcessDialog::onGroupActionTriggered()
{
    QAction* action = (QAction*)sender();
    if(action &&
            QMessageBox::question(this, tr("Group activation"),
                                           tr("Would you really like to start new group '%1'?\n").arg(action->text())
                                           + tr("All processes/services for active group will be stopped."),
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes )
        mCurrentSocket->activateGroup(action->data().toString());
}

void ScriptProcessDialog::setupActions()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    mActionServiceRestart = new QAction(this);
    //mActionServiceRestart->setIcon(QIcon(":/images/Restart"));
    mActionServiceRestart->setText(QApplication::translate("MainWindow", "Restart", 0, QApplication::UnicodeUTF8));
    mActionServiceRestart->setToolTip(QApplication::translate("MainWindow", "Restart the selected service", 0, QApplication::UnicodeUTF8));

    mActionServiceLog = new QAction(this);
    //mActionServiceLog->setIcon(QIcon(":/images/Log"));
    mActionServiceLog->setText(QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
    mActionServiceLog->setToolTip(QApplication::translate("MainWindow", "Show log for selected service", 0, QApplication::UnicodeUTF8));

    mActionRefresh = new QAction(this);
    //mActionRefresh->setIcon(QIcon(":/images/Refresh"));
    mActionRefresh->setText(QApplication::translate("MainWindow", "Refresh", 0, QApplication::UnicodeUTF8));
    mActionRefresh->setToolTip(QApplication::translate("MainWindow", "Refresh connections information", 0, QApplication::UnicodeUTF8));

    mActionServiceStatusToggle = new QAction(this);
    mActionServiceStatusToggle->setText(QApplication::translate("MainWindow", "Status", 0, QApplication::UnicodeUTF8));
    mActionServiceStatusToggle->setToolTip(QApplication::translate("MainWindow", "Status of selected process/service", 0, QApplication::UnicodeUTF8));
#else
    mActionServiceRestart = new QAction(this);
    //mActionServiceRestart->setIcon(QIcon(":/images/Restart"));
    mActionServiceRestart->setText(QApplication::translate("MainWindow", "Restart", 0));
    mActionServiceRestart->setToolTip(QApplication::translate("MainWindow", "Restart the selected service", 0));

    mActionServiceLog = new QAction(this);
    //mActionServiceLog->setIcon(QIcon(":/images/Log"));
    mActionServiceLog->setText(QApplication::translate("MainWindow", "Log", 0));
    mActionServiceLog->setToolTip(QApplication::translate("MainWindow", "Show log for selected service", 0));

    mActionRefresh = new QAction(this);
    //mActionRefresh->setIcon(QIcon(":/images/Refresh"));
    mActionRefresh->setText(QApplication::translate("MainWindow", "Refresh", 0));
    mActionRefresh->setToolTip(QApplication::translate("MainWindow", "Refresh connections information", 0));

    mActionServiceStatusToggle = new QAction(this);
    mActionServiceStatusToggle->setText(QApplication::translate("MainWindow", "Status", 0));
    mActionServiceStatusToggle->setToolTip(QApplication::translate("MainWindow", "Status of selected process/service", 0));
#endif

    connect(mActionServiceRestart, SIGNAL(triggered()), this, SLOT(onServiceRestart()));
    connect(mActionRefresh, SIGNAL(triggered()), this, SLOT(onRefresh()));
    connect(mActionServiceLog, SIGNAL(triggered()), this, SLOT(onServiceLog()));
    connect(mActionServiceStatusToggle, SIGNAL(triggered()), this, SLOT(onServiceStatusToggle()));
}

bool ScriptProcessDialog::confirm(const QString &title, const QString &text)
{
    return (QMessageBox::Yes == QMessageBox::question(this, title, text
                                                      , QMessageBox::Yes | QMessageBox::No));
}
