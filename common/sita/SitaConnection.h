#ifndef SITACONNECTION_H
#define SITACONNECTION_H

#include "Matip.h"
#include "Batap.h"

class QTcpSocket;
class BatapMessageProducer;
class BatapMessageStorage;

class SitaConnection : public QObject
{
    Q_OBJECT
public:
    struct SitaConfig
    {
        Matip::MatipConfig matipConfig;
        Batap::BatapConfig batapConfig;
    };

    SitaConnection(const SitaConfig &config, QTcpSocket *socket, QObject *parent = 0);

    void acceptSession(BatapMessageStorage *storage);
    void refuseSession(Matip::RefuseCause cause);
    void close();

    QString name() const {return mName;}

    QString address() const {return mAddress;}
    void setAddress(const QString &address) {mAddress = address;}

public slots:
    void start();
    void stop();

signals:
    void newMessage(QByteArray message);
    void sessionRequested(quint16 senderId, quint16 recipientId);
    void sessionClosed();
    void closed();

private slots:
    void onSessionClosed();
    void onSocketDisconnected();

private:
    QTcpSocket *mSocket;
    Matip      *mMatip;
    Batap      *mBatap;
    BatapMessageProducer *mBatapMessageProducer;

    SitaConfig mConfig;

    QString mName;
    QString mAddress;
};

#endif // SITACONNECTION_H
