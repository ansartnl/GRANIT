#include "ManagerWindow.h"

#include "NetSnmpManager.h"

#include <QtCore/QDateTime>
#include <QtCore/QThread>

#include <QScrollBar>

#include <QtDebug>

using namespace snmp;

ManagerWindow::ManagerWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    QThread *thread = new QThread(this);
    NetSnmpManager::instance()->setRemoveHostsOnStop(true);
    netSnmpManager->moveToThread(thread);

    connect(thread, SIGNAL(started()), netSnmpManager, SLOT(start()));
    connect(netSnmpManager, SIGNAL(stopped()), thread, SLOT(quit()));

    connect(netSnmpManager, SIGNAL(variableChanged(snmp::SnmpVariable))
            , SLOT(onVariableChanged(snmp::SnmpVariable)));
    connect(netSnmpManager, SIGNAL(scriptStateChanged(snmp::SnmpScript))
            , SLOT(onScriptStateChanged(snmp::SnmpScript)));
    connect(netSnmpManager, SIGNAL(programStateChanged(snmp::SnmpProgramState))
            , SLOT(onProgramStateChanged(snmp::SnmpProgramState)));
    connect(netSnmpManager, SIGNAL(trapCame(snmp::SnmpTrap)), SLOT(onTrapCame(snmp::SnmpTrap)));
    connect(netSnmpManager, SIGNAL(error(QString))
            , SLOT(onError(QString)));
    connect(this, SIGNAL(startScript(snmp::SnmpScript)), netSnmpManager, SLOT(startScript(snmp::SnmpScript)));
    connect(this, SIGNAL(setVariable(snmp::SnmpVariable)), netSnmpManager, SLOT(setVariable(snmp::SnmpVariable)));

    connect(this, SIGNAL(start()), thread, SLOT(start()));
    connect(this, SIGNAL(stop()), netSnmpManager, SLOT(stop()));

    connect(netSnmpManager, SIGNAL(started()), SLOT(onStarted()));
    connect(thread, SIGNAL(finished()), SLOT(onStopped()));

    connect(netSnmpManager, SIGNAL(connected(QString)), SLOT(onConnected(QString)));
    connect(netSnmpManager, SIGNAL(lostConnection(QString)), SLOT(onLostConnection(QString)));

    connect(pushButton, SIGNAL(clicked()), SLOT(onPushButton()));
    connect(runButton, SIGNAL(clicked()), SLOT(onRunButton()));
    connect(setButton, SIGNAL(clicked()), SLOT(onSetButton()));

    connect(scripts->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(onScriptSelected(QModelIndex)));
    connect(variables->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex))
            , SLOT(onVariableSelected(QModelIndex)));

    scripts->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    scripts->horizontalHeader()->hideSection(STC_STATE_INT);
    scripts->horizontalHeader()->hideSection(STC_KEY);

    variables->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    variables->horizontalHeader()->hideSection(VTC_KEY);

    programStates->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    onScriptSelected(QModelIndex());
    onVariableSelected(QModelIndex());
}

ManagerWindow::~ManagerWindow()
{
    emit stop();
}

void ManagerWindow::clearScriptsTable()
{
    while (scripts->rowCount())
        scripts->removeRow(0);
}

void ManagerWindow::clearVariablesTable()
{
    while (variables->rowCount())
        variables->removeRow(0);
}

void ManagerWindow::clearProgramStatesTable()
{
    while (programStates->rowCount())
        programStates->removeRow(0);
}

void ManagerWindow::clearConnectionStatesTable()
{
    while (connectionStates->rowCount())
        connectionStates->removeRow(0);
}

void ManagerWindow::updateScriptsTable()
{
    clearScriptsTable();

    const QList < QString > keys = mScripts.keys();
    int row = 0;
    foreach (const QString &key, keys) {
    const snmp::SnmpScript ss = mScripts[key];
        scripts->insertRow(row);
        scripts->setItem(row, STC_HOST, new QTableWidgetItem(ss.host));
        scripts->setItem(row, STC_MIB, new QTableWidgetItem(ss.mib));
        scripts->setItem(row, STC_NAME, new QTableWidgetItem(ss.name));
        scripts->setItem(row, STC_INDEX, new QTableWidgetItem(QString::number(ss.index)));
        scripts->setItem(row, STC_STATE, new QTableWidgetItem(ss.toString()));
        scripts->setItem(row, STC_STATE_INT, new QTableWidgetItem(QString::number(ss.state)));
        scripts->setItem(row, STC_KEY, new QTableWidgetItem(key));
        ++row;
    }
}

