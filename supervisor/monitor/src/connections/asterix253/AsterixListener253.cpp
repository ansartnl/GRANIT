#include "AsterixListener253.h"
#include "Asterix253Struct.h"
#include "peleng/asterix253peleng.h"

#include "TVAError.h"
#include "Log.h"
#include "src/main.h"

#include "asterixdata.h"
#include "asterixerrors.h"

namespace asterix
{
    AsterixListener253::AsterixListener253(const QString& qsIP, quint16 quiPort)
        : m_Host(qsIP), m_ushPort(quiPort), mIsMessagesBlocked(false), mLastDataTime(QTime::currentTime())
    {
        setupTimeout();
    }

    AsterixListener253::AsterixListener253(const QString &qsID, const QString &qsIP, quint16 quiPort)
        : m_Host(qsIP), m_ushPort(quiPort), mIsMessagesBlocked(false), mId(qsID), mLastDataTime(QTime::currentTime())
    {
        setupTimeout();
    }

    AsterixListener253::~AsterixListener253()
    {
            Stop();
    }

    void AsterixListener253::Start()
    {
            Stop();

            TVA_CHECK(
                    m_Socket.bind(m_Host, m_ushPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint),
                    (tr("Unable to listen adress") + (": %1 : %2")).arg(m_Host.toString()).arg(m_ushPort),
                    E_FAIL);
            connect(&m_Socket, SIGNAL(readyRead()), this, SLOT(OnUDPData()));
    }

    void AsterixListener253::Stop()
    {
            disconnect(&m_Socket, SIGNAL(readyRead()), 0, 0);
            m_Socket.close();
    }

