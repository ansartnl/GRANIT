#ifndef INCOMINGDATAHANDLER_H
#define INCOMINGDATAHANDLER_H

#include <QtCore/QMutex>
#include <QtCore/QObject>

namespace local_connection
{

class IncomingDataHandler : public QObject
{
    Q_OBJECT
public:
    explicit IncomingDataHandler(QObject *parent = 0);

public slots:
    void append(const QByteArray &ba);

signals:
    void readyRead(const QVariant &data);

private:
    void pAppend(const QByteArray &ba);
    void tryProcess();
    size_t bufferSize() const;
    qint64 packageSize() const;
    const QByteArray takeLeft(qint64 size);

private slots:
    void process();

private:
    mutable QMutex mutex;
    QByteArray mBuffer;
};

} // namespace local_connection

#endif // INCOMINGDATAHANDLER_H
