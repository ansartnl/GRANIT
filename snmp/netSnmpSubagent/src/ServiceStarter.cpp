#include "ServiceStarter.h"

#include "Service.h"

#include <QtCore/QThread>
#include <QtCore/QVariant>

ServiceStarter::ServiceStarter(QObject *parent) :
    QObject(parent)
{
}

void ServiceStarter::installService(Service *newService)
{
    static const char kServicePointer[] = "service_pointer";

    QThread *newThread = new QThread(this);
    newService->moveToThread(newThread);
    newThread->setProperty(kServicePointer, reinterpret_cast < quintptr >(newService));

    connect(newThread, SIGNAL(started()), newService, SLOT(start()));
    connect(newThread, SIGNAL(finished()), newService, SLOT(stop()));

    if (mThreads.isEmpty()) {
        connect(newService, SIGNAL(started()), SIGNAL(started()));
        connect(newService, SIGNAL(stopped()), SIGNAL(stopped()));
    } else {
        QThread *lastThread = mThreads.last();
        Service *lastService = reinterpret_cast < Service * >(
                    lastThread->property(kServicePointer).value < quintptr >());
        connect(lastService, SIGNAL(started()), newThread, SLOT(start()));
        connect(newService, SIGNAL(stopped()), lastThread, SLOT(quit()));

        disconnect(lastService, SIGNAL(started()), this, SIGNAL(started()));
        connect(newService, SIGNAL(started()), SIGNAL(started()));
    }

    mThreads << newThread;
}

void ServiceStarter::start()
{
    if (mThreads.isEmpty()) {
        emit started();
        return;
    }
    mThreads.first()->start();
}

void ServiceStarter::stop()
{
    if (mThreads.isEmpty()) {
        emit stopped();
        return;
    }
    mThreads.last()->quit();
}
