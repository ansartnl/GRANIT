#ifndef _SERVICE_CONTROLLER_H__
#define _SERVICE_CONTROLLER_H__

#include "Controller.h"
#include "qtservice.h"

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>


//! Controller for monitoring on available service/daemon.
class ServiceController : public Controller
{
    Q_OBJECT

public:
    ServiceController();

    //! Associate service with the controller.
    void setService(const QString &name);

    //! Service name.
    QString serviceName() const;

    //! Retrieve current controller info.
    virtual Info information() const;

    //! Type associated with service controller.
    virtual Type type() const;

    //! Retrieve current status.
    virtual Status status() const;

    //! Check if service is running.
    virtual bool isRunning() const;

public slots:
    //! Starts service.
    virtual bool start();
    //! Restarts service.
    virtual void restart();
    //! Stops this service.
    virtual bool stop();

protected:
    //! Returns path to log file according to the controller.
    virtual QString logFilePath() const;

protected slots:
    virtual void on_databaseReady();

private slots:
    void on_spy_timeout();

protected:
    //! Start service.
    bool startService();

    typedef QScopedPointer<QtServiceController>     TQtServiceController;

    TQtServiceController    mService;
    QTimer                  *mServicesSpy;
};

#endif // _SERVICE_CONTROLLER_H__
