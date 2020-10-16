#include "EventLogDialog.h"
#include "ui_EventLogDialog.h"

#include <QDir>
#include <QSettings>
#include <QCloseEvent>
#include <QTimer>

#include "printactionsmanager.h"
#include "Log.h"

#include "src/SQLiteManager.h"
#include "src/models/EventLogModel.h"
#include "src/models/EventLogFilterModel.h"

const QString EventLogDialog::DB_CONNECTION_NAME = "EventLogDialog";


EventLogDialog::EventLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventLogDialog)
{
    ui->setupUi(this);

    QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION_NAME);

    setAttribute(Qt::WA_DeleteOnClose);

    mModel = new EventLogModel(this);
    EventLogFilterModel* filterModel = new EventLogFilterModel(this);
    filterModel->setSourceModel(mModel);
    ui->logTable->setModel(filterModel);
    ui->logTable->horizontalHeader()->setStretchLastSection(true);

    QSettings settings;
    restoreGeometry( settings.value("EventLogDialogGeometry").toByteArray() );
    ui->logTable->horizontalHeader()->restoreState( settings.value("EventLogHeaderState").toByteArray() );

    ui->date->setMaximumWidth(ui->logTable->horizontalHeader()->sectionSize(0));
    ui->lineHost->setMaximumWidth(ui->logTable->horizontalHeader()->sectionSize(1));
    ui->lineService->setMaximumWidth(ui->logTable->horizontalHeader()->sectionSize(2));
    ui->lineEvent->setMaximumWidth(ui->logTable->horizontalHeader()->sectionSize(3));
    connect(ui->logTable->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), this, SLOT(onColumnResized(int,int,int)));

    PrintActionsManager *printActionsManager = new PrintActionsManager(this);
    connect(this, SIGNAL(print()), printActionsManager, SIGNAL(printPreview()));

    connect(ui->buttonPrint, SIGNAL(clicked()), this, SLOT(onPrint()));
    connect(ui->buttonResetFilter, SIGNAL(clicked()), this, SLOT(onClearFilter()));
    connect(ui->checkShowFailure, SIGNAL(toggled(bool)), this, SLOT(onFilterFailureToggled(bool)));

    connect(ui->date, SIGNAL(dateChanged(QDate)), SLOT(loadDatabase(QDate)));

    connect(ui->lineEvent, SIGNAL(textChanged(QString)), filterModel, SLOT(setEventFilter(QString)));
    connect(ui->lineHost, SIGNAL(textChanged(QString)), filterModel, SLOT(setHostFilter(QString)));
    connect(ui->lineService, SIGNAL(textChanged(QString)), filterModel, SLOT(setServiceFiler(QString)));
    connect(ui->checkShowFailure, SIGNAL(toggled(bool)), filterModel, SLOT(setFailureFilter(bool)));

    updateDateEdit();
    onClearFilter();
}

EventLogDialog::~EventLogDialog()
{
    mModel->clear();
    QSqlDatabase::removeDatabase(DB_CONNECTION_NAME);

    delete ui;
}

void EventLogDialog::enableActiveControl(bool enable)
{
    ui->buttonPrint->setEnabled(enable);
}

void EventLogDialog::refresh()
{
    if (ui->date->date() == QDate::currentDate())
    {
        mModel->select(QSqlDatabase::database(DB_CONNECTION_NAME, false));
    }
}

void EventLogDialog::closeEvent(QCloseEvent *event)
{
    QSettings regSettings;
    regSettings.setValue("EventLogDialogGeometry", saveGeometry());
    regSettings.setValue("EventLogHeaderState", ui->logTable->horizontalHeader()->saveState());

    QDialog::closeEvent(event);
}

void EventLogDialog::onFilterFailureToggled(bool checked)
{
    if(checked)
    {
        mEventFilterBuffer = ui->lineEvent->text();
        mHostFilterBuffer = ui->lineHost->text();
        mServiceFilterBuffer = ui->lineService->text();

        ui->lineEvent->clear();
        ui->lineHost->clear();
        ui->lineService->clear();
    }
    else
    {
        ui->lineEvent->setText(mEventFilterBuffer);
        ui->lineHost->setText(mHostFilterBuffer);
        ui->lineService->setText(mServiceFilterBuffer);
    }


    ui->lineEvent->setDisabled(checked);
    ui->lineHost->setDisabled(checked);
    ui->lineService->setDisabled(checked);
}

void EventLogDialog::onClearFilter()
{
    ui->checkShowFailure->setChecked(false);

    ui->date->setDate(QDate::currentDate());
    ui->lineEvent->clear();
    ui->lineHost->clear();
    ui->lineService->clear();
}

void EventLogDialog::onPrint()
{
    ui->logTable->setFocus();
    emit print();
}

void EventLogDialog::onColumnResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize)
    switch(logicalIndex)
    {
    case 0:
    {
        ui->date->setMaximumWidth(newSize);
        break;
    }
    case 1:
    {
        ui->lineHost->setMaximumWidth(newSize);
        break;
    }
    case 2:
    {
        ui->lineService->setMaximumWidth(newSize);
        break;
    }
    case 3:
    {
        ui->lineEvent->setMaximumWidth(newSize);
        break;
    }
    }
}

void EventLogDialog::updateDateEdit()
{
    QDate date = QDate::currentDate();
    ui->date->setDate(date);
    ui->date->setMaximumDate(date);

    QTimer::singleShot((86400 - qAbs(QTime::currentTime().secsTo(QTime(0,0,1)))) * 1000, this, SLOT(updateDateEdit()));
}

void EventLogDialog::loadDatabase(const QDate &date)
{
    QString databasePath = SQLiteManager::databasePath(date);

    QSqlDatabase db = QSqlDatabase::database(DB_CONNECTION_NAME, false);
    if (db.isOpen())
    {
        db.close();
    }

    db.setDatabaseName(databasePath);

    if (QFile::exists(databasePath))
    {
        if (!db.open())
        {
            qLogError() << "Failed to open event log database: " << db.lastError().text();
        }
    }

    mModel->select(db);
}
