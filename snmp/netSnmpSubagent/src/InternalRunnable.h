#ifndef INTERNALRUNNABLE_H
#define INTERNALRUNNABLE_H

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QVariant>

namespace local_connection
{

class LocalSocket;

} // namespace local_connection

class InternalRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit InternalRunnable(local_connection::LocalSocket *socket, const QVariant &request);

    void run();

signals:
    void send(local_connection::LocalSocket *socket, const QVariant &reply);

private:
    local_connection::LocalSocket *mSocket;
    QVariant mRequest;
};

#endif // INTERNALRUNNABLE_H
