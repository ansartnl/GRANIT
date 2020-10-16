#ifndef DBCONNECTIONSMANAGER_H
#define DBCONNECTIONSMANAGER_H

#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtSql/QSqlDatabase>

class QTimer;

struct DbParams
{
    QString driver;
    QString host;
    int port;
    QString databaseName;
    QString user;
    QString password;

    DbParams() : port(0) {}

    DbParams(const QString &driver, const QString &host, const int port,
             const QString &name, const QString &user, const QString &password) :
        driver(driver), host(host), port(port), databaseName(name), user(user), password(password)
    {}
};

typedef QList<DbParams> DbParamsList;

//inline QDataStream &operator<<(QDataStream &out, const DbParams &params)
//{
//    out << params.driver
//        << params.host
//        << params.port
//        << params.databaseName
//        << params.user
//        << params.password;
//    return out;
//}

//QDataStream &operator>>(const QDataStream &in, DbParams &params)
//{
//    in >> params.driver
//        >> params.host
//        >> params.port
//        >> params.databaseName
//        >> params.user
//        >> params.password;
//    return in;
//}

class DBConnectionsWorker : public QObject
{
    Q_OBJECT
public:
    explicit DBConnectionsWorker(QObject *parent = 0);
    virtual ~DBConnectionsWorker();

    void addDbParams(const DbParams &p);

    void setDbAlias(const QString &alias);
    void setInterval(int msec);

    bool isConnected() const;

public slots:
    void start();
    void stop();

signals:
    void connected();
    void connected(int index);
    void connected(const DbParams &p);
    void connectFailed(const DbParams &p);
    void disconnected();
    void error(const QString &err);
    void finished();

protected:
    void reconnect();
    bool fillDb();
    void removeDb();

    void setActive(bool active);
    void setConnectTimeout(QSqlDatabase &db, int secs);

protected slots:
    void onTimer();
    void onConnected();

protected:
    QTimer *timer;

    DbParamsList mDbParams;
    int mCurrentIndex;

    QString mAlias;
    QSqlDatabase db;

    bool isActive;
    mutable QMutex mutex;

    bool isFirstReconnect;
};

/*!
  * \class DBConnectionsManager
  * \brief Автоматическое переподключение бд по списку
  *
  * Класс предназначен для автоматического подключения отключившейся базы данных
  * через заданный промежуток времени с заданными параметрами, с периодической проверкой альтернативных БД
  */
class DBConnectionsManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBConnectionsManager)

    DBConnectionsManager();
    ~DBConnectionsManager();

public:
    static DBConnectionsManager &instance();

    void addDbParams(const DbParams &p);

    void setDbAlias(const QString &alias);
    void setInterval(int msec);

    bool isConnected();

public slots:
    void start();
    void start(const QString &dbAlias);
    void stop();
    void quit();

signals:
    void connected();
    void connected(int index);
    void connected(const DbParams &p);
    void connectFailed(const DbParams &p);
    void disconnected();
    void error(const QString &err);

private slots:
    void onConnected(const DbParams &p);

private:
    QThread *mThread;
    DBConnectionsWorker *mWorker;
};

#endif // DBCONNECTIONSMANAGER_H
