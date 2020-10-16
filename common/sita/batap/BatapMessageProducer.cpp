#include <QtCore/QMetaObject>

#include "Batap.h"
#include "BatapMessageStorage.h"
#include "BatapMessageProducer.h"

BatapMessageProducer::BatapMessageProducer(Batap *batap, BatapMessageStorage *storage, QObject *parent):
    QObject(parent), mBatap(batap), mStorage(storage), mRunning(false)
{
    mBatap->setParent(this);
    mStorage->setParent(this);

    connect(mBatap, SIGNAL(readyForTransmission()), SLOT(start()));
    connect(mStorage, SIGNAL(hasMessage()), SLOT(start()));

    connect(mBatap, SIGNAL(messageDelivered(int)), SLOT(onMessageDelivered(int)));

    connect(mStorage, SIGNAL(requestedMessage(QVariant,QByteArray)), SLOT(onRequestedMessage(QVariant,QByteArray)));
}

void BatapMessageProducer::onRequestedMessage(QVariant id, QByteArray message)
{
    mRunning = false;

    if (id.isNull()) {
        return;
    }

    int batapId = mBatap->send(message);

    if (batapId != -1) {
        mIdMap.insert(batapId, id);
    }

    if (mBatap->isReadyForTransmission()) {
        requestNextMessage();
    }
}

void BatapMessageProducer::onMessageDelivered(int id)
{
    if (mIdMap.contains(id)) {
        QMetaObject::invokeMethod(mStorage, "removeMessage", Qt::QueuedConnection, Q_ARG(QVariant, mIdMap.take(id)));
    }
}

void BatapMessageProducer::start()
{
    if (!mRunning && mBatap->isReadyForTransmission()) {
        requestNextMessage();
    }
}

void BatapMessageProducer::requestNextMessage()
{
    mRunning = true;
    QMetaObject::invokeMethod(mStorage, "requestNextMessage", Qt::QueuedConnection, Q_ARG(QList<QVariant>, mIdMap.values()));
}
