#include "NetController.h"

#include <QtCore/QMutexLocker>

QMutex NetController::mutex;

NetController::NetController(QObject *parent) :
    QObject(parent)
{
    timerId = 0;
}

void NetController::startMonitoring(const int interval)
{
    if (timerId)
        stopMonitoring();
    timerId = startTimer(interval);
}

void NetController::stopMonitoring()
{
    killTimer(timerId);
    timerId = 0;
}

NetworkList NetController::getList()
{
    QMutexLocker locker(&mutex);

    NetworkList result;
// getting list of all ipv4 addresses, except local
    QList<QHostAddress> hostList;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.toIPv4Address() && (address != QHostAddress(QHostAddress::LocalHost)))
            hostList.append(address);
    }
// getting all interfaces, having ipv4 address and status - running
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces())
    {
        if (interface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            foreach (const 	QNetworkAddressEntry &entry, interface.addressEntries())
            {
                if (hostList.contains(entry.ip()))
                {
                    result.append(interface);
                    break;
                }
            }
        }
    }
    return result;
}

void NetController::timerEvent(QTimerEvent *)
{
    NetworkList newList = getList();
    emit list(newList);
    foreach (const QNetworkInterface &newInterface, newList)
    {
        if (!lastList.contains(newInterface))
        {
            emit up(newInterface);
            lastList.append(newInterface);
        }
    }
    foreach (const QNetworkInterface &oldInterface, lastList)
    {
        if (!newList.contains(oldInterface))
        {
            emit down(oldInterface);
            lastList.removeAll(oldInterface);
        }
    }
}

bool operator ==(const QNetworkInterface &net1, const QNetworkInterface &net2)
{
    if (net1.addressEntries() != net2.addressEntries())
        return false;
    if (net1.hardwareAddress() != net2.hardwareAddress())
        return false;
    if (net1.humanReadableName() != net2.humanReadableName())
        return false;
    if (net1.index() != net2.index())
        return false;
    if (net1.name() != net2.name())
        return false;
    return true;
}
