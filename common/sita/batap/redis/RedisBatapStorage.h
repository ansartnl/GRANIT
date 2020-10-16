#ifndef REDISBATAPSTORAGE_H
#define REDISBATAPSTORAGE_H

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class RedisConnection;

class RedisBatapStorage : public QObject
{
    Q_OBJECT
public:
    RedisBatapStorage(RedisConnection *redisConnection, QObject *parent = 0);

public slots:
    void addMessage(QByteArray queue, QByteArray message);
    void removeMessage(QByteArray queue, QByteArray key);
    void requestNextMessage(QByteArray queue, QSet<QByteArray> keys);

signals:
    void hasMessage(QByteArray queue);
    void requestedMessage(QByteArray queue, QByteArray key, QByteArray message);
    void error(QByteArray queue, QString message);

protected:
    QVariant command(QList<QByteArray> cmd);

private:
    RedisConnection *mRedisConnection;
};

#endif // REDISBATAPSTORAGE_H
