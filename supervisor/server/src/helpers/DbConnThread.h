#ifndef _DB_CONN_THREAD_H__
#define _DB_CONN_THREAD_H__

#include <QtCore/QThread>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtSql/QSqlDatabase>


//! Database connection search engine.
/*!
    Once connection is found - thread finishes.
*/
class DbConnThread : public QThread
{
    Q_OBJECT
public:
    explicit DbConnThread(QObject *parent = 0);

    //! Make a prototype of this object.
    DbConnThread* clone(QObject *parent = 0) const;

    //! Schedule stop the thread.
    void stop() { mIsStopped = true; }
    bool isStopped() { return mIsStopped; }

    //! User data setter and getter.
    void setUserData(const QVariant &data) { mUserData = data; }
    const QVariant& userData() const { return mUserData; }

    void setDatabaseParams(const QString &driver, const QString &host, int port,
                           const QString &dbName, const QString &user, const QString &password);

    //! Current database connection name.
    void setDatabaseConnectionName(const QString &name) { mDbConnectionName = name; }
    QString databaseConnectionName();

public slots:
    //! Start thread in order to wait conection with the database.
    /*! Database must be created first using QThread::start command.
     *  \note This function is asynchronous
    */
    void waitConnection();

    void start();

signals:
    void connectionReady();

protected:
    virtual void run();

    void runDefault();
    void runWaitConnection();

    //! Creates database object and returns it.
    QSqlDatabase createDatabase();
    //! True when user stopped thread operation or connection is valid.
    bool testConnection(QSqlDatabase &db);

private:
    enum RunMode
    {
        RunDefault,
        RunWaitConnection
    };

    /** Cancel state */
    bool        mIsStopped;
    /** Database connection name */
    QString     mDbConnectionName;
    /** Run mode */
    RunMode     mRunMode;
    // Database parameters.
    QString     mDriver, mHost, mDbName, mUser, mPassword;
    int         mPort;
    /** User data */
    QVariant    mUserData;
};

#endif // _DB_READ_THREAD_H__
