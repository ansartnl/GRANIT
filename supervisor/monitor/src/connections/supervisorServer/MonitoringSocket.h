#ifndef _MONITORING_SOCKET_H__
#define _MONITORING_SOCKET_H__

#include <QtCore/QStringList>
#include <QtCore/QDateTime>

#include <QtGui/QColor>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include "SpProto.h"

#include "src/connections/IConnection.h"

class QtVariantConnection;
class QtSocketAutoConnector;
class QtConnectionControler;

struct MonitoredObject
{
    MonitoredObject()
        : isOn(true)
    {}

    enum Status { Started, Stopped };
    enum Type { CWP, FMTP, FDP, ADP, STCA, AFTNWS, DBSYNC, PROCESS, SERVICE, Server };

    Type        type;
    QString     name;
    QString     description;
    Status      status;
    QString     mVisualName;
    QString     additionalStatus;
    QStringList cwpSectors;
    QString     user;
    bool        isOn;                   //!< True when object status - monitored
    inline QString visualName() const
    { return (mVisualName.isEmpty()) ? (name) : (mVisualName); }
    //! True when service can be activated.
    inline bool canActivate() const
    { return type == FDP; }
};
QString toString(MonitoredObject::Type type);
QString toString(MonitoredObject::Status status);

struct Group
{
    QString     name;
    QString     visualName;
};


//! Managment of monitored host via network socket.
class MonitoringSocket : public QTcpSocket, public IConnection
{
    Q_OBJECT

public:
    explicit MonitoringSocket(const QString &id, const QString &name,
                              const QString &host, const QString &host2, quint16 port,
                              QObject *parent = 0);

    ~MonitoringSocket();
    //{@ Connection parameters
    const QString id() const { return mId; }   //! IConnection
    const QString& name() const { return mName; }
    const QString host() const { return mHost; }   //! IConnection
    const QString& host2() const { return mHost2; }
    quint16 port() const { return mPort; }  //! IConnection
    //@}

    //! Retruns true if socket is connected.
    bool isConnected() const;

    //! Returns true if status of all monitored objects is 'Started'.
    bool isValid() const;

    //! Retrurn true if all processes of this socket have 'Status On'
    bool isAllProcessesOn() const;

    //! Returns true if status of socket is on.
    bool isStatusOn() const { return mIsStatusOn; }
    //! Change current status.
    void changeStatus(bool isOn);

    //{@ Monitored object functions
    int countObjects() const;
    const MonitoredObject& object(int index) const;
    int findIndex(MonitoredObject::Type type, const QString &name) const;
    void clearObjects();
    //@}

    void clear();

    //{@ Groups functions
    int countGroups() const;
    const Group& group(int index) const;
    int findGroupIndex(const QString &name) const;
    void clearGroups();
    const QString& activeGroup() const { return mActiveGroup; }
    //@}

    //! Request of available monitored objects and their statuses.
    void requestObjects();

    //! Request of available networks.
    void requestNetworks();

    //! Request of available scripts.
    void requestScripts();

    //! Request status for specified service.
    void requestStatus(MonitoredObject::Type type, const QString &name);

    //! Request status for all services.
    void requestAllStatuses();

    //! Request log for specified service.
    void requestLog(MonitoredObject::Type type, const QString &name);

    //! Request Remote Operations for specified service.
    void requestRemoteOperations(const QString &name, const QDateTime &from, const QDateTime &to);

    //! Request Logs for all services
    void requestRemoteLogs(const QDateTime &from, const QDateTime &to);

    //! Restarts specified service.
    bool restart(MonitoredObject::Type type, const QString &name);

    //! Starts all services
    bool startAll();
    //! Stops all services
    bool stopAll();

    //! Request available groups and active group on the server.
    void requestGroups();

    //! Request active group name.
    void requestActiveGroup();

    //! Activate specified group of services.
    void activateGroup(const QString &name);

    //! Returns fdp service if server has one, 0 otherwise
    MonitoredObject * cwp();
    //! Returns fdp service if server has one, 0 otherwise
    MonitoredObject * fdp();
    //! Activates fdp.
    void activateFdp(const QString &name = QString());

    inline QStringList availableNets() const { return mAvailableNets; }
    inline QStringList availableScripts() const { return mScripts; }
    inline QStringList scriptOutput() const { return mScriptOutput; }

    //! Current object
    void setCurrentObject(int index);
    inline MonitoredObject *currentObject() const { return mCurrentObject; }

    //! Modified flag
    void setModify(bool modify);
    bool isModify() const;

public slots:   //! Script Called slots
    //! Shutdown computer
    bool shutdown();
    //! Reboot computer
    bool reboot();
    //! Toggle socket status (on/off)
    void toggleStatus();
    //! Request Logs for all services (will prompt for date range)
    void requestRemoteLogs();

    //! Starts specified script. Only one script can run at one time
    void startScript(const QString &name, const QString &parameter = QString());
    //! Stops running script
    void stopScript(const QString &name);

    //! Set SNMP variable
    void snmp(const QString &args);

signals:
    void newVariantMessage(const QVariant &);
    void netStatusChanged();
    void scriptListChanged();
    void scriptOutputChanged();

    void socketConnected();
    void socketDisconnected();

    void eventToLog(const QString &host, const QString &service
                    , const QString &message, QColor color = Qt::black);

    void remoteLogRecieved(const QString &sender, const SupervisorProto::RemoteLog &log);

private slots:
    void on_variantReceived(const QVariant &v);
    void on_connected();
    void on_disconnected();
    void on_error();
    void on_connectingAttempt(QHostAddress, quint16);

private:
    typedef QList<MonitoredObject>      TMonitoredObjectList;
    typedef QList<Group>                TGroupList;

    //! Find monitored object.
    MonitoredObject* findObject(MonitoredObject::Type type, const QString &name);
    QHostAddress lookupHost(const QString &host);

    /** Interaction variant socket wrapper */
    QtVariantConnection     *mConnection;
    QtSocketAutoConnector   *mConnector;
    QtConnectionControler   *mConnectionController;

    // Connection parameters
    QString             mId;
    QString             mName;
    QString             mHost;
    QString             mHost2;
    quint16             mPort;
    /** List of monitored objects */
    TMonitoredObjectList    mObjects;
    MonitoredObject *mCurrentObject;
    // Available networks
    QStringList mAvailableNets;
    // Available scripts
    QStringList mScripts;
    QStringList mScriptOutput;

    // True when host status - monitored
    bool        mIsStatusOn;

    TGroupList  mGroups;
    QString     mActiveGroup;
    bool        mModifyFlag;
};

#endif // _MONITORING_SOCKET_H__
