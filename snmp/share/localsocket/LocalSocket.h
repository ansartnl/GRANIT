#ifndef LOCALSOCKET_H
#define LOCALSOCKET_H

#include <QtCore/QObject>
#include <QtCore/QVariant>

class QLocalSocket;

namespace local_connection
{

class IncomingDataHandler;

class LocalSocket : public QObject
{
    Q_OBJECT
public:
    explicit LocalSocket(QObject *parent = 0);
    explicit LocalSocket(QLocalSocket *socket, QObject *parent = 0);

    bool isConnected() const;

    template < typename T >
    void send(const T &data)
    {
        send(QVariant::fromValue(data));
    }

public slots:
    void connectToServer(const QString &name = QString());
    void disconnectFromServer();

    void send(const QVariant &data);

signals:
    void connected();
    void disconnected();
    void readyRead(const QVariant &data);

    void error(const QString &text);

private:
    void init();
    void initSocket();
    void initDataHandler();

private slots:
    void onStateChanged();
    void onError();
    void onReadyRead();

private:
    QLocalSocket *mSocket;
    IncomingDataHandler *mDataHandler;

    enum FutureState {
        FS_DEFAULT
        , FS_CONNECTED
        , FS_DISCONNECTED
    } mFutureState;
    QString mServerName;
};

} // namespace local_connection

#endif // LOCALSOCKET_H
