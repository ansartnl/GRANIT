#include "FMTPControllers.h"

#include "InitMain.h"
#include "Log.h"
#include "qtxmlsettings.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>


/////////////////////////////////////////////////////////////////////////////////
// FMTPServiceController implementation.

FMTPControllerHandler::FMTPControllerHandler(Controller *ctrl, const QString &dbConnectionName)
    : mFmtpStatus(enDisable), mDbConnectionName(dbConnectionName), mController(ctrl)
{
    Q_ASSERT(mController);
    mLocalDbConnectionName = QString("%1-%2").arg(mDbConnectionName).arg(ctrl->name());
}

EFMTPStatus FMTPControllerHandler::fmtpStatus() const
{
    return mFmtpStatus;
}

void FMTPControllerHandler::setConfigName(const QString &name)
{
    Q_UNUSED(name)
    QString configName = mController->configPath();
    if ( !QFile::exists(configName) )
    {
        qLogWarn("Controller") << " Could not find FMTP config";
        return;
    }

    QRegExp reg("(ATCAddress=\")(.+)(\")");
    reg.setMinimal(true);
    QFile config(configName);
    config.open(QFile::ReadOnly);
    QString configString(config.readAll());
    config.close();
    if (reg.indexIn(configString) == -1)
    {
        qLogWarn("Controller") << " Could not find 'ATCAddress' in FMTP config";
        return;
    }
    mStatusGiver = reg.cap(2);
    qLogInfo("Controller") << QString(" FMTP status giver: %1").arg(mStatusGiver);
}

bool FMTPControllerHandler::cloneDatabase()
{
    if ( QSqlDatabase::contains(mLocalDbConnectionName) )
        QSqlDatabase::removeDatabase(mLocalDbConnectionName);

    if ( !QSqlDatabase::contains(mDbConnectionName) )
        return false;

    QSqlDatabase db = QSqlDatabase::cloneDatabase( QSqlDatabase::database(mDbConnectionName),
                                                   mLocalDbConnectionName );

    return db.open();
}

QList<EFMTPStatus> FMTPControllerHandler::statusCodes() const
{
    QList<EFMTPStatus> statuses;

    QSqlDatabase db = QSqlDatabase::database(mLocalDbConnectionName);
    QSqlQuery query(db);
    QString queryString = QString("SELECT STATUSCODE FROM CURRENTSTATUS WHERE STATUSGIVER = '%1'")
            .arg(mStatusGiver);
    query.prepare(queryString);
    if (query.exec() && query.next())
    {
        statuses << EFMTPStatus(query.value(0).toInt());
    }

    return statuses;
}


/////////////////////////////////////////////////////////////////////////////////
// FMTPProcessController implementation.

FMTPProcessController::FMTPProcessController(const QString &dbConnectionName)
    : FMTPControllerHandler(this, dbConnectionName), mTimerId(0)
{
    connect(this, SIGNAL(statusChanged(Type,QString,Status))
            , this, SLOT(onStatusChanged(Type,QString,Status)));
}

Type FMTPProcessController::type() const
{
    return FMTP;
}

void FMTPProcessController::onStatusChanged(Type, const QString &name, Status status)
{
    if (status == Started)
    {
        if ( !cloneDatabase() )
            qLogWarn("Controller") << tr("FMTP process '%1' can not create database connection").arg(name);
        else
        {
            if ( mTimerId )
                killTimer(mTimerId);
            mTimerId = startTimer(1000);
        }
    } else
    {
        killTimer(mTimerId);
        mTimerId = 0;
        if ( QSqlDatabase::contains(mLocalDbConnectionName) )
            QSqlDatabase::removeDatabase(mLocalDbConnectionName);
    }
}

void FMTPProcessController::timerEvent(QTimerEvent *)
{
    foreach ( EFMTPStatus status, statusCodes() )
    {
        if (mFmtpStatus != status)
        {
            mFmtpStatus = status;
            emit fmtpStatusChanged(name(), mFmtpStatus);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////////
// FMTPServiceController implementation.

FMTPServiceController::FMTPServiceController(const QString &dbConnectionName)
    : FMTPControllerHandler(this, dbConnectionName), mTimerId(0)
{
    connect(this, SIGNAL(statusChanged(Type,QString,Status))
            , this, SLOT(onStatusChanged(Type,QString,Status)));
}

Type FMTPServiceController::type() const
{
    return FMTP;
}

void FMTPServiceController::onStatusChanged(Type, const QString &name, Status status)
{
    if (status == Started)
    {
        if ( !cloneDatabase() )
            qLogWarn("Controller") << tr("FMTP service '%1' can not create database connection").arg(name);
        else
        {
            if ( mTimerId )
                killTimer(mTimerId);
            mTimerId = startTimer(1000);
        }
    } else
    {
        killTimer(mTimerId);
        mTimerId = 0;
        if ( QSqlDatabase::contains(mLocalDbConnectionName) )
            QSqlDatabase::removeDatabase(mLocalDbConnectionName);
    }
}

void FMTPServiceController::timerEvent(QTimerEvent *)
{
    foreach ( EFMTPStatus status, statusCodes() )
    {
        if (mFmtpStatus != status)
        {
            mFmtpStatus = status;
            emit fmtpStatusChanged(name(), mFmtpStatus);
        }
    }
}
