#include "FDPControllers.h"

#include "Log.h"
#include "PulsReceiver.h"
#include "qtxmlsettings.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtNetwork/QNetworkInterface>


/////////////////////////////////////////////////////////////////////////////////
// FDPControllerHandler implementation.

FDPControllerHandler::FDPControllerHandler(QObject *parent)
    : QObject(parent), pulsReceiver(0), controllerRef(0)
{
    isMaster = false;
    myAddress = QHostAddress::LocalHost;

    controllerRef = qobject_cast<Controller *>(parent);
    connect(controllerRef, SIGNAL(statusChanged(Type,QString,Status)),
            SLOT(onStatusChanged(Type,QString,Status)));
}

void FDPControllerHandler::start()
{
    if ( pulsReceiver )
        return;

    pulsReceiver = new PulsReceiver(AFTNPulsarDefaultPort, this);
    connect(pulsReceiver, SIGNAL(pulsarChanged(QHostAddress,QString,QHostAddress,QVariantHash))
            , SLOT(onPulsarStatusChanged(QHostAddress)));
    connect(pulsReceiver, SIGNAL(status(QHostAddress,QString,QVariantHash))
            , SLOT(onStatus(QHostAddress)));
    pulsReceiver->start();
}

void FDPControllerHandler::activate()
{
    qLogInfo(tr("Controller")) << tr(" Activating FDP on %1").arg(myAddress.toString());
    pulsReceiver->activatePulsar(myAddress);
}

void FDPControllerHandler::onPulsarStatusChanged(const QHostAddress &pulsar)
{
    bool iAmMaster;
    if (QNetworkInterface::allAddresses().contains(pulsar))
        iAmMaster = true;
    else
        iAmMaster = false;
    if (iAmMaster != isMaster)
    {
        isMaster = iAmMaster;
        emit fdpStatusChanged(controllerRef->name(), isMaster);
    }
}

void FDPControllerHandler::onStatus(const QHostAddress &pulsar)
{
    if (QNetworkInterface::allAddresses().contains(pulsar))
        myAddress = pulsar;
}

void FDPControllerHandler::onStatusChanged(Type , const QString &, Status status)
{
    if (status == Stopped)
    {
        if (isMaster)
            emit fdpStatusChanged(controllerRef->name(), false);
        isMaster = false;
    }
}


/////////////////////////////////////////////////////////////////////////////////
// FDPProcessController implementation.

FDPProcessController::FDPProcessController()
    : mHandler(0)
{
    mHandler = new FDPControllerHandler(this);
    connect(mHandler, SIGNAL(fdpStatusChanged(const QString &, bool)),
            SIGNAL(fdpStatusChanged(const QString &, bool)));

    mHandler->start();
}

Type FDPProcessController::type() const
{
    return FDP;
}


/////////////////////////////////////////////////////////////////////////////////
// FDPServiceController implementation.

FDPServiceController::FDPServiceController()
    : mHandler(0)
{
    mHandler = new FDPControllerHandler(this);
    connect(mHandler, SIGNAL(fdpStatusChanged(const QString &, bool)),
            SIGNAL(fdpStatusChanged(const QString &, bool)));

    mHandler->start();
}

Type FDPServiceController::type() const
{
    return FDP;
}
