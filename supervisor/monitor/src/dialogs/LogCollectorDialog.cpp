#include "LogCollectorDialog.h"
#include "ui_LogCollectorDialog.h"
//#include "MainWindow.h"

#include "src/connections/supervisorServer/MonitoringSocket.h"

#include <QMessageBox>

CLogCollectorDialog::CLogCollectorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLogCollectorDialog),
    mUseArchive(false),
    mSocket(0)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->dt_from->setDate(QDate::currentDate());
    ui->dt_to->setDate(QDate::currentDate());

    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui->btn_request, SIGNAL(clicked()), this, SLOT(onRequest()));
    connect(ui->cb_archive, SIGNAL(toggled(bool)), this, SLOT(onArchiveToggle(bool)));
}

CLogCollectorDialog::~CLogCollectorDialog()
{
    delete ui;
}

void CLogCollectorDialog::setSocket(MonitoringSocket *socket)
{
    mSocket = socket;
}

bool copyDir(const QString source, const QString destination, const bool override) {

    QDir directory(source);
    bool error = false;

    if (!directory.exists()) {
        return false;
    }

    if(!QDir(destination).exists())
        return false;

    QStringList dirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList files = directory.entryList(QDir::Files);

    QList<QString>::iterator d,f;

    for (d = dirs.begin(); d != dirs.end(); ++d) {

        if (!QFileInfo(directory.path() + "/" + (*d)).isDir()) {
            continue;
        }

        QDir temp(destination + "/" + (*d));
        temp.mkpath(temp.path());

        if (!copyDir(directory.path() + "/" + (*d), destination + "/" + (*d), override)) {
            error = true;
        }
    }

    for (f = files.begin(); f != files.end(); ++f) {
        QFile tempFile(directory.path() + "/" + (*f));

        if (QFileInfo(directory.path() + "/" + (*f)).isDir()) {
            continue;
        }

        QFile destFile(destination + "/" + directory.relativeFilePath(tempFile.fileName()));

        if (destFile.exists() && override) {
            destFile.remove();
        }

        if (!tempFile.copy(destination + "/" + directory.relativeFilePath(tempFile.fileName()))) {
            error = true;
        }
    }
    return !error;
}

