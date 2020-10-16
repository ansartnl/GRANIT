#ifndef NETCONTROLLER_H
#define NETCONTROLLER_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QMutex>

#include <QtNetwork/QNetworkInterface>

typedef QList<QNetworkInterface> NetworkList;

bool operator ==(const QNetworkInterface &net1, const QNetworkInterface &net2);

class NetController : public QObject
{
    Q_OBJECT
public:
    explicit NetController(QObject *parent = 0);

    void startMonitoring(const int interval = 5000);
    void stopMonitoring();

    static NetworkList getList();

signals:
    void up(const QNetworkInterface &interface);
    void down(const QNetworkInterface &interface);
    void list(const NetworkList &list);

protected slots:
    void timerEvent(QTimerEvent *);

protected:
    NetworkList lastList;
    int timerId;
    static QMutex mutex;
};

#endif // NETCONTROLLER_H
