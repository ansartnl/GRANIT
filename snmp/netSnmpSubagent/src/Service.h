#ifndef SERVICE_H
#define SERVICE_H

#include <QtCore/QObject>

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QObject *parent = 0);

public slots:
    void start();
    void stop();

signals:
    void started();
    void stopped();

protected:
    virtual void pStart() = 0;
    virtual void pStop() = 0;
};

#endif // SERVICE_H