    void AsterixListener253::OnUDPData()
    {
        QMutexLocker locker(&mutex);

        mLastDataTime = QTime::currentTime();
        if(!mIdleTimer.isActive())
            mIdleTimer.start();

        int iAttemp(5);
        while (m_Socket.hasPendingDatagrams() && iAttemp-- > 0) {
            QByteArray qabBuffer;
            qabBuffer.resize(m_Socket.pendingDatagramSize());
            QHostAddress SenderIP;
            quint16 quiSenderPort(0);
            m_Socket.readDatagram(qabBuffer.data(), qabBuffer.size(),
                                                            &SenderIP, &quiSenderPort);


            logManager().asterixLog(mId, qabBuffer);

            if ( mIsMessagesBlocked ) //|| !m_pParent )
                continue;

            // skip header
            qabBuffer.remove(0, 25);

            TVA_CATCHALL_TRY
            {
                AsterixData aData(253, AsterixPeleng);
                int readedBytes = 0;
                while ( true )
                {
                    AsterixErrors::ErrorCode errorCode = aData.fromArray(qabBuffer, &readedBytes);
                    if ( errorCode == AsterixErrors::Error_MessageIsShort )
                        break;

                    bool isErrorExists = false;
                    bool isProtocolNotSupported = false;
                    // Dump errors to log.
                    foreach ( AsterixErrors *error, aData.errors() )
                    {
                        if ( *error == AsterixErrors::OK ||
                             *error == AsterixErrors::Error_Mandatory )
                            continue;

                        QString errorString = tr("Asterix listener: ") + error->toString();

                        if ( error->code() == AsterixErrors::Error_ProtocolNotSupported )
                        {
                            errorString += tr(". Asterix category: ") + QString("%1").arg(int(aData.category), 3, 10, QChar('0'));
                            isProtocolNotSupported = true;
                        }

                        if ( error->code() == AsterixErrors::Error_UnexpectedEndOfData ||
                             error->code() == AsterixErrors::Error_ProtocolNotSupported )
                        {
                            errorString += tr(". Buffer dump: ") + qabBuffer.toHex();
                        }

                        if ( error->code() != AsterixErrors::Error_Mandatory )
                            isErrorExists = true;

                        qLogInfo("Astrix253 listener") << errorString;
                        //LOG_STR(INFO, errorString);
                    }

                    // If error is occured -> break datagram dump.
                    if ( isErrorExists && !isProtocolNotSupported )
                        break;

                    qabBuffer.remove(0, readedBytes);

                    // Only cat. 253 makes sense here.
                    if ( aData.category != 253 )
                        continue;

                    Asterix253Struct result;

                    result.dataSource.sac = aData.value(Peleng::Asterix253::DataSourceIdentifier_SAC).toUInt();
                    result.dataSource.sic = aData.value(Peleng::Asterix253::DataSourceIdentifier_SIC).toUInt();
                    result.messageType = aData.value(Peleng::Asterix253::MessageType).toUInt();
                    result.serviceIdentifier = aData.value(Peleng::Asterix253::ServiceIdentifier).toUInt();

                    switch (aData.value(Peleng::Asterix253::State).toUInt())
                    {
                    case 0xFF:
                        result.state = Asterix253Struct::NotSet;
                        break;
                    case 0xF0:
                        result.state = Asterix253Struct::Master;
                        break;
                    case 0x00:
                        result.state = Asterix253Struct::Slave;
                        break;
                    case 0xF1:
                        result.state = Asterix253Struct::Undefined;
                        break;
                    }

                    switch (aData.value(Peleng::Asterix253::Mode).toUInt())
                    {
                    case 0xF0:
                        result.mode = Asterix253Struct::Mode0;
                        break;
                    case 0xF1:
                        result.mode = Asterix253Struct::Mode1;
                        break;
                    }

                    QVariantList sac = aData.value(Peleng::Asterix253::ChannelState_SAC).toList();
                    QVariantList sic = aData.value(Peleng::Asterix253::ChannelState_SIC).toList();
                    QVariantList first = aData.value(Peleng::Asterix253::ChannelState_First).toList();
                    QVariantList second = aData.value(Peleng::Asterix253::ChannelState_Second).toList();

                    for (int i = 0; i < sac.count(); ++i)
                    {
                        Asterix253Struct::ChannelState cs;

                        cs.dataSource.sac = sac[i].toUInt();
                        cs.dataSource.sic = sic[i].toUInt();

                        switch (first[i].toUInt())
                        {
                        case 0xFF:
                            cs.first = Asterix253Struct::ChannelState::Valid;
                            break;
                        case 0xF0:
                            cs.first = Asterix253Struct::ChannelState::NotConnected;
                            break;
                        case 0xF1:
                            cs.first = Asterix253Struct::ChannelState::NotInitiated;
                            break;
                        case 0x00:
                            cs.first = Asterix253Struct::ChannelState::Disabled;
                            break;
                        }

                        switch (second[i].toUInt())
                        {
                        case 0xFF:
                            cs.second = Asterix253Struct::ChannelState::Valid;
                            break;
                        case 0xF0:
                            cs.second = Asterix253Struct::ChannelState::NotConnected;
                            break;
                        case 0xF1:
                            cs.second = Asterix253Struct::ChannelState::NotInitiated;
                            break;
                        case 0x00:
                            cs.second = Asterix253Struct::ChannelState::Disabled;
                            break;
                        }

                        result.channelState.append(cs);
                    }

                    QVector<quint8> v(2);
                    v[0] = aData.value(Peleng::Asterix253::AdapterState_First).toUInt();
                    v[1] = aData.value(Peleng::Asterix253::AdapterState_Second).toUInt();

                    foreach (quint8 state, v)
                    {
                        Asterix253Struct::AdapterState as;

                        switch (state)
                        {
                        case 0xFF:
                            as.state = Asterix253Struct::AdapterState::Valid;
                            break;
                        case 0xF0:
                            as.state = Asterix253Struct::AdapterState::NotConnected;
                            break;
                        case 0x00:
                            as.state = Asterix253Struct::AdapterState::NotInitiated;
                            break;
                        }

                        result.adapterState.append(as);
                    }

                    emit asterix253Data(QVariant::fromValue(result));
                    //m_pParent->OnAsterix253Data(QVariant::fromValue(result));
                }

            } TVA_TRAPALL{}
        }
    }

    void AsterixListener253::checkIdle()
    {
        if(mLastDataTime.secsTo(QTime::currentTime()) > mIdleTimeout)
        {
            emit isIdle();
            mIdleTimer.stop();
            emit eventToLog(mId, QString(""), QString("Has become IDLE"), Qt::red);
        }
    }

    void AsterixListener253::setupTimeout()
    {
        mIdleTimeout = globalSettings()->value("Asterix253IdleTimeout", 24).toInt();

        mIdleTimer.setSingleShot(false);
        mIdleTimer.setInterval(1000);
        connect(&mIdleTimer, SIGNAL(timeout()), this, SLOT(checkIdle()));
    }
}
