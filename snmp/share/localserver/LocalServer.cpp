#include "LocalServer.h"

#include "LocalSocket.h"

#ifdef Q_OS_UNIX
#include <QtCore/QDir>
#include <QtCore/QFile>
#endif

#include <QtNetwork/QLocalServer>

namespace local_connection
{

LocalServer::LocalServer(QObject *parent) :
    QObject(parent), mLocalServer(new QLocalServer(this))
{
    connect(mLocalServer, SIGNAL(newConnection()), SLOT(onNewConnection()));
}

void LocalServer::listen(const QString &name)
{
#ifdef Q_OS_UNIX
    QFile socketFile(QDir::temp().absoluteFilePath(name));
    if (socketFile.exists())
        socketFile.remove();
#endif

    mLocalServer->listen(name);
}

void LocalServer::stop()
{
    mLocalServer->close();
}

const QList < LocalSocket * > LocalServer::sockets() const
{
    return mSockets;
}

void LocalServer::sendAll(const QVariant &data)
{
    foreach (LocalSocket *socket, mSockets)
        socket->send(data);
}

void LocalServer::onNewConnection()
{
    while (mLocalServer->hasPendingConnections()) {
        LocalSocket *socket = new LocalSocket(mLocalServer->nextPendingConnection(), this);
        connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
        connect(socket, SIGNAL(error(QString)), SLOT(onError(QString)));
        connect(socket, SIGNAL(readyRead(QVariant)), SLOT(onReadyRead(QVariant)));
        mSockets << socket;
        emit newConnection(socket);
    }
}

void LocalServer::onDisconnected()
{
    LocalSocket *socket = qobject_cast < LocalSocket * >(sender());
    mSockets.removeAll(socket);
    emit disconnected(socket);
    socket->deleteLater();
}

void LocalServer::onError(const QString &text)
{
    LocalSocket *socket = qobject_cast < LocalSocket * >(sender());
    emit error(socket, text);
}

void LocalServer::onReadyRead(const QVariant &data)
{
    LocalSocket *socket = qobject_cast < LocalSocket * >(sender());
    emit readyRead(socket, data);
}

} // namespace local_connection
