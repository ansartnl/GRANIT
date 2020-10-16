#ifndef _APPLICATION_CONTROLLER_H__
#define _APPLICATION_CONTROLLER_H__

#include "Controller.h"
#include "DbConnThread.h"

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QMap>
#include <QtCore/QMutex>

#include "SpProto.h"

class ClientConnection;
class DatabaseController;


//! Runs and controls specified services and applications.
class ApplicationController : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationController(DatabaseController *dbController, QObject *parent = 0);

    struct ServiceOption
    {
        QString     id;                 //!< Uniquie identifier
        Type        type;               //!< Controller type to create
        int         order;              //!< Order of start/stop operation
        QString     namePath;           //!< Binary path or service name
        QString     logPath;
        QString     arguments;          //!< Process command line arguments
        QString     configPath;
        QString     visualName;
        QString     description;
        QString     database;           //!< Database identifier
        bool        isRestartable;
        QString     otherOptions;       //!< Other specific options that coded as "key1=value1;key2=value2;..."
        int         killTimeout;
    };
    typedef QList<ServiceOption>            TServiceOptionList;
    typedef QSharedPointer<Controller>      TController;
    typedef QMap<QString, TController>      TControllerMap;
    typedef QList<SupervisorProto::RemoteLog>     TControllerLogs;

    //! Returns true if specified process or service this controller uses.
    bool containsService(const QString &id) const;

    void addServices(const TServiceOptionList &scList);
    void addProcesses(const TServiceOptionList &scList);

    //! Find controller by identifier.
    TController findController(const QString &id) const;

    //! Retrieve list of available services.
    QList<Controller::Info> serviceList() const;

    //! List of database identifiers for active group.
    QStringList activeDatabaseList() const;

    //! Read log file for specified service.
    QStringList readLog(const QString &id, int countLines = 300) const;

    //! Read log file for specified service from specified date.
    QStringList readLog(const QString &id, const QDateTime &from, const QDateTime &to, bool extractLines = true) const;

    //! Read log files from al controllers for specified service from specified date.
    TControllerLogs readLog(const QDateTime &from, const QDateTime &to) const;

    //! Check if service or application is running.
    /*!
        \param name Service name or path to application
    */
    bool isRunning(const QString &name);

    //! Do special connections for incoming TCP connection.
    void connectTo(ClientConnection *clientConn);

    SupervisorProto::RemoteOperations parseLog(QStringList &log);

signals:
    void started();
    void stopped();

public slots:
    //! Restarts process/service.
    void restart(const QString &id);
    //! Stops specified process/service.
    bool stop(const QString &id);

    //! Delayed start of all services and processes (after connection to database is found).
    void start();
    //! Stop all services and processes
    void stop();

private:
    TControllerMap  mControllers;
    mutable QMutex  mMutex;

    /** Database controller reference */
    DatabaseController  *mDbControllerRef;
};

#endif // _APPLICATION_CONTROLLER_H__
