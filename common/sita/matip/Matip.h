#ifndef MATIP_H
#define MATIP_H

#include <QtCore/QObject>
#include <QtCore/QQueue>

class QTimer;
class QTcpSocket;
class MatipHeader;

//! MATIP protocol implementation
class Matip : public QObject
{
    Q_OBJECT
public:
    //! Connection mode
    enum Mode
    {
        ClientMode,
        ServerMode
    };

    //! MATIP connection state
    enum State
    {
        UnconnectedState,
        ListeningState,
        ConnectingState,
        DataTransferState,
        IdleState
    };

    //! MATIP coding
    enum Coding
    {
        BaudotCode,
        IparsCode,
        AsciiCode,
        EbcdicCode,
        UnknownCode
    };

    //! MATIP errors
    enum Error
    {
        InvalidHeaderError,
        VersionMismatchError,
        UnknownCommandError,
        UnexpectedCommandError
    };

    //! The reason for the MATIP session refusal
    enum RefuseCause
    {
        NoTrafficTypeMatching,
        InformationInHeaderIncoherent,
        TypeOfProtectionMechanismAreDifferent,
        ClientUserNotConfiguredInServer,
        ConnectionRequestsCollision,
        ConnectionDisabledBySupervisor,
        OpenRequestForSessionAlreadyOpen,
        RecipientResourceIdMismatch,
        UnknownCause
    };

    struct MatipConfig
    {
        MatipConfig() :
            sessionStatusQueryTimeout(30),
            maxSessionStatusQueryNumber(3)
        {
        }

        Mode mode;                          //!< MATIP mode
        int sessionStatusQueryTimeout;      //!< Timeout for "Session Status Query" packet
        int maxSessionStatusQueryNumber;    //!< Maximum number of unanswered "Session Status Query" packets
    };

    //! c-tor
    explicit Matip(const MatipConfig &config, QTcpSocket *socket, QObject *parent = 0);

    //! Returns MATIP connection state
    State state() const { return mState; }

    //! Returns TCP socket
    QTcpSocket* socket() const { return mSocket; }

    /*!
     * \brief Opens the MATIP session
     * \param[in] cd Coding
     * \param[in] senderId Local resource ID
     * \param[in] recipientId Remote resource ID
     */
    void openSession(Coding cd, quint16 senderId, quint16 recipientId);

    //! Closes the MATIP session
    void closeSession();

    //! Asks remote side to stop transmission
    void sendStopTransmission();

    //! Asks remote side to resume transmission
    void sendResumeTransmission();

    //! Accepts new MATIP session
    void acceptConnection();

    //! Refuses MATIP session
    void refuseConnection(RefuseCause cause);

    //! Reads next message
    QByteArray readMessage();

    //! Writes the message
    void writeMessage(const QByteArray &message);

    //! Returns true if MATIP has unread messages
    bool hasPendingMessages() const;

signals:
    //! This signal is emitted when new MATIP session has been requested
    void sessionRequested(quint16 senderId, quint16 recipientId);

    //! This signal is emitted when MATIP session has been accepted
    void sessionAccepted();

    //! This signal is emitted when MATIP session has been refused
    void sessionRefused(Matip::RefuseCause cause);

    //! This signal is emitted when MATIP session has been closed
    void sessionClosed();

    //! This signal is emitted when MATIP stops transmission
    void stopTransmission();

    //! This signal is emitted when MATIP resumes transmission
    void resumeTransmission();

    //! This signal is emitted when new message is available
    void newMessage(QByteArray message);

    //! This signal is emitted when message has been written to the network
    void messageWritten(QByteArray message);

    //! This signal is emitted when maximum number of unanswered "Session Status Query" packets has been reached
    void sessionTimeout();

    //! This signal is emitted after an error occurred
    void error(Matip::Error);

    void receivedSize(quint32 size);
    void sentSize(quint32 size);

private slots:
    void onReadyRead();
    void onTimeout();
    void onBytesWritten(qint64 bytes);

private:
    static const int headerSize = 4;

    void write(const uchar *data, qint64 size);

    void processIdleState(const MatipHeader &header, const QByteArray &data);
    void processConnectingState(const MatipHeader &header, const QByteArray &data);
    void processListeningState(const MatipHeader &header, const QByteArray &data);

    void appendData();

    QTimer     *mTimer;
    QTcpSocket *mSocket;

    State   mState;
    quint16 mBytesToRead;
    int     mQueryCounter;

    QQueue<QByteArray> mInputQueue;
    QByteArray         mTempMessage;

    MatipConfig mConfig;
};

#endif // MATIP_H
