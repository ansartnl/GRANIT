#include "Service.h"

Service::Service(QObject *parent) :
    QObject(0)
{
    if (parent)
        connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));
}

void Service::start()
{
    pStart();

    emit started();
}

void Service::stop()
{
    pStop();

    emit stopped();
}
