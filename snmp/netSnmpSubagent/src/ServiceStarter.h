#ifndef SERVICESTARTER_H
#define SERVICESTARTER_H

#include <QtCore/QObject>

class Service;

class QThread;

class ServiceStarter : public QObject
{
    Q_OBJECT
public:
    explicit ServiceStarter(QObject *parent = 0);

    void installService(Service *newService);

public slots:
    void start();
    void stop();

signals:
    void started();
    void stopped();

private:
    QList < QThread * > mThreads;
};

#endif // SERVICESTARTER_H
