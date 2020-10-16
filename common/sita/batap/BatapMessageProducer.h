#ifndef BATAPMESSAGEPRODUCER_H
#define BATAPMESSAGEPRODUCER_H

#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtCore/QVariant>

class Batap;
class BatapMessageStorage;

class BatapMessageProducer : public QObject
{
    Q_OBJECT
public:
    BatapMessageProducer(Batap *batap, BatapMessageStorage *storage, QObject *parent = 0);

private slots:
    void onRequestedMessage(QVariant id, QByteArray message);
    void onMessageDelivered(int id);
    void start();

private:
    void requestNextMessage();

private:
    Batap *mBatap;
    BatapMessageStorage *mStorage;

    QMap<int, QVariant> mIdMap;
    bool mRunning;
};

#endif // BATAPMESSAGEPRODUCER_H
