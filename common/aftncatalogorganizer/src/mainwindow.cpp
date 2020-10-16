#include "mainwindow.h"
#include "ui_mainwindow.h"

QSettings * xmlSettings(const QString &pathConfig)
{
    static QtXmlSettings *xmlStg = new QtXmlSettings(pathConfig);
    return xmlStg;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    organizer.SetCheck(true);

    ui->label_inc->setText(organizer.GetIncomingPath());
    ui->label_out->setText(organizer.GetOutgoingPath());
    ui->label_today->setText(organizer.GetTodaysPath());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    //  input/output folders
    mIncPath = xmlSettings()->value("incoming","").toString();
    mOutPath = xmlSettings()->value("outgoing","").toString();

    organizer.SetPaths(mIncPath, mOutPath);
}
