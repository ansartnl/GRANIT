#include "LocalSocket.h"

#include "private/IncomingDataHandler.h"

#include <QtCore/QTimer>

#include <QtNetwork/QLocalSocket>

namespace local_connection
{

LocalSocket::LocalSocket(QObject *parent) :
    QObject(parent), mSocket(new QLocalSocket(this)), mDataHandler(new IncomingDataHandler(this))
  , mFutureState(FS_DISCONNECTED)
{
    init();
}

LocalSocket::LocalSocket(QLocalSocket *socket, QObject *parent) :
    QObject(parent), mSocket(socket), mDataHandler(new IncomingDataHandler(this))
  , mFutureState(FS_DEFAULT)
{
    mSocket->setParent(this);

    init();
}

bool LocalSocket::isConnected() const
{
    return (mSocket->state() == QLocalSocket::ConnectedState);
}

void LocalSocket::connectToServer(const QString &name)
{
    mFutureState = FS_CONNECTED;
    if (!name.isEmpty())
        mServerName = name;
    mSocket->connectToServer(mServerName);
}

void LocalSocket::disconnectFromServer()
{
    mFutureState = FS_DISCONNECTED;
    mServerName.clear();
    mSocket->disconnectFromServer();
}

void LocalSocket::send(const QVariant &data)
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << data;
    const qint64 size = ba.size();

    QByteArray ba2;
    QDataStream stream2(&ba2, QIODevice::WriteOnly);
    stream2 << size;
    stream2.writeRawData(ba.data(), ba.size());

    mSocket->write(ba2);
}

void LocalSocket::initSocket()
{
    connect(mSocket, SIGNAL(connected()), SIGNAL(connected()));
    connect(mSocket, SIGNAL(disconnected()), SIGNAL(disconnected()));
    connect(mSocket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), SLOT(onStateChanged()));
    connect(mSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), SLOT(onError()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void LocalSocket::initDataHandler()
{
    connect(mDataHandler, SIGNAL(readyRead(QVariant)), SIGNAL(readyRead(QVariant)));
}

void LocalSocket::onStateChanged()
{
    if ((mSocket->state() == QLocalSocket::UnconnectedState) && (mFutureState == FS_CONNECTED))
        QTimer::singleShot(100, this, SLOT(connectToServer()));
}

void LocalSocket::onError()
{
    if (mSocket->error() != QLocalSocket::PeerClosedError)  // это не ошибка
        emit error(mSocket->errorString());
}

void LocalSocket::onReadyRead()
{
    mDataHandler->append(mSocket->readAll());
}

void LocalSocket::init()
{
    initSocket();
    initDataHandler();
}

} // namespace local_connection
