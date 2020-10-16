#include <QtCore/QMetaType>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtCore/QUuid>
#include <QtSql/QSqlError>

#include "dbconnectionsmanager.h"

int type = qRegisterMetaType<DbParams>("DbParams");

DBConnectionsWorker::DBConnectionsWorker(QObject *parent) :
    mCurrentIndex(-1), isActive(false), isFirstReconnect(true)
{
    setObjectName("DBConnectionsManager");

    timer = new QTimer(parent);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()), SLOT(onTimer()));

    connect(this, SIGNAL(connected()), SLOT(onConnected()));
}

DBConnectionsWorker::~DBConnectionsWorker()
{
    timer->stop();
    removeDb();
}

void DBConnectionsWorker::addDbParams(const DbParams &p)
{
    mDbParams.append(p);
    if (mCurrentIndex == -1)
        mCurrentIndex = 0;
}

void DBConnectionsWorker::setDbAlias(const QString &alias)
{
    mAlias = alias;
}

void DBConnectionsWorker::setInterval(int msec)
{
    timer->setInterval(msec);
    setConnectTimeout(db, msec / 1000);
}

bool DBConnectionsWorker::isConnected() const
{
    QMutexLocker locker(&mutex);
    return isActive;
}

void DBConnectionsWorker::start()
{
    if (mAlias.isEmpty())
        mAlias = QLatin1String(QUuid::createUuid().toByteArray());

    if (!fillDb())
        return;

    timer->start();
    onTimer();
}

void DBConnectionsWorker::reconnect()
{
    if (isConnected())
    {
        setActive(false);
        emit disconnected();
    }
    if (!timer->isActive())
        start();
}

void DBConnectionsWorker::stop()
{
    timer->stop();
    setActive(false);
    emit finished();
}

bool DBConnectionsWorker::fillDb()
{
    removeDb();

    if (mDbParams.isEmpty())
    {
        emit error(tr("No db params"));
        return false;
    }

    if ((mCurrentIndex < 0) || (mCurrentIndex >= mDbParams.count()))
        mCurrentIndex = 0;

    DbParams p = mDbParams[mCurrentIndex];

    db = QSqlDatabase::addDatabase(p.driver, mAlias);
    db.setHostName(p.host);
    db.setPort(p.port);
    db.setDatabaseName(p.databaseName);
    db.setUserName(p.user);
    db.setPassword(p.password);
    setConnectTimeout(db, timer->interval() / 1000);

    return true;
}

void DBConnectionsWorker::removeDb()
{
    if (db.isValid())
    {
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(mAlias);
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

void DBConnectionsWorker::setActive(bool active)
{
    QMutexLocker locker(&mutex);
    isActive = active;
}

void DBConnectionsWorker::onTimer()
{
    if (db.open())
    {
        db.close();
        if (!isConnected())
        {
            setActive(true);
            emit connected();
        }

        isFirstReconnect = true;
    } else
    {
        emit error(db.lastError().text());
        emit connectFailed(mDbParams[mCurrentIndex]);

        if (isConnected())
            reconnect();

        if (isFirstReconnect)
        {
            isFirstReconnect = false;
        }
        else
        {
            ++mCurrentIndex;
            isFirstReconnect = true;
            fillDb();
        }
    }

}

void DBConnectionsWorker::onConnected()
{
    emit connected(mCurrentIndex);
    emit connected(mDbParams[mCurrentIndex]);
}

void DBConnectionsWorker::setConnectTimeout(QSqlDatabase &db, int secs)
{
    if (!db.isValid())
        return;

    if (db.driverName() == "QPSQL") {
        db.setConnectOptions("connect_timeout=" + QString::number(secs));
    }
}

DBConnectionsManager::DBConnectionsManager()
{
    mThread = new QThread();
    mWorker = new DBConnectionsWorker();
}

DBConnectionsManager::~DBConnectionsManager()
{
}

DBConnectionsManager &DBConnectionsManager::instance()
{
    static DBConnectionsManager object;
    return object;
}

void DBConnectionsManager::addDbParams(const DbParams &p)
{
    mWorker->addDbParams(p);
}

void DBConnectionsManager::setDbAlias(const QString &alias)
{
    mWorker->setDbAlias(alias);
}

void DBConnectionsManager::setInterval(int msec)
{
    mWorker->setInterval(msec);
}

bool DBConnectionsManager::isConnected()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen())
        return false;

    QProcess process;
    process.start("ping", QStringList() << "-c" << "1" << "-W" << "1" << db.hostName());
    process.waitForFinished();
    return (process.exitCode() == 0);
}

void DBConnectionsManager::start()
{
    mWorker->moveToThread(mThread);

    connect(mThread, SIGNAL (started()), mWorker, SLOT (start()));
    connect(mWorker, SIGNAL (finished()), mThread, SLOT (quit()));
    connect(mWorker, SIGNAL (finished()), mWorker, SLOT (deleteLater()));
    connect(mThread, SIGNAL (finished()), mThread, SLOT (deleteLater()));

    connect(mWorker, SIGNAL(connected(DbParams)), SLOT(onConnected(DbParams)));

    connect(mWorker, SIGNAL(connected()), SIGNAL(connected()));
    connect(mWorker, SIGNAL(connected(int)), SIGNAL(connected(int)));
    connect(mWorker, SIGNAL(connected(DbParams)), SIGNAL(connected(DbParams)));
    connect(mWorker, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(mWorker, SIGNAL(error(QString)), SIGNAL(error(QString)));

    mThread->start();
}

void DBConnectionsManager::start(const QString &dbAlias)
{
    setDbAlias(dbAlias);
    start();
}

void DBConnectionsManager::stop()
{
    if (mThread->isRunning()) {
        QMetaObject::invokeMethod(mWorker, "stop", Qt::QueuedConnection);
        mThread->wait(5000);
    }
    else {
        delete mWorker;
        delete mThread;
    }

    mThread = new QThread();
    mWorker = new DBConnectionsWorker();
}

void DBConnectionsManager::quit()
{
    if (mThread->isRunning()) {
        QMetaObject::invokeMethod(mWorker, "stop", Qt::QueuedConnection);
        mThread->wait(5000);
    }
    else {
        delete mWorker;
        delete mThread;
    }

    mWorker = 0;
    mThread = 0;
}

void DBConnectionsManager::onConnected(const DbParams &p)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(p.driver, QSqlDatabase::defaultConnection);
    db.setHostName(p.host);
    db.setPort(p.port);
    db.setDatabaseName(p.databaseName);
    db.setUserName(p.user);
    db.setPassword(p.password);
    db.open();
}
