#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

class QLocalServer;

namespace local_connection
{

class LocalSocket;

class LocalServer : public QObject
{
    Q_OBJECT
public:
    explicit LocalServer(QObject *parent = 0);

    void listen(const QString &name);
    void stop();

    const QList < LocalSocket * > sockets() const;

    template < typename T >
    void sendAll(const T &data)
    {
        sendAll(QVariant::fromValue(data));
    }

    void sendAll(const QVariant &data);


signals:
    void newConnection(local_connection::LocalSocket *socket);
    void disconnected(local_connection::LocalSocket *socket);
    void error(local_connection::LocalSocket *socket, const QString &text);
    void readyRead(local_connection::LocalSocket *socket, const QVariant &data);

private slots:
    void onNewConnection();

    void onDisconnected();
    void onError(const QString &text);
    void onReadyRead(const QVariant &data);

private:
    QLocalServer *mLocalServer;

    QList < LocalSocket * > mSockets;
};

} // namespace local_connection

#endif // LOCALSERVER_H
