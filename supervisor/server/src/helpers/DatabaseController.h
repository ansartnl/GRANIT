#ifndef _DATABASE_CONTROLLER_H__
#define _DATABASE_CONTROLLER_H__

#include "DbConnThread.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtCore/QMutex>


//! Run and store references to database connections to check if there is a connection to them.
class DatabaseController : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseController(QObject *parent = 0);

    //! Returns true if specified database is controlled.
    bool contains(const QString &id) const;

    //! Add parameters for controlling specified database.
    void addDatabase(const QString &id, const QString &driver, const QString &host, int port,
                     const QString &dbName, const QString &user, const QString &password);

    //! Finds database by identifier.
    DbConnThread* database(const QString &id) const;

    //! Returns true if empty.
    bool empty() const { return mDatabases.empty(); }

    //! True when all connections to database were created.
    bool isReady() const;

    //! Set list of monitored database identifiers.
    void setMonitoredDatabases(const QStringList &dbList);

signals:
    //! Emits this signal after all connections are ready for databases.
    void ready();

public slots:
    //! Start connection finder process.
    void start();
    //! Stop any connection search engine.
    void stop();

    //! Restart database connection service.
    /*! Usefull to change active group if new active group is set.
    */
    void restart();

private slots:
    //! Database ready slot.
    void onDatabaseReady();

private:
    //! Start next database connection checker thread.
    void startNext();

    // Start process/service order map (which order process/service should be started).
    // first - database identifier, second - database worker (connection checker)
    typedef QMap<QString, DbConnThread*>    TDatabaseMap;

    TDatabaseMap    mDatabases;
    mutable QMutex  mMutex;

    //! Connections that are monitored by controller.
    QSet<QString>   mMonitoredConnections;

    /** Connections to databases that were already created */
    QSet<QString>   mReadyConnections;
    /** Currently checking (active) connection */
    QString         mActiveConnection;
};

#endif // _DATABASE_CONTROLLER_H__
