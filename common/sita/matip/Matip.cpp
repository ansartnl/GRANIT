#include "Matip.h"
#include "MatipHeader.h"

#include <QtCore/QtEndian>
#include <QtCore/QTimer>
#include <QtNetwork/QTcpSocket>

#ifdef MATIP_DEBUG
#define DEBUG(x) (qDebug() << QString("MATIP[%1]: %2").arg(objectName()).arg(QString(x)))
#else
#define DEBUG(x)
#endif

#define WARNING(x)

Matip::Matip(const MatipConfig &config, QTcpSocket *socket, QObject *parent) :
    QObject(parent), mTimer(0), mSocket(socket),
    mQueryCounter(0), mConfig(config)
{
    mSocket->setParent(this);

    mState = (config.mode == ClientMode) ? UnconnectedState : ListeningState;

    mTimer = new QTimer(this);

    mTimer->setInterval(mConfig.sessionStatusQueryTimeout * 1000);

    connect(mTimer, SIGNAL(timeout()), SLOT(onTimeout()));

    connect(mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(mSocket, SIGNAL(readyRead()), mTimer, SLOT(start()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), SLOT(onBytesWritten(qint64)));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), mTimer, SLOT(start()));
}

void Matip::openSession(Coding cd, quint16 senderId, quint16 recipientId)
{
    uchar data[10];

    MatipHeader::make(data, MatipHeader::SessionOpenCommand, 10);

    data[4] = 0x00;
    data[5] = 0x22;

    switch (cd) {
    case BaudotCode:
        data[4] |= 0x00;
        break;
    case IparsCode:
        data[4] |= 0x02;
        break;
    case AsciiCode:
        data[4] |= 0x04;
        break;
    case EbcdicCode:
        data[4] |= 0x06;
        break;
    default:
        ;
    }

    qToBigEndian(senderId, data + 6);
    qToBigEndian(recipientId, data + 8);

    mState = ConnectingState;
    write(data, 10);
}

void Matip::closeSession()
{
    /*
     * Session Close command
     * byte0-byte3: header
     * byte4:       close cause
     *
     */

    mState = UnconnectedState;

    uchar data[5];

    MatipHeader::make(data, MatipHeader::SessionCloseCommand, 5);
    data[4] = 0x00;

    write(data, 5);
    mSocket->flush();
}

void Matip::sendStopTransmission()
{
    uchar data[4];

    MatipHeader::make(data, MatipHeader::StopTransmissionCommand, 4);

    write(data, 4);
}

void Matip::sendResumeTransmission()
{
    uchar data[4];

    MatipHeader::make(data, MatipHeader::ResumeTransmissionCommand, 4);

    write(data, 4);
}

void Matip::acceptConnection()
{
    uchar data[5];

    MatipHeader::make(data, MatipHeader::OpenConfirmCommand, 5);
    data[4] = 0x00;

    write(data, 5);

    mState = IdleState;
}

void Matip::refuseConnection(RefuseCause cause)
{
    uchar data[5];

    MatipHeader::make(data, MatipHeader::OpenConfirmCommand, 5);

    data[4] = 0x40;
    switch (cause)
    {
    case NoTrafficTypeMatching:
        data[4] |= 0x01;
        break;
    case InformationInHeaderIncoherent:
        data[4] |= 0x02;
        break;
    case TypeOfProtectionMechanismAreDifferent:
        data[4] |= 0x03;
        break;
    case ClientUserNotConfiguredInServer:
        data[4] |= 0x04;
        break;
    case ConnectionRequestsCollision:
        data[4] |= 0x05;
        break;
    case ConnectionDisabledBySupervisor:
        data[4] |= 0x06;
        break;
    case OpenRequestForSessionAlreadyOpen:
        data[4] |= 0x07;
        break;
    case RecipientResourceIdMismatch:
        data[4] |= 0x08;
        break;
    default:
        ;
    }

    write(data, 5);
}

QByteArray Matip::readMessage()
{
    return mInputQueue.dequeue();
}

void Matip::writeMessage(const QByteArray &message)
{
    DEBUG("send message: [" + message + "]");

    uchar data[headerSize];
    MatipHeader::make(data, MatipHeader::NoCommand, headerSize + message.size());

    write(data, headerSize);
    write(reinterpret_cast<const uchar *>(message.data()), message.size());

    emit messageWritten(message);
}

bool Matip::hasPendingMessages() const
{
    return !mInputQueue.isEmpty();
}

void Matip::onReadyRead()
{
    if (mState == UnconnectedState)
    {
        mSocket->readAll();
        return;
    }

    if (mState != DataTransferState)
    {
        if (mSocket->bytesAvailable() < headerSize)
            return;

        uchar headerData[headerSize];
        mSocket->peek(reinterpret_cast<char *>(headerData), headerSize);

        MatipHeader header(headerData);

        DEBUG("header received: " + header.rawData().toHex());

        if (!header.isValid())
        {
            if (header.error() == MatipHeader::InvalidHeaderError)
                emit error(InvalidHeaderError);
            else if (header.error() == MatipHeader::VersionMismatchError)
                emit error(VersionMismatchError);

            mSocket->read(headerSize);
            return;
        }

        QByteArray payload;
        if (header.isControlPacket())
        {
            if (mSocket->bytesAvailable() < headerSize + header.dataSize())
                return;

            // skip header
            mSocket->read(headerSize);

            payload = mSocket->read(header.dataSize());

            if (header.command() == MatipHeader::UnknownCommand)
            {
                emit error(UnknownCommandError);

                return;
            }
        }
        else
        {
            mSocket->read(headerSize);
        }

        switch (mState)
        {
        case ListeningState:
            processListeningState(header, payload);
            break;
        case ConnectingState:
            processConnectingState(header, payload);
            break;
        case IdleState:
            processIdleState(header, payload);
            break;
        default:
            ;
        }
    }
    else
    {
        appendData();
    }

    if (mSocket->bytesAvailable())
        onReadyRead();
}

