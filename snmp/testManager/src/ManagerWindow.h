#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include "ui_ManagerWindow.h"

#include "SnmpVariable.h"

class ManagerWindow : public QMainWindow, private Ui::ManagerWindow
{
    Q_OBJECT
public:
    explicit ManagerWindow(QWidget *parent = 0);
    virtual ~ManagerWindow();

signals:
    void start();
    void stop();

    void startScript(const snmp::SnmpScript &script);
    void setVariable(const snmp::SnmpVariable &variable);

private:
    enum ScriptsTableColumn {
        STC_HOST = 0
        , STC_MIB
        , STC_NAME
        , STC_INDEX
        , STC_STATE
        // invisible
        , STC_STATE_INT
        , STC_KEY
    };

    enum VariablesTableColumn {
        VTC_HOST = 0
        , VTC_MIB
        , VTC_NAME
        , VTC_INDEX
        , VTC_VALUE
        , VTC_TYPE
        , VTC_WRITABLE
        // invisible
        , VTC_KEY
    };

    enum ProgramStatesTableColumn {
        PSTC_HOST = 0
        , PSTC_MIB
        , PSTC_NAME
        , PSTC_INDEX
        , PSTC_STATE
        // invisible
    };

    enum ConnectionStatesTableColumn {
        CSTC_HOST = 0
        , CSTC_STATE
        // invisible
    };

    void clearScriptsTable();
    void clearVariablesTable();
    void clearProgramStatesTable();
    void clearConnectionStatesTable();

    void updateScriptsTable();
    void updateVariablesTable();
    void updateProgramStatesTable();
    void updateConnectionStatesTable();

private slots:
    void onPushButton();
    void onRunButton();
    void onSetButton();

    void onStarted();
    void onStopped();

    void onConnected(const QString &host);
    void onLostConnection(const QString &host);

    void onScriptSelected(const QModelIndex &index);
    void onVariableSelected(const QModelIndex &index);

    void onScriptStateChanged(const snmp::SnmpScript &ss);
    void onVariableChanged(const snmp::SnmpVariable &sv);
    void onProgramStateChanged(const snmp::SnmpProgramState &sps);
    void onTrapCame(const snmp::SnmpTrap &trap);

    void onError(const QString &text);

private:
    QMap < QString, snmp::SnmpScript> mScripts;
    QString mSelectedScript;
    QMap < QString, snmp::SnmpVariable > mVariables;
    QString mSelectedVariable;
    QMap < QString, snmp::SnmpProgramState> mProgramStates;

    QMap < QString, bool > mConnectionStates;
};

#endif // MANAGERWINDOW_H
