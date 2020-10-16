#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "Matip.h"
#include "Batap.h"
#include "BatapMessageProducer.h"

#include "SitaConnection.h"

SitaConnection::SitaConnection(const SitaConfig &config, QTcpSocket *socket, QObject *parent) :
    QObject(parent), mSocket(socket), mMatip(0), mBatap(0), mBatapMessageProducer(0), mConfig(config)
{
    mMatip = new Matip(mConfig.matipConfig, mSocket, this);

    connect(mSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));

    connect(mMatip, SIGNAL(sessionRequested(quint16,quint16)), SIGNAL(sessionRequested(quint16,quint16)));
    connect(mMatip, SIGNAL(sessionClosed()), SLOT(onSessionClosed()));
    connect(mMatip, SIGNAL(sessionTimeout()), SLOT(onSessionClosed()));

    mName = QString("%1:%2").arg(mSocket->peerAddress().toString()).arg(mSocket->peerPort());
    mMatip->setObjectName(name());
}

void SitaConnection::acceptSession(BatapMessageStorage *storage)
{
    mMatip->acceptConnection();
    mBatap = new Batap(mConfig.batapConfig, mMatip);
    connect(mBatap, SIGNAL(newMessage(QByteArray)), SIGNAL(newMessage(QByteArray)));

    mBatapMessageProducer = new BatapMessageProducer(mBatap, storage, this);
}

void SitaConnection::refuseSession(Matip::RefuseCause cause)
{
    mMatip->refuseConnection(cause);

    mSocket->disconnectFromHost();
}

void SitaConnection::close()
{
    stop();

    if (mMatip->state() == Matip::DataTransferState || mMatip->state() == Matip::IdleState) {
        mMatip->closeSession();
        emit sessionClosed();
    }

    mSocket->disconnectFromHost();
}

void SitaConnection::start()
{
    if (mBatap) {
        mBatap->start();
    }
}

void SitaConnection::stop()
{
    if (mBatap) {
        mBatap->stop();
    }
}

void SitaConnection::onSessionClosed()
{
    stop();

    emit sessionClosed();

    mSocket->disconnectFromHost();
}

void SitaConnection::onSocketDisconnected()
{
    stop();

    if (mMatip->state() == Matip::DataTransferState || mMatip->state() == Matip::IdleState) {
        emit sessionClosed();
    }

    emit closed();
}
