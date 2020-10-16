#ifndef ICMPSOCKET_H
#define ICMPSOCKET_H

#include <QtCore/QObject>

class QTimer;

/*!
 * \class IcmpSocket
 * \brief Сокет для ICMP Echo-Request/Echo-Reply сообщений
 *
 * Открытие \a open открывает сокет и запускает приём сообщений по таймеру.
 * Закрытие \a close останавливает приём сообщений и закрывает сокет.
 *
 * Отправка запросов и таймаут сообщений контролируются вызывающим.
 *
 * Сигнал \a error вызывается только при чтении. В остальных случаях следует проверять возвращаемое функцией
 * значение на \a false и вызывать \a lastError или \a errorString .
 */

class IcmpSocket : public QObject
{
    Q_OBJECT
public:
    explicit IcmpSocket(QObject *parent = 0);

    bool open(const QString &address);
    bool close();

    bool send(const QByteArray &msg);

    enum IcmpSocketError {
        ISE_NO_ERROR = -1
        , ISE_UNKNOWN_ERROR = 0

        , ISE_ERROR_CREATING_SOCKET = 1
        , ISE_ERROR_CLOSING_SOCKET = 2
        , ISE_SOCKET_CLOSED = 3
        , ISE_SEND_ERROR = 4
        , ISE_SELECT_ERROR = 5
        , ISE_RECEIVE_ERROR = 6
        , ISE_RECEIVED_PACKET_ERROR = 7
    };

    IcmpSocketError lastError() const;
    const QString errorString() const;

signals:
    void warning(const QString &text);
    void error(const QString &text);
    void pongCame();

private:
    bool receive();

private slots:
    void onReceiveTimer();

private:
    IcmpSocketError mError;
    QString mAddress;

    QTimer *mReceiveTimer;

    int icmp_sock;
    int num_sent;

    const int ident;
};

#endif // ICMPSOCKET_H
