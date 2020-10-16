#ifndef REQUEST_THREAD_H
#define REQUEST_THREAD_H

#include "common.h"

#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QStack>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtNetwork/QNetworkInterface>

class ApplicationController;
class DatabaseController;
class NetController;
class ScriptController;
class LogController;
class SnmpController;

typedef QList<QNetworkInterface> NetworkList;


//! Used to do handle requests in a separate thread.
class RequestThread : public QThread
{
    Q_OBJECT

public:
    RequestThread(QObject *parent = 0);

    //! Set references to controllers
    void setControllers(ApplicationController *appCtrl
                        , DatabaseController *dbCtrl
                        , NetController *netController
                        , ScriptController *scriptController
                        , LogController *logController
                        , SnmpController *snmpController);

    //! Stop the thread.
    void stop();
    bool isStopped() { return mIsStopped; }

public slots:
    //! Add new request for processing.
    void add(QVariant);

signals:
    void sendVariant(const QVariant &);

    //! Send message about status changes.
    void sendStatus(Type type, const QString &name, Status status);
    //! Send message about fmtp status changes.
    void sendFmtpStatus(const QString &name, EFMTPStatus status);
    //! Send message about cwp sectors changes.
    void sendCWPSectors(const QString &name, const QString &client, const QStringList &sectors);
    //! Send message about cwp user changes.
    void sendCWPUser(const QString &name, const QString &user);
    //! Send message about fdp status changes.
    void sendFdpStatus(const QString &name, bool status);
    //! Send message about adp status changes.
    void sendAdpStatus(const QString &name, const QString &status);

protected:
    void run();
    //! Process one request.
    void processRequest(QVariant);

    void setNetList(const NetworkList &list);

    //! Shutdown computer
    void shutdown();
    //! Reboot computer
    void reboot();

    //! Read server log
    QStringList log(int countLines = 300);

private:
    /*! Update data mutex */
    QMutex      mUpdateMutex;
    /*! Forever loop mutex */
    QMutex      mForeverMutex;
    /*! Airplane wait condition */
    QWaitCondition mWait;

    /*! Reference to application controller */
    ApplicationController   *mAppCtrl;
    /*! Reference to database controller */
    DatabaseController      *mDbCtrl;
    /** Reference to network controller object */
    NetController           *mNetController;
    /** Reference to script controller object */
    ScriptController        *mScriptController;
    /** Reference to log controller object */
    LogController           *mLogController;
    /** Reference to SNMP controller object */
    SnmpController          *mSnmpController;

    typedef QStack<QVariant>            TVariantStack;

    /*! Stack of waiting for processing requests */
    TVariantStack   mRequests;

    /*! Cancel state */
    volatile bool   mIsStopped;
};

#endif // REQUEST_THREAD_H
