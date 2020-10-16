#include "LogDialog.h"
#include "ui_LogDialog.h"

#include "printactionsmanager.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    connect(ui->printButton, SIGNAL(clicked()), SLOT(onPrint()));
    connect(ui->filter, SIGNAL(textChanged(QString)), SLOT(showLog()));

    PrintActionsManager *printActionsManager = new PrintActionsManager(this);
    connect(this, SIGNAL(print()), printActionsManager, SIGNAL(printPreview()));
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::setLogText(const QStringList &log)
{
    mLog = log;
    showLog();
}

void LogDialog::setTitle(const QString &title)
{
    mTitle = title;
    if (mTitle.isEmpty())
        setWindowTitle(tr("Self log information"));
    else
        setWindowTitle(tr("%1 log information").arg(mTitle));
}

void LogDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        if (mTitle.isEmpty())
            setWindowTitle(tr("Self log information"));
        else
            setWindowTitle(tr("%1 log information").arg(mTitle));
    }

    QDialog::changeEvent(event);
}

QStringList LogDialog::filter(const QStringList &log)
{
    QString f = ui->filter->text();
    if (f.isEmpty())
        return log;
    QStringList ret;
    foreach (const QString s, log)
    {
        if (s.contains(f))
            ret << s;
    }
    return ret;
}

void LogDialog::onPrint()
{
    ui->plainTextEdit->setFocus();
    emit print();
}

void LogDialog::showLog()
{
    ui->plainTextEdit->setPlainText(filter(mLog).join("\n"));
}
