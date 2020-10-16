#include "ClientWindow.h"

#include "TestNetSnmpDataSource.h"

#include "NetSnmpClient.h"

#include <QtCore/QDateTime>

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    TestNetSnmpDataSource *dataSource = new TestNetSnmpDataSource;
    connect(dataSource, SIGNAL(text(QString)), SLOT(onText(QString)));
    connect(dataSource, SIGNAL(info(QString)), SLOT(onInfo(QString)));
    connect(dataSource, SIGNAL(error(QString)), SLOT(onError(QString)));

    netSnmpClient->setDataSource(dataSource);
}

void ClientWindow::writeLog(const QString &level, const QString &text)
{

    log->insertPlainText(QString("[%1] %2 - %3\n")
                         .arg(QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz"))
                         .arg(level)
                         .arg(text));
}

void ClientWindow::onText(const QString &text)
{
    writeLog("text", text);
}

void ClientWindow::onInfo(const QString &text)
{
    writeLog("info", text);
}

void ClientWindow::onError(const QString &text)
{
    writeLog("error", text);
}
