#ifndef _CLIENT_CONNECTION_H__
#define _CLIENT_CONNECTION_H__

#include <QtNetwork/QTcpSocket>
#include <QtCore/QProcess>

#include "common.h"
#include "qtvariantconnection.h"

#include "FolderController.h"

class NetController;
class QtConnectionPing;
class RequestThread;
class ScriptController;
class SupervisorService;
class SnmpController;

//! Managment of monitored host via network socket.
class ClientConnection : public QObject
{
    Q_OBJECT

public:
    explicit ClientConnection(SupervisorService *supervisor,
                              QTcpSocket *socket,
                              QObject *parent = 0);
    ~ClientConnection();

public slots:
    //! Send message about status changes.
    void setStatus(Type type, const QString &name, Status status);
    //! Send message about fmtp status changes.
    void setFmtpStatus(const QString &name, EFMTPStatus status);
    //! Send message about cwp sector changes.
    void setCWPSectors(const QString &name, const QString &client, const QStringList &sectors);
    //! Send message about cwp sector changes.
    void setCWPUser(const QString &name, const QString &user);
    //! Send message about fdp status changes.
    void setFdpStatus(const QString &name, bool status);
    //! Send message about adp status changes.
    void setAdpStatus(const QString &name, const QString &status);

private slots:
    void on_variantReceived(const QVariant &v);
    void on_commandReceived(QVariant);
    void on_disconnected();
    void onNetChanged();
    void onScriptOutputString(const QString &str);
    void onScriptFinished(int, QProcess::ExitStatus);
    void onFolderItemsRecieved(const QList<FolderController::FileItem>& items);
    void onSnmpVariablesReceived(QString agent, QVariantMap variables);

private:
    /** Interaction variant socket wrapper */
    QtVariantConnection *mConnection;
    /** Used to ping client connection */
    QtConnectionPing    *mConnectionPing;
    /** Reference to supervisor service object */
    SupervisorService   *mSupervisor;
    /** Reference to network controller object */
    NetController       *mNetController;
    /** Reference to script controller object */
    ScriptController    *mScriptController;
    /** Reference to folder controller object */
    FolderController    *mFolderController;
    /** Reference to SNMP controller object */
    SnmpController      *mSnmpController;
    /** Thread to process requests */
    RequestThread       *mRequests;
};

#endif // _CLIENT_CONNECTION_H__
