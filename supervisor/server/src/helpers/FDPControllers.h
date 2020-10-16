#ifndef _FDP_CONTROLLERS_H__
#define _FDP_CONTROLLERS_H__

#include <QtCore/QtGlobal>

#include "ProcessController.h"
#include "ServiceController.h"
#include "common.h"

#include <QtNetwork/QHostAddress>

class PulsReceiver;
class QHostAddress;
class FDPProcessController;
class FDPServiceController;


//! FDP controller handler.
class FDPControllerHandler : public QObject
{
    Q_OBJECT
    friend class FDPProcessController;
    friend class FDPServiceController;
public:
    //! FDP Status: true if master, false if slave
    inline bool fdpStatus() const { return isMaster; }

    //! Activate this fdp, i.e. make it master
    void activate();

    //! Start pulsar.
    void start();

signals:
    void fdpStatusChanged(const QString &name, bool status);

protected slots:
    void onPulsarStatusChanged(const QHostAddress &pulsar);
    void onStatus(const QHostAddress &pulsar);
    void onStatusChanged(Type , const QString &, Status status);

protected:
    explicit FDPControllerHandler(QObject *parent=0);

    PulsReceiver *pulsReceiver;
    bool isMaster;
    QHostAddress myAddress;
    Controller *controllerRef;
};


//! FDP concrete-class process controller.
class FDPProcessController : public ProcessController
{
    Q_OBJECT
public:
    FDPProcessController();

    //! Type associated with this controller.
    virtual Type type() const;
    //! FDP Status: true if master, false if slave
    inline bool fdpStatus() const { return mHandler->fdpStatus(); }

public slots:
    //! Activate this fdp, i.e. make it master
    void activate() { mHandler->activate(); }

signals:
    void fdpStatusChanged(const QString &name, bool status);

protected:
    FDPControllerHandler *mHandler;
};


//! FDP concrete-class service controller.
class FDPServiceController : public ServiceController
{
    Q_OBJECT
public:
    FDPServiceController();

    //! Type associated with this controller.
    virtual Type type() const;
    //! FDP Status: true if master, false if slave
    inline bool fdpStatus() const { return mHandler->fdpStatus(); }

public slots:
    //! Activate this fdp, i.e. make it master
    void activate() { mHandler->activate(); }

signals:
    void fdpStatusChanged(const QString &name, bool status);

protected:
    FDPControllerHandler *mHandler;
};

#endif // _FDP_CONTROLLERS_H__