void CLogCollectorDialog::onRemoteLogRecieved(const QString &sender, const SupervisorProto::RemoteLog &log)
{
    //create appropriate logfile
    //sender contains: <%host>/<%service>/
    //need to create <%log.service>.log
    if(mRequestedServices.contains( log.service ))
    {
        if(log.log.isEmpty())
        {
            //specified service returned no logfile
            mLogDetails.append(tr("Service '%1' (%2) did not send a logfile!").arg(sender).arg(log.service));
        }
        else
        {
            QString path = mLogDir.absolutePath() + "/" + sender;
            mLogDir.mkpath(path);

            QFile logfile(path + QString("%1.log").arg(log.service));
            logfile.open(QIODevice::WriteOnly);
            if(logfile.isOpen())
            {
                for (int i = log.log.count() - 1; i >= 0; --i)
                    logfile.write(log.log[i].toLatin1());
                logfile.close();
            }
        }
    }

    if(mRequestedServices.isEmpty())
        return;

    mMutex.lock();
    QString section = log.service;
//    QString section = sender.section(QChar('/'), -1, -1, QString::SectionSkipEmpty);
    mRequestedServices.removeOne(section);

    if(mRequestedServices.isEmpty())
    {
        if(mUseArchive)
        {
            QString dirs;   //contains all subdirectories, wrapped by "" and separated with spaces
            foreach(QString dir, mLogDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
                dirs.append(" \"" + dir + "\"");
#ifdef Q_OS_WIN
            QString arcCmd = QString("cd \"%1\" && zip -r \"%2\" %3")
                                    .arg(mLogDir.absolutePath())
                                    .arg(mLogDir.dirName()+".zip")
                                    .arg(dirs);
            if(0 == system( arcCmd.toAscii().data() )) //archieve logs
            {
                foreach(QString dir, mLogDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
                {
                    QString delCmd = QString("rd /S /Q \"%1\"")
                            .arg(mLogDir.absolutePath() + "/" + dir);
                    system( delCmd.toAscii().data());   //delete files itself
                }
            }
#else
            QString arcCmd = QString("cd \"%1\" && tar czf \"%2\" %3")
                                                .arg(mLogDir.absolutePath())
                                                .arg(mLogDir.dirName()+".tar.gz")
                                                .arg(dirs);
            if(0 == system( arcCmd.toLatin1().data() )) //archieve logs
            {
                foreach(QString dir, mLogDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
                {
                    QString delCmd = QString("rm -rf /S /Q \"%1\"")
                            .arg(mLogDir.absolutePath() + "/" + dir);
                    system( delCmd.toLatin1().data());   //delete files itself
                }
            }
#endif
        }
        //ended recieving
        QMessageBox* mb = new QMessageBox(QMessageBox::Information,
                       tr("Complete!"),
                       tr("Log recieving complete!"),
                       QMessageBox::Ok);
        mb->setDefaultButton(QMessageBox::Ok);
        if(QApplication::activeWindow())
        {
            QWidget *active = QApplication::activeWindow();
            QPoint offset(mb->width() / 2, mb->height() / 2);
            mb->move(active->contentsRect().center() + active->pos() - offset);
        }

        if(mLogDetails.isEmpty())
            mb->setDetailedText(tr("All log files recieved!"));
        else
            mb->setDetailedText(mLogDetails.join(QString("\n")));

        mb->setAttribute(Qt::WA_DeleteOnClose);
        connect(mb, SIGNAL(finished(int)), this, SLOT(close()));
        mb->show();

        hide();
        enableControls(true);
    }
    mMutex.unlock();
}

void CLogCollectorDialog::onCancel()
{
    close();
}

void CLogCollectorDialog::onArchiveToggle(bool toggled)
{
    mUseArchive = toggled;
}

void CLogCollectorDialog::onSaveToFlashDialogFinished(int result)
{
    if(result == QMessageBox::Ok && !mFlashPath.isEmpty())
    {
        mLogDir.mkpath(mFlashPath + "/" + mLogDir.dirName());
        if(!copyDir(mLogDir.absolutePath(), mFlashPath + "/" + mLogDir.dirName() , true))
        {
            QMessageBox::warning(this,
                               tr("Fail!"),
                               tr("Could not copy to flash drive!"),
                               QMessageBox::Ok);
        }

        QMessageBox* mb = new QMessageBox(QMessageBox::Information,
                       tr("Complete!"),
                       tr("Try another copy?"),
                       QMessageBox::Ok | QMessageBox::Cancel);
        mb->setDefaultButton(QMessageBox::Cancel);
        mb->setAttribute(Qt::WA_DeleteOnClose);
        mb->show();
        connect(mb, SIGNAL(finished(int)), this, SLOT(onSaveToFlashDialogFinished(int)));
    }
    else
    {
        close();
    }
}

void CLogCollectorDialog::enableControls(bool enable)
{
    ui->btn_request->setEnabled(enable);
    ui->btn_cancel->setEnabled(enable);
    ui->dt_from->setEnabled(enable);
    ui->dt_to->setEnabled(enable);
}

void CLogCollectorDialog::onRequest()
{
    enableControls(false);
    QDateTime from = ui->dt_from->dateTime();
    QDateTime to = ui->dt_to->dateTime();
    from.setTime(QTime(0,0));
    to.setTime(QTime(23,59));

    mRequestedServices.clear();
    if(mSocket)
    {
        for(int j = 0; j < mSocket->countObjects(); j++)
            mRequestedServices.append(mSocket->object(j).name);
        mSocket->requestRemoteLogs(from, to);
    }

    mLogDetails.clear();

    //create folder here
    mLogDir.setPath(QString("../log/%1/").arg(QDateTime::currentDateTimeUtc().toString(QString("yy_MM_dd hh.mm"))));
    mLogDir.mkpath(mLogDir.absolutePath());
}
