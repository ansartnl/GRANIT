#include <QtCore/QThread>
#include <QtCore/QUuid>
#include <QtCore/QMetaObject>
#include <QtCore/QSet>

#include "RedisConnection.h"

#include "RedisBatapStorage.h"

RedisBatapStorage::RedisBatapStorage(RedisConnection *redisConnection, QObject *parent):
    QObject(parent), mRedisConnection(redisConnection)
{
    qRegisterMetaType<QSet<QByteArray>>("QSet<QByteArray>");
}

void RedisBatapStorage::addMessage(QByteArray queue, QByteArray message)
{
    if (thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "addMessage", Qt::QueuedConnection, Q_ARG(QByteArray, queue), Q_ARG(QByteArray, message));

        return;
    }

    try {
        QByteArray key = QUuid::createUuid().toByteArray();

        command({"MULTI"});
        command({"SET", key, message});
        command({"RPUSH", queue, key});
        command({"EXEC"});

        emit hasMessage(queue);
    }
    catch (QString e) {
        emit error(queue, e);
    }
}

void RedisBatapStorage::removeMessage(QByteArray queue, QByteArray key)
{
    if (thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "removeMessage", Qt::QueuedConnection, Q_ARG(QByteArray, queue), Q_ARG(QByteArray, key));

        return;
    }

    try {
        command({"MULTI"});
        command({"LREM", queue, "1", key});
        command({"DEL", key});
        command({"EXEC"});
    }
    catch (QString e) {
        emit error(queue, e);
    }
}

void RedisBatapStorage::requestNextMessage(QByteArray queue, QSet<QByteArray> keys)
{
    if (thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "requestNextMessage", Qt::QueuedConnection, Q_ARG(QByteArray, queue), Q_ARG(QSet<QByteArray>, keys));

        return;
    }

    try {
        QVariant response = command({"LRANGE", queue, "0", "-1"});

        foreach (QVariant v, response.toList()) {

            QByteArray key = v.toByteArray();
            if (!keys.contains(key)) {
                response = command({"GET", key});

                if (response.isValid()) {
                    QByteArray m = response.toByteArray();

                    emit requestedMessage(queue, key, m);
                    return;
                }
                else {
                    command({"LREM", queue, "1", key});
                }
            }
        }

        emit requestedMessage(queue, QByteArray(), QByteArray());
    }
    catch (QString e) {
        emit error(queue, e);
    }
}

QVariant RedisBatapStorage::command(QList<QByteArray> cmd)
{
    RedisConnection::Response response = mRedisConnection->command(cmd);
    if (response.error) {
        throw response.errorMessage;
    }

    return response.value;
}

