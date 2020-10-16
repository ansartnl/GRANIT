#ifndef BATAPMESSAGESTORAGE_H
#define BATAPMESSAGESTORAGE_H

#include <QtCore/QVariant>

class BatapMessageStorage : public QObject
{
    Q_OBJECT
public:
    BatapMessageStorage(QObject *parent = 0) : QObject(parent)
    {
    }

public slots:
    virtual void removeMessage(QVariant id) = 0;
    virtual void requestNextMessage(QList<QVariant> ids) = 0;

signals:
    void hasMessage();
    void requestedMessage(QVariant id, QByteArray message);
    void error();
};

#endif // BATAPMESSAGESTORAGE_H
