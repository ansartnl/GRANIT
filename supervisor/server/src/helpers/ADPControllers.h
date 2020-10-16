#ifndef _ADP_CONTROLLERS_H__
#define _ADP_CONTROLLERS_H__

#include <QtCore/QtGlobal>

#include "ProcessController.h"
#include "ServiceController.h"
#include "common.h"

class QTimer;

class ADPProcessController;
class ADPServiceController;


//! ADP controller handler.
class ADPControllerHandler : public QObject
{
    Q_OBJECT
    friend class ADPProcessController;
    friend class ADPServiceController;

    // Available statuses
/*
    enum Status
    {
        Master,
        Slave,
        BecomingMaster,
        BecomingSlave,
        Undefined,
        Offline,
        BAD
    };
*/

public:
    //! ADP current status.
    inline QString status() const { return mStatus; }

    //! Start monitoring the folder.
    void start(const QString &path);

    //! Update controller's arguments and status.
    void update();

signals:
    void statusChanged(const QString &name, const QString &status);

protected slots:
    void onWatchTimeout();

protected:
    explicit ADPControllerHandler(QObject *parent=0);

    /** Reference to controller */
    Controller  *mControllerRef;
    /** Monitoring timer */
    QTimer      *mWatchFolderTimer;
    /** Path to monitoring folder */
    QString     mMonitoringPath;
    /** Current status */
    QString     mStatus;
    /** Original controller's arguments */
    QString     mArgumentsController;
};


//! ADP concrete-class process controller.
class ADPProcessController : public ProcessController
{
    Q_OBJECT
public:
    //! ctor
    ADPProcessController();

    //! Type associated with this controller.
    virtual Type type() const;
    //! Current ADP status
    inline QString adpStatus() const { return mHandler->status(); }

    //! Start Adp watcher.
    /*!
     * \param params Encoded specific key-value pairs
    */
    void startWatcher(const QString &params);

signals:
    void adpStatusChanged(const QString &name, const QString &status);

protected slots:
    //! Called when connection to database ready or immediatelly after process is started.
    virtual void on_databaseReady();

protected:
    ADPControllerHandler *mHandler;
};


//! ADP concrete-class service controller.
class ADPServiceController : public ServiceController
{
    Q_OBJECT
public:
    //! ctor
    ADPServiceController();

    //! Type associated with this controller.
    virtual Type type() const;
    //! Current ADP status
    inline QString adpStatus() const { return mHandler->status(); }

    //! Start Adp watcher.
    /*!
     * \param params Encoded specific key-value pairs
    */
    void startWatcher(const QString &params);

signals:
    void adpStatusChanged(const QString &name, const QString &status);

protected slots:
    //! Called when connection to database ready or immediatelly after process is started.
    virtual void on_databaseReady();

protected:
    ADPControllerHandler *mHandler;
};

#endif // _ADP_CONTROLLERS_H__
