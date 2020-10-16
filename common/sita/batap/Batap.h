#ifndef BATAP_H
#define BATAP_H

#include <QtCore/QMap>
#include <QtCore/QObject>

class QTimer;
class Matip;

//! BATAP protocol implementation
class Batap : public QObject
{
    Q_OBJECT
public:
    //! Batap errors
    enum Error
    {
        InvalidMessageSizeError,
        InvalidImaMessageError,
        InvalidTrailerError
    };

    //! Batap config
    struct BatapConfig
    {
        BatapConfig() :
            noImaTime(30),
            groupImaTime(250),
            transmissionWindow(7),
            maxRetryCount(3)
        {
        }

        int noImaTime;          //!<  No IMA timer in secs
        int groupImaTime;       //!<  Group IMA timer in msecs
        int transmissionWindow; //!<  Maximum number of messages awaiting acknowledgment
        int maxRetryCount;      //!<  Retry counter
//        int expiryTimeout;      //!<  Messages expiration time in secs
    };

    /*!
     * \brief c-tor
     * \param config Batap config
     * \param matip Matip instance
     * \param parent Parent
     */
    Batap(const BatapConfig &config, Matip *matip, QObject *parent = 0);
    ~Batap();

    //! Start sending messages to MATIP layer
    void start();

    //! Stops messages sending
    void stop();

    bool isReadyForTransmission() const;

    int send(const QByteArray &message);

signals:
    //! This signal is emitted when new message is available
    void newMessage(QByteArray message);

    //! This signal is emitted after an error occurred
    void error(Batap::Error);

    void messageDelivered(int id);

    //! This signal is emitted when the message can't be delivered
    void deliveryFailed(int id, QByteArray message);

    void readyForTransmission();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void onNewMessageFromMatip();
    void onStopTransmission();
    void onResumeTransmission();
    void sendImaMessage();

private:
    void sendMessage(const QByteArray message, int id, bool pdm = false);
    void sendMessages();
    void processImaMessage(const QByteArray &message);
    void processMessage(const QByteArray &message);

private:
    struct Message
    {
        Message():
            timerId(0),
            retryCount(0)
        {
        }

        QByteArray message;
        int timerId;
        int retryCount;
    };

    Matip  *mMatip;
    QTimer *mImaTimer;

    bool mIsStopped;
    bool mIsTransmissionStopped;
    int mCurrentSerialNumber;

    QList<int> mSerialNumbers;
    QMap<int, Message> mPendingMessages;

    BatapConfig mConfig;
};

#endif // BATAP_H
