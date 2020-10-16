#include <QtCore/QMetaObject>
#include <QtCore/QSet>

#include "RedisBatapStorage.h"
#include "RedisBatapMessageStorage.h"

RedisBatapMessageStorage::RedisBatapMessageStorage(RedisBatapStorage *storage, const QByteArray &queue, QObject *parent):
    BatapMessageStorage(parent), mStorage(storage), mQueue(queue)
{
    connect(mStorage, SIGNAL(hasMessage(QByteArray)), SLOT(onHasMessage(QByteArray)));
    connect(mStorage, SIGNAL(requestedMessage(QByteArray, QByteArray, QByteArray)), SLOT(onRequestedMessage(QByteArray,QByteArray,QByteArray)));
}

void RedisBatapMessageStorage::removeMessage(QVariant id)
{
    QMetaObject::invokeMethod(mStorage, "removeMessage", Qt::QueuedConnection, Q_ARG(QByteArray, mQueue), Q_ARG(QByteArray, id.toByteArray()));
}

void RedisBatapMessageStorage::requestNextMessage(QList<QVariant> ids)
{
    QSet<QByteArray> keys;
    foreach (const QVariant &v, ids) {
        keys.insert(v.toByteArray());
    }

    QMetaObject::invokeMethod(mStorage, "requestNextMessage", Qt::QueuedConnection, Q_ARG(QByteArray, mQueue), Q_ARG(QSet<QByteArray>, keys));
}

void RedisBatapMessageStorage::onHasMessage(QByteArray queue)
{
    if (queue == mQueue) {
        emit hasMessage();
    }
}

void RedisBatapMessageStorage::onRequestedMessage(QByteArray queue, QByteArray key, QByteArray message)
{
    if (queue == mQueue) {
        emit requestedMessage(key, message);
    }
}
