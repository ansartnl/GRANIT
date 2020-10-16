#ifndef REDISBATAPMESSAGESTORAGE_H
#define REDISBATAPMESSAGESTORAGE_H

#include "BatapMessageStorage.h"

class RedisBatapStorage;

//  Wrapper around real redis storage
class RedisBatapMessageStorage : public BatapMessageStorage
{
    Q_OBJECT
public:
    RedisBatapMessageStorage(RedisBatapStorage *storage, const QByteArray &queue, QObject *parent = 0);

public slots:
    void removeMessage(QVariant id);
    void requestNextMessage(QList<QVariant> ids);

private slots:
    void onHasMessage(QByteArray queue);
    void onRequestedMessage(QByteArray queue, QByteArray key, QByteArray message);

private:
    RedisBatapStorage *mStorage;
    QByteArray mQueue;
};

#endif // REDISBATAPMESSAGESTORAGE_H
