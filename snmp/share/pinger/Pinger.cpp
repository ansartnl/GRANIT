#include "Pinger.h"

#include <QtCore/QTimer>

Pinger::Pinger(QObject *parent) :
    QObject(parent)
  , mTimer(new QTimer(this))
  , mMaximumLosses(kDefaultMaximumLosses)
  , mRemoveOnTimeout(kDefaultRemoveOnTimeout)
{
    connect(mTimer, SIGNAL(timeout()), SLOT(onTimer()));

    setTimerInterval(kDefaultTimerInterval);
}

void Pinger::setTimerInterval(int msecs)
{
    mTimer->setInterval(msecs);
}

void Pinger::setMaximumLosses(int count)
{
    mMaximumLosses = count;
}

void Pinger::setRemoveOnTimeout(bool remove)
{
    mRemoveOnTimeout = remove;
}

void Pinger::addItem(const QString &key)
{
    updateItem(key, 0);

    if (!mItems.isEmpty() && !mTimer->isActive())
        mTimer->start();
}

void Pinger::removeItem(const QString &key)
{
    mItems.remove(key);

    if (mItems.isEmpty() && mTimer->isActive())
        mTimer->stop();
}

void Pinger::pingCame(const QString &key)
{
    updateItem(key, 0);
}

void Pinger::updateItem(const QString &key, int count)
{
    if (!key.isEmpty())
        mItems.insert(key, count);
}

void Pinger::onTimer()
{
    const QList < QString > keys = mItems.keys();
    foreach (const QString &key, keys) {
        emit sendPing(key);
    }
    foreach (const QString &key, keys) {
        const int val = mItems[key];
        updateItem(key, val + 1);
    }
    foreach (const QString &key, keys) {
        const int val = mItems[key];
        if (val > mMaximumLosses) {
            emit timeout(key);
            if (mRemoveOnTimeout)
                removeItem(key);
        }
    }
}
