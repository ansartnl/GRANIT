#include "Ping.h"

#include "PingPrivate.h"

#include <QtCore/QThread>

Ping::Ping(QObject *parent) :
    QObject(parent), d(new PingPrivate), mPingThread(new QThread(this))
{
    d->moveToThread(mPingThread);

    connect(mPingThread, SIGNAL(started()), d.data(), SLOT(start()));
    connect(mPingThread, SIGNAL(finished()), d.data(), SLOT(stop()));
    connect(d.data(), SIGNAL(quit()), mPingThread, SLOT(quit()));
    connect(mPingThread, SIGNAL(started()), SIGNAL(started()));
    connect(mPingThread, SIGNAL(finished()), SIGNAL(finished()));

    connect(d.data(), SIGNAL(socketWarning(QString)), SIGNAL(warning(QString)));
    connect(d.data(), SIGNAL(socketError(QString)), SIGNAL(error(QString)));
    connect(d.data(), SIGNAL(pingSent(int)), SIGNAL(pingSent(int)));
    connect(d.data(), SIGNAL(pongCame()), SLOT(onPongCame()));
    connect(d.data(), SIGNAL(timeout()), SLOT(onTimeout()));
}

Ping::~Ping()
{
}

bool Ping::isRunning() const
{
    return mPingThread->isRunning();
}

void Ping::setPingData(const QByteArray &data)
{
    d->setPingData(data);
}

const QString Ping::host() const
{
    return d->host();
}

void Ping::start(const QString &host, int num)
{
    if (isRunning()) {
        stop();
        mPingThread->wait();
    }
    d->setHost(host);
    d->setNumber(num);
    mPingThread->start();
}

void Ping::stop()
{
    mPingThread->quit();
}

void Ping::onPongCame()
{
    emit pongCame();
    emit result(true);
}

void Ping::onTimeout()
{
    emit timeout();
    emit result(false);
}
