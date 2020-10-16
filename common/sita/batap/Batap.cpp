
#include <QtCore/QTimer>
#include <QtCore/QTimerEvent>

#include "Matip.h"
#include "Batap.h"

Batap::Batap(const BatapConfig &config, Matip *matip, QObject *parent)
    : QObject(parent), mMatip(matip),
      mIsStopped(true), mIsTransmissionStopped(false),
      mCurrentSerialNumber(0), mConfig(config)
{
    mMatip->setParent(this);

    mImaTimer = new QTimer(this);
    mImaTimer->start(config.groupImaTime);
    connect(mImaTimer, SIGNAL(timeout()), SLOT(sendImaMessage()));

    connect(mMatip, SIGNAL(newMessage(QByteArray)), SLOT(onNewMessageFromMatip()));
    connect(mMatip, SIGNAL(stopTransmission()), SLOT(onStopTransmission()));
    connect(mMatip, SIGNAL(resumeTransmission()), SLOT(onResumeTransmission()));
}

Batap::~Batap()
{
}

void Batap::start()
{
    mIsStopped = false;

    if (isReadyForTransmission()) {
        emit readyForTransmission();
    }
}

void Batap::stop()
{
    mIsStopped = true;
}

bool Batap::isReadyForTransmission() const
{
    return !mIsStopped &&
            !mIsTransmissionStopped &&
            mPendingMessages.size() < mConfig.transmissionWindow;
}

int Batap::send(const QByteArray &message)
{
    if (!isReadyForTransmission()) {
        return -1;
    }

    if (message.isEmpty()) {
        return -1;
    }

    if (++mCurrentSerialNumber > 999) {
        mCurrentSerialNumber = 0;
    }

    sendMessage(message, mCurrentSerialNumber, false);

    return mCurrentSerialNumber;
}

void Batap::timerEvent(QTimerEvent *event)
{
    if (mIsTransmissionStopped)
        return;

    killTimer(event->timerId());

    QMap<int, Message>::const_iterator it = mPendingMessages.constBegin();
    while (it != mPendingMessages.constEnd()) {
        if (it.value().timerId == event->timerId()) {
            int id = it.key();
            if (mIsStopped || it.value().retryCount >= mConfig.maxRetryCount) {
                mPendingMessages.remove(id);
                emit deliveryFailed(id, it.value().message);
            }
            else {
                sendMessage(it.value().message, id, true);
            }

            return;
        }
        ++it;
    }
}

void Batap::onNewMessageFromMatip()
{
    while (mMatip->hasPendingMessages()) {
        QByteArray message = mMatip->readMessage();

        if (message.startsWith("IMA")) {
            processImaMessage(message);
        }
        else {
            processMessage(message);
        }
    }
}

void Batap::onStopTransmission()
{
    mIsTransmissionStopped = true;
}

void Batap::onResumeTransmission()
{
    mIsTransmissionStopped = false;

    if (isReadyForTransmission()) {
        emit readyForTransmission();
    }
}

void Batap::sendImaMessage()
{
    if (mSerialNumbers.isEmpty())
        return;

    QByteArray message("IMA");
    foreach (int serialNumber, mSerialNumbers) {
        QString strNumber = QString("%1").arg(serialNumber, 3, 10, QChar('0'));
        message.append(strNumber.toLatin1());
    }
    mSerialNumbers.clear();

    mMatip->writeMessage(message);
}

void Batap::sendMessage(const QByteArray message, int id, bool pdm)
{
    char charPdm = pdm ? 'P' : ' ';
    QString serial = QString("%1").arg(id, 3, 10, QChar('0'));

    mMatip->writeMessage(message + charPdm + serial.toLatin1());

    Message &m = mPendingMessages[id];
    m.message = message;
    m.timerId = startTimer(mConfig.noImaTime * 1000);
    m.retryCount++;
}

void Batap::processImaMessage(const QByteArray &message)
{
    /*
     * IMA format:
     *
     * |IMA|   list of three-digit serial numbers   |
     *
     */

    int size = message.size();
    if (size % 3 != 0) {
        emit error(InvalidImaMessageError);
        return;
    }

    QList<int> serialNumbers;
    for (int i = 3; i < size; i += 3) {
        bool ok;
        int serialNumber = message.mid(i, 3).toInt(&ok);

        if (!ok) {
            emit error(InvalidImaMessageError);
            return;
        }

        serialNumbers.append(serialNumber);
    }

    foreach (int id, serialNumbers) {
        if (mPendingMessages.contains(id)) {
            Message m = mPendingMessages.take(id);
            killTimer(m.timerId);

            emit messageDelivered(id);
        }
    }

    if (isReadyForTransmission()) {
        emit readyForTransmission();
    }
}

void Batap::processMessage(const QByteArray &message)
{
    /*
     * message format:
     *
     * |   message   |XNNN
     *
     * if X == ' ': normal message
     * if X == 'P': possible dublicate message
     *
     * NNN: serial number
     *
     */

    int size = message.size();
    if (size < 5) {
        emit error(InvalidMessageSizeError);
        return;
    }

    char pdm = message.at(size - 4);
    if (pdm != ' ' && pdm != 'P') {
        emit error(InvalidTrailerError);
        return;
    }

    bool ok;
    int serialNumber = message.right(3).toInt(&ok);

    if(!ok) {
        emit error(InvalidTrailerError);
        return;
    }

    mSerialNumbers.append(serialNumber);

    emit newMessage(message.left(size - 4));
}
