#include "DbConnThread.h"

#include "common.h"

#include "Log.h"

#include <QtCore/QMutexLocker>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

// Thread locker. Lets run one thread at a time.
static QMutex g_mutex;


DbConnThread::DbConnThread(QObject *parent)
    : QThread(parent), mIsStopped(false), mRunMode(RunDefault), mPort(0)
{
}

DbConnThread* DbConnThread::clone(QObject *parent) const
{
    DbConnThread *dbConn = new DbConnThread(parent);

    dbConn->mDbConnectionName = mDbConnectionName;
    dbConn->mDriver = mDriver;
    dbConn->mHost = mHost;
    dbConn->mDbName = mDbName;
    dbConn->mUser = mUser;
    dbConn->mPassword = mPassword;
    dbConn->mPort = mPort;
    dbConn->mUserData = mUserData;

    return dbConn;
}

void DbConnThread::setDatabaseParams(const QString &driver, const QString &host, int port,
                                     const QString &dbName, const QString &user, const QString &password)
{
    mDriver = driver;
    mHost = host;
    mDbName = dbName;
    mUser = user;
    mPassword = password;
    mPort = port;

    setObjectName(QString("%1:%2").arg(host).arg(port));
}

QString DbConnThread::databaseConnectionName()
{
    return mDbConnectionName.isEmpty() ? getConnectionDbName() : mDbConnectionName;
}

void DbConnThread::waitConnection()
{
    // Wait until other operation is finished.
    if ( isRunning() )
        wait();

    mRunMode = RunWaitConnection;
    QThread::start();
}

void DbConnThread::start()
{
    // Wait until other operation is finished.
    if ( isRunning() )
        wait();

    mRunMode = RunDefault;
    QThread::start();
}

void DbConnThread::run()
{
    // Does not let to run other thread until this is executed.
    QMutexLocker locker(&g_mutex);

    mIsStopped = false;

    if ( mHost.isEmpty() || mUser.isEmpty() || mDbName.isEmpty() )
    {
        qLogError() << tr(" Database is not configured. Failed to start");
        return;
    }

    if ( RunDefault == mRunMode )
        runDefault();
    else
        runWaitConnection();
}

QSqlDatabase DbConnThread::createDatabase()
{
    QString dbConnectionName = databaseConnectionName();

    QSqlDatabase db;
    if ( !QSqlDatabase::contains( dbConnectionName ) )
    {
        db = QSqlDatabase::addDatabase(mDriver, dbConnectionName);
        db.setHostName(mHost);
        db.setDatabaseName(mDbName);
        db.setUserName(mUser);
        db.setPassword(mPassword);
        db.setPort(mPort);
    }
    return db;
}

bool DbConnThread::testConnection(QSqlDatabase &db)
{
    // Do workaround for this - making transaction to check if there is a connection
    // to database.
    if ( db.transaction() )
    {
        db.rollback();
        return true;
    }
    else
    {
        if ( mIsStopped )
            return true;

        if ( db.open() )
            return true;
    }

    qLogWarn(tr("Database")) << tr(" Database is not available. Error: %1")
                                .arg(db.lastError().text());

    if ( mIsStopped )
        return true;

    // Sleep 1 sec and try to connect one more time.
    // do not need here very complex logic for sleaping.
    sleep(1);

    if ( mIsStopped )
        return true;

    return false;
}

void DbConnThread::runDefault()
{
    QString connectionParams = QString("'%1:%2' '%3', '%4'").arg(mHost).arg(mPort).arg(mDbName).arg(mUser);

    qLogInfo(tr("Database")) << tr(" Connecting to database: %1").arg(connectionParams);

    // Try to connect to database
    {
        QString dbConnectionName = databaseConnectionName();

        QSqlDatabase db;
        if ( QSqlDatabase::contains( dbConnectionName ) )
            db = QSqlDatabase::database( dbConnectionName );
        else
            db = createDatabase();

        forever
        {
            if ( testConnection(db) )
                break;
        }
    }
//    QSqlDatabase::removeDatabase( g_connectionDbName );

    if ( !mIsStopped )
    {
        qLogInfo(tr("Database")) << tr(" Connected to database %1 successfully").arg(connectionParams);
        emit connectionReady();
    }
    else
        qLogWarn(tr("Database")) << tr(" Connection to database %1 failed. Process was stopped by the user").arg(connectionParams);
}

void DbConnThread::runWaitConnection()
{
    // Try to see if there is a connection to database ...

    QSqlDatabase db = QSqlDatabase::database( databaseConnectionName() );
    // If connection was not created yet - must be created.
    if ( !db.isValid() )
        db = createDatabase();

    forever
    {
        if ( testConnection(db) )
            break;
    }

    if ( !mIsStopped )
        emit connectionReady();
}