void ManagerWindow::updateVariablesTable()
{
    const int scrollPos = variables->verticalScrollBar()->value();
    const QModelIndex lastIndex = variables->selectionModel()->currentIndex();
    const QString value = variableValue->text();
    const int cursorPos = variableValue->cursorPosition();

    clearVariablesTable();

    const QList < QString > keys = mVariables.keys();
    int row = 0;
    foreach (const QString &key, keys) {
    const snmp::SnmpVariable sv = mVariables[key];
        variables->insertRow(row);
        variables->setItem(row, VTC_HOST, new QTableWidgetItem(sv.host));
        variables->setItem(row, VTC_MIB, new QTableWidgetItem(sv.mib));
        variables->setItem(row, VTC_NAME, new QTableWidgetItem(sv.name));
        variables->setItem(row, VTC_INDEX, new QTableWidgetItem(QString::number(sv.index)));
        variables->setItem(row, VTC_VALUE, new QTableWidgetItem(sv.value.toString()));
        variables->setItem(row, VTC_TYPE, new QTableWidgetItem(sv.typeString()));
        variables->setItem(row, VTC_WRITABLE, new QTableWidgetItem(
                               sv.writable ? QString("True") : QString("False")));
        variables->setItem(row, VTC_KEY, new QTableWidgetItem(key));
        ++row;
    }

    variables->selectionModel()->setCurrentIndex(lastIndex
                                                 , QItemSelectionModel::Select
                                                 | QItemSelectionModel::Rows);
    variables->verticalScrollBar()->setValue(scrollPos);
    variableValue->setText(value);
    variableValue->setCursorPosition(cursorPos);
}

void ManagerWindow::updateProgramStatesTable()
{
    clearProgramStatesTable();

    const QList < QString > keys = mProgramStates.keys();
    int row = 0;
    foreach (const QString &key, keys) {
    const snmp::SnmpProgramState sps = mProgramStates[key];
        programStates->insertRow(row);
        programStates->setItem(row, PSTC_HOST, new QTableWidgetItem(sps.host));
        programStates->setItem(row, PSTC_MIB, new QTableWidgetItem(sps.mib));
        programStates->setItem(row, PSTC_NAME, new QTableWidgetItem(sps.name));
        programStates->setItem(row, PSTC_INDEX, new QTableWidgetItem(QString::number(sps.index)));
        programStates->setItem(row, PSTC_STATE, new QTableWidgetItem(sps.toString()));
        programStates->item(row, PSTC_STATE)->setBackground(
                    sps.state == snmp::SnmpProgramState::PS_UP ? Qt::green : Qt::red);
        ++row;
    }
}

void ManagerWindow::updateConnectionStatesTable()
{
    clearConnectionStatesTable();

    const QList < QString > keys = mConnectionStates.keys();
    int row = 0;
    foreach (const QString &key, keys) {
    const bool cs = mConnectionStates[key];
        connectionStates->insertRow(row);
        connectionStates->setItem(row, CSTC_HOST, new QTableWidgetItem(key));
        connectionStates->setItem(row, CSTC_STATE, new QTableWidgetItem(
                                      cs ? QString("CONNECTED") : QString("DISCONNECTED")));
        connectionStates->item(row, CSTC_STATE)->setTextAlignment(Qt::AlignHCenter);
        connectionStates->item(row, CSTC_HOST)->setBackground(cs ? Qt::green : Qt::red);
        connectionStates->item(row, CSTC_STATE)->setBackground(cs ? Qt::green : Qt::red);
        ++row;
    }
}

void ManagerWindow::onPushButton()
{
    if (netSnmpManager->isRunning()) {
        pushButton->setEnabled(false);
        clearScriptsTable();
        clearVariablesTable();
        clearProgramStatesTable();
        clearConnectionStatesTable();
        emit stop();
    } else {
        pushButton->setEnabled(false);
        parameters->setEnabled(false);
        mScripts.clear();
        mVariables.clear();
        mProgramStates.clear();
        mConnectionStates.clear();
        updateScriptsTable();
        updateVariablesTable();
        updateProgramStatesTable();
        log->clear();
        netSnmpManager->setBaseOid(baseOid->text());
        netSnmpManager->setCommunity(community->text());
        netSnmpManager->setTimerIntervalMs(intervalMs->value());
        const QList < QString > hostsList = hosts->text().split(",");
        foreach (const QString &h, hostsList) {
            mConnectionStates.insert(h, false);
            netSnmpManager->addHost(h);
        }
        updateConnectionStatesTable();
        emit start();
    }
}

