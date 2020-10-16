#ifndef PING_H
#define PING_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class PingPrivate;

class Ping : public QObject
{
    Q_OBJECT
public:
    explicit Ping(QObject *parent = 0);
    virtual ~Ping();

    bool isRunning() const;

    //! Data, sent in Echo-Request message
    //! Returned in Echo-Reply message
    //! Should be at most \a MAX_PING_DATA_SIZE characters.
    void setPingData(const QByteArray &data);

    enum {
        INFINITE = -1
    };

    const QString host() const;

public slots:
    /*!
     * Start to send ICMP Echo-Request messages.
     * \param host - destination host
     * \param num - number of messages to send
     */
    void start(const QString &host, int num = Ping::INFINITE);
    void stop();

signals:
    void started();
    void finished();

    void warning(const QString &text);
    void error(const QString &text);
    void pingSent(int number);
    void pongCame();
    void timeout();
    void result(bool pongCame);

private slots:
    void onPongCame();
    void onTimeout();

private:
    QScopedPointer < PingPrivate > d;
    QThread *mPingThread;
};

#endif // PING_H
