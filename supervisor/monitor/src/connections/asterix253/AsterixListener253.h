#ifndef ASTERIXLISTENER253_H
#define ASTERIXLISTENER253_H

#include <QUdpSocket>
#include <QList>
#include <QTime>
#include <QTimer>
#include <QColor>
#include <QMutex>

#include "src/connections/IConnection.h"

namespace asterix
{
    //! Asterix listener
    class AsterixListener253 :
                    public QObject,
                    public IConnection
                    //public IAsterixDataSrc
    {
    Q_OBJECT
    public:
            //! Constructor
            /*!
                    \param pParent pointer to data handler
                    \param qsIP address for listening
                    \param quiPort port for listening
            */
            AsterixListener253(const QString& qsIP, quint16 quiPort);
            AsterixListener253(const QString& qsID, const QString& qsIP, quint16 quiPort);
            //! Destructor
            virtual ~AsterixListener253();
            //! Start listening
            virtual void Start();
            //! Stop listening
            virtual void Stop();
            //! IConnection
            const QString id() const { return mId; }
            const QString host() const { return m_Host.toString(); }
            quint16 port() const { return m_ushPort; }
            void setID(const QString& id) {mId = id;}
            //! Block for incoming messages.
            virtual void BlockMessages(bool on)
            { mIsMessagesBlocked = on; }

    signals:
            void asterix253Data(const QVariant& data);
            void isIdle();
            void eventToLog(const QString &host, const QString &service
                            , const QString &message, QColor color = Qt::black);

    protected slots:
            //! Slot to QUdpSocket::readyRead() signal
            virtual void OnUDPData();
            //! Slot for checking last data recieved timeout
            void checkIdle();

    protected:
            void setupTimeout();

    protected:
            //@}
            //! Pointer to data handler
            //IParent* m_pParent;
            //! Host name for listen
            QHostAddress m_Host;
            //! Port for listening
            quint16 m_ushPort;
            //! Low-level UDP socket
            QUdpSocket m_Socket;
            /** State of blocking incoming messages */
            bool       mIsMessagesBlocked;
            //! Identificator
            QString mId;

            int mIdleTimeout;
            QTime mLastDataTime;
            QTimer mIdleTimer;

            QMutex mutex;
    };
}

#endif // ASTERIXLISTENER253_H