void ManagerWindow::onRunButton()
{
    log->append(QString("[%1] info - Trying to run %2")
                .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                .arg(mSelectedScript));
    snmp::SnmpScript script = mScripts[mSelectedScript];
    emit startScript(script);
}

void ManagerWindow::onSetButton()
{
    log->append(QString("[%1] info - Trying to update %2")
                .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                .arg(mSelectedVariable));
    snmp::SnmpVariable var = mVariables[mSelectedVariable];
    var.value = variableValue->text();
    emit setVariable(var);
}

void ManagerWindow::onStarted()
{
    pushButton->setText("Stop");

    log->append(QString("[%1] info - Started").arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz")));

    pushButton->setEnabled(true);
}

void ManagerWindow::onStopped()
{
    pushButton->setText("Start");
    parameters->setEnabled(true);

    log->append(QString("[%1] info - Stopped").arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz")));

    pushButton->setEnabled(true);
}

void ManagerWindow::onConnected(const QString &host)
{
    if (!netSnmpManager->isRunning())
        return;
    const bool isConnected = mConnectionStates[host];
    if (!isConnected) {
        log->append(QString("[%1] info - Connected to %2")
                    .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                    .arg(host));
        mConnectionStates.insert(host, true);
        updateConnectionStatesTable();
    }
}

void ManagerWindow::onLostConnection(const QString &host)
{
    if (!netSnmpManager->isRunning())
        return;
    const bool isConnected = mConnectionStates[host];
    if (isConnected) {
        log->append(QString("[%1] info - Disconnected from %2")
                    .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                    .arg(host));
        mConnectionStates.insert(host, false);
        updateConnectionStatesTable();
    }
}

void ManagerWindow::onScriptSelected(const QModelIndex &index)
{
    if (!index.isValid()) {
        scriptName->clear();
        runButton->setEnabled(false);
        mSelectedScript.clear();
        return;
    }

    scriptName->setText(scripts->item(index.row(), STC_NAME)->text());

    runButton->setEnabled(scripts->item(index.row(), STC_STATE_INT)->text().toInt()
                          == snmp::SnmpScript::RS_STOPPED);

    mSelectedScript = scripts->item(index.row(), STC_KEY)->text();
}

void ManagerWindow::onVariableSelected(const QModelIndex &index)
{
    if (!index.isValid()) {
        variableName->clear();
        variableValue->clear();
        setButton->setEnabled(false);
        mSelectedVariable.clear();
        return;
    }

    variableName->setText(variables->item(index.row(), VTC_NAME)->text());
    variableValue->setText(variables->item(index.row(), VTC_VALUE)->text());

    const bool isWritable = variables->item(index.row(), VTC_WRITABLE)->text() == QString("True");
    variableValue->setReadOnly(!isWritable);
    setButton->setEnabled(isWritable);

    mSelectedVariable = variables->item(index.row(), VTC_KEY)->text();
}

void ManagerWindow::onScriptStateChanged(const SnmpScript &ss)
{
    if (!netSnmpManager->isRunning())
        return;
    mScripts.insert(QString("%1/%2").arg(ss.host).arg(ss.oidString()), ss);
    updateScriptsTable();
}

void ManagerWindow::onVariableChanged(const SnmpVariable &sv)
{
    if (!netSnmpManager->isRunning())
        return;
    mVariables.insert(QString("%1/%2").arg(sv.host).arg(sv.oidString()), sv);
    updateVariablesTable();
}

void ManagerWindow::onProgramStateChanged(const SnmpProgramState &sps)
{
    if (!netSnmpManager->isRunning())
        return;
    mProgramStates.insert(QString("%1/%2").arg(sps.host).arg(sps.oidString()), sps);
    updateProgramStatesTable();
}

void ManagerWindow::onTrapCame(const SnmpTrap &trap)
{
    log->append(QString("[%1] info - Trap %2 came from %3")
                .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                .arg(trap.name)
                .arg(trap.host));
}

void ManagerWindow::onError(const QString &text)
{
    log->append(QString("[%1] error - %2")
                .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                .arg(text));
    qDebug() << text;
}
