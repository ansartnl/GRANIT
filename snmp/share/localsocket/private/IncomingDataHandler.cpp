#include "IncomingDataHandler.h"

#include <QtCore/QDataStream>
#include <QtCore/QVariant>

namespace local_connection
{

IncomingDataHandler::IncomingDataHandler(QObject *parent) :
    QObject(parent)
{
}

void IncomingDataHandler::append(const QByteArray &ba)
{
    pAppend(ba);
    tryProcess();
}

void IncomingDataHandler::pAppend(const QByteArray &ba)
{
    QMutexLocker locker(&mutex);
    mBuffer.append(ba);
}

void IncomingDataHandler::tryProcess()
{
    staticMetaObject.invokeMethod(this, "process", Qt::QueuedConnection);
}

size_t IncomingDataHandler::bufferSize() const
{
    QMutexLocker locker(&mutex);
    size_t ret = mBuffer.size();
    return ret;
}

qint64 IncomingDataHandler::packageSize() const
{
    QMutexLocker locker(&mutex);

    qint64 ret = 0;
    QDataStream stream(mBuffer);
    stream >> ret;

    return ret;
}

const QByteArray IncomingDataHandler::takeLeft(qint64 size)
{
    QMutexLocker locker(&mutex);

    mBuffer.remove(0, sizeof(size));

    const QByteArray ret = mBuffer.left(size);
    mBuffer.remove(0, size);
    return ret;
}

void IncomingDataHandler::process()
{
    if (bufferSize() < sizeof(qint64))
        return;

    qint64 size = packageSize();

    if (bufferSize() < size + sizeof(qint64))
        return;

    QByteArray ba = takeLeft(size);

    QVariant ret;
    QDataStream streams(ba);
    streams >> ret;

    emit readyRead(ret);

    tryProcess();
}

} // namespace local_connection