void Matip::onTimeout()
{
    if (mState == IdleState || mState == DataTransferState)
    {
        if (mQueryCounter == mConfig.maxSessionStatusQueryNumber)
        {
            closeSession();
            emit sessionTimeout();
        }
        else
        {
            uchar data[4];
            MatipHeader::make(data, MatipHeader::SessionStatusQueryCommand, 4);
            write(data, 4);

            ++mQueryCounter;
        }
    }
}

void Matip::onBytesWritten(qint64 bytes)
{
    emit sentSize(bytes);
}

void Matip::processConnectingState(const MatipHeader &header, const QByteArray &data)
{
    if (header.command() == MatipHeader::OpenConfirmCommand)
    {
        /*
         * Open Confirm command
         * byte0-byte3: header
         * byte4:       00000000 (session accepted) or 01xxxxxx (session refused)
         *
         */

        DEBUG("command received: Open Confirm");

        uchar c = data.at(0);

        // session accepted
        if ((c >> 6) == 0x00)
        {
            mState = IdleState;

            emit sessionAccepted();
        }
        // session refused
        else
        {
            RefuseCause cause;
            switch (c & 0x3F)
            {
            case 0x01:
                cause = NoTrafficTypeMatching;
                break;
            case 0x02:
                cause = InformationInHeaderIncoherent;
                break;
            case 0x03:
                cause = TypeOfProtectionMechanismAreDifferent;
                break;
            case 0x04:
                cause = ClientUserNotConfiguredInServer;
                break;
            case 0x05:
                cause = ConnectionRequestsCollision;
                break;
            case 0x06:
                cause = ConnectionDisabledBySupervisor;
                break;
            case 0x07:
                cause = OpenRequestForSessionAlreadyOpen;
                break;
            case 0x08:
                cause = RecipientResourceIdMismatch;
                break;
            default:
                cause = UnknownCause;
            }

            mState = UnconnectedState;

            emit sessionRefused(cause);
        }
    }
    else
    {
        WARNING("unexpected command received: " << (int) header.command());
        emit error(UnexpectedCommandError);
    }
}

void Matip::processListeningState(const MatipHeader &header, const QByteArray &data)
{
    if (header.command() == MatipHeader::SessionOpenCommand)
    {
        if (data.size() < 6)
        {
            refuseConnection(InformationInHeaderIncoherent);
            return;
        }

        quint8 trafficType = (quint8(data[0]) >> 3) & 0x03;
        if (trafficType != 0x00)
        {
            refuseConnection(NoTrafficTypeMatching);
            return;
        }

        quint16 senderId = qFromBigEndian<quint16>( reinterpret_cast<const uchar*>(data.constData()+2) );
        quint16 recipientId = qFromBigEndian<quint16>( reinterpret_cast<const uchar*>(data.constData()+4) );

        emit sessionRequested(senderId, recipientId);
    }
}

void Matip::write(const uchar *data, qint64 size)
{
    DEBUG("write to socket: " + QByteArray(reinterpret_cast<const char *>(data), size).toHex());
    mSocket->write(reinterpret_cast<const char *>(data), size);
}

void Matip::processIdleState(const MatipHeader &header, const QByteArray &data)
{
    Q_UNUSED(data);
    switch (header.command())
    {
    case MatipHeader::NoCommand:
        mBytesToRead = header.dataSize();

        mTempMessage.clear();
        mTempMessage.reserve(mBytesToRead);

        appendData();
        break;

    case MatipHeader::SessionCloseCommand:
        DEBUG("command received: Session Close");
        mState = UnconnectedState;
        emit sessionClosed();
        break;

    case MatipHeader::SessionStatusQueryCommand:
        DEBUG("command received: Session Status Query");
        uchar data[4];
        MatipHeader::make(data, MatipHeader::SessionStatusResponseCommand, 4);
        write(data, 4);
        break;

    case MatipHeader::SessionStatusResponseCommand:
        DEBUG("command received: Session Status Response");
        mQueryCounter = 0;
        break;

    case MatipHeader::StopTransmissionCommand:
        DEBUG("command received: Stop Transmission");
        emit stopTransmission();
        break;

    case MatipHeader::ResumeTransmissionCommand:
        DEBUG("command received: Resume Transmission");
        emit resumeTransmission();
        break;

    case MatipHeader::SessionOpenCommand:
        WARNING("unexpected command received: Session Open");
        emit error(UnexpectedCommandError);
        break;

    case MatipHeader::OpenConfirmCommand:
        WARNING("unexpected command received: Open Confirm");
        emit error(UnexpectedCommandError);
        break;

    case MatipHeader::UnknownCommand:
        WARNING("unknown command received");
        emit error(UnknownCommandError);
        break;
    }
}

void Matip::appendData()
{
    QByteArray data = mSocket->read(mBytesToRead);
    mBytesToRead -= data.size();

    mTempMessage.append(data);

    if (mBytesToRead == 0)
    {
        DEBUG("message received: [" + mTempMessage + "]");

        mInputQueue.enqueue(mTempMessage);

        mState = IdleState;

        emit newMessage(mTempMessage);
    }
    else
    {
        mState = DataTransferState;
    }
}
