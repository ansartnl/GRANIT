#include "DatabaseController.h"

#include "Log.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QUuid>


DatabaseController::DatabaseController(QObject *parent)
    : QObject(parent)
{
}

void DatabaseController::addDatabase(const QString &id, const QString &driver, const QString &host, int port,
                                     const QString &dbName, const QString &user, const QString &password)
{
    // Database identifier must be unique.
    if ( id.isEmpty() || mDatabases.contains(id) )
        return;

    DbConnThread *dbConn = new DbConnThread(this);
    connect(dbConn, SIGNAL(connectionReady()), SLOT(onDatabaseReady()), Qt::QueuedConnection);

    dbConn->setUserData(id);
    dbConn->setDatabaseParams(driver, host, port, dbName, user, password);
    dbConn->setDatabaseConnectionName(QString("%1-%2").arg(id).arg(QUuid::createUuid().toString()));

    mDatabases[id] = dbConn;
}

bool DatabaseController::contains(const QString &id) const
{
    return mDatabases.contains(id);
}

DbConnThread* DatabaseController::database(const QString &id) const
{
    if ( mDatabases.contains(id) )
        return mDatabases[id];
    return 0;
}

bool DatabaseController::isReady() const
{
    // Think that there is no hack in this class.
    return mReadyConnections.size() == mDatabases.size();
}

void DatabaseController::setMonitoredDatabases(const QStringList &dbList)
{
    mMonitoredConnections.clear();

    // Create existing database connections.
    foreach ( const QString &id, dbList )
    {
        if ( mDatabases.contains(id) )
            mMonitoredConnections << id;
    }
}

void DatabaseController::restart()
{
    stop();
    start();
}

void DatabaseController::stop()
{
    QMutexLocker locker(&mMutex);

    for ( TDatabaseMap::iterator it = mDatabases.begin(); it != mDatabases.end(); ++it )
    {
        DbConnThread *dbConn = static_cast<DbConnThread *>(*it);
        dbConn->stop();
        dbConn->wait();
    }

    mActiveConnection = QString();
    mReadyConnections.clear();
}

void DatabaseController::start()
{
    QMutexLocker locker(&mMutex);

    // Test that nobody started process before and it is not running now.
    if ( (!mReadyConnections.empty() && mReadyConnections.size() < mMonitoredConnections.size())
         || !mActiveConnection.isEmpty() )
        return;

    mReadyConnections.clear();

    startNext();
}

void DatabaseController::startNext()
{
    if ( mMonitoredConnections.empty() )
    {
        mActiveConnection = QString();
        emit ready();
    }

    if ( !mActiveConnection.isEmpty() )
        mReadyConnections << mActiveConnection;

    QStringList connectionsToGo = mMonitoredConnections.toList();
    // Leave only connections that needs to be started.
    foreach ( const QString &conn, mReadyConnections )
        connectionsToGo.removeAll(conn);

    if ( connectionsToGo.empty() )
    {
        mActiveConnection = QString();
        emit ready();
    }
    else
    {
        mActiveConnection = connectionsToGo.front();

        DbConnThread *dbConn = mDatabases[mActiveConnection];
        dbConn->start();
    }
}

void DatabaseController::onDatabaseReady()
{
    startNext();
}
