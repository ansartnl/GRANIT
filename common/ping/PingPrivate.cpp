#include "PingPrivate.h"

#include "IcmpSocket.h"

#include <QtCore/QTimer>

#include <QtDebug>

PingPrivate::PingPrivate(QObject *parent) :
    QObject(parent)
  , mNumberToSend(-1)
  , mSentCount(0)
  , mSendTimer(new QTimer(this))
  , mWaitTimer(new QTimer(this))
  , mSocket(new IcmpSocket(this))
{
    mSendTimer->setInterval(1000);
    mWaitTimer->setInterval(3000);

    connect(mSendTimer, SIGNAL(timeout()), SLOT(onSendTimer()));
    connect(mWaitTimer, SIGNAL(timeout()), SIGNAL(timeout()));

    connect(mSocket, SIGNAL(warning(QString)), SIGNAL(socketWarning(QString)));
    connect(mSocket, SIGNAL(error(QString)), SIGNAL(socketError(QString)));
    connect(mSocket, SIGNAL(pongCame()), SLOT(onPongCame()));
}

void PingPrivate::setPingData(const QByteArray &data)
{
    QMutexLocker locker(&mutex);
    mData = data.mid(0, MAX_PING_DATA_SIZE);
}

const QByteArray PingPrivate::data() const
{
    QMutexLocker locker(&mutex);
    return mData;
}

const QString PingPrivate::host() const
{
    QMutexLocker locker(&mutex);
    return mHost;
}

int PingPrivate::number() const
{
    QMutexLocker locker(&mutex);
    return mNumberToSend;
}

void PingPrivate::setSendTime(int timeMs)
{
    mSendTimer->setInterval(timeMs);
}

void PingPrivate::setWaitTime(int timeMs)
{
    mWaitTimer->setInterval(timeMs);
}

void PingPrivate::start()
{
    mSentCount = 0;
    if (!mSocket->open(host())) {
        emit socketError(mSocket->errorString());
        emit quit();
        return;
    }
    mSendTimer->start();
    mWaitTimer->start();
    onSendTimer();
}

void PingPrivate::stop()
{
    mWaitTimer->stop();
    mSendTimer->stop();
    mSocket->close();
}

void PingPrivate::setHost(const QString &host)
{
    QMutexLocker locker(&mutex);
    mHost = host;
}

void PingPrivate::setNumber(int num)
{
    QMutexLocker locker(&mutex);
    mNumberToSend = num;
}

void PingPrivate::onSendTimer()
{
    if (mNumberToSend > 0 && mSentCount >= mNumberToSend) {
        stop();
        emit quit();
        return;
    }
    if (mSocket->send(data())) {
        ++mSentCount;
        emit pingSent(mSentCount);
    }
}

void PingPrivate::onPongCame()
{
    mWaitTimer->stop();
    mWaitTimer->start();

    emit pongCame();
}
