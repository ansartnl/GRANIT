#ifndef PINGPRIVATE_H
#define PINGPRIVATE_H

#include <QtCore/QMutex>
#include <QtCore/QObject>

class IcmpSocket;

class QTimer;

class PingPrivate : public QObject
{
    Q_OBJECT
public:
    explicit PingPrivate(QObject *parent = 0);

    void setPingData(const QByteArray &data);
    const QByteArray data() const;

    const QString host() const;
    int number() const;

    void setSendTime(int timeMs);
    void setWaitTime(int timeMs);

public slots:
    void start();
    void stop();

    void setHost(const QString &host);
    void setNumber(int num);

signals:
    void quit();

    void socketWarning(const QString &text);
    void socketError(const QString &text);
    void pingSent(int number);
    void pongCame();
    void timeout();

private slots:
    void onSendTimer();
    void onPongCame();

private:
    mutable QMutex mutex;

    QByteArray mData;
    QString mHost;
    int mNumberToSend;

    int mSentCount;

    QTimer *mSendTimer;
    QTimer *mWaitTimer;
    IcmpSocket *mSocket;
};

#endif // PINGPRIVATE_H
