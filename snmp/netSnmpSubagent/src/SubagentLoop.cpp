#include "SubagentLoop.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include <errno.h>

SubagentLoop::SubagentLoop(int timerMs, QObject *parent) :
    QThread(0), mTimerMs(timerMs), mStop(true)
{
    connect(parent, SIGNAL(destroyed()), SLOT(deleteLater()));
}

void SubagentLoop::stop()
{
    QWriteLocker locker(&lock);
    mStop = true;
}

void SubagentLoop::run()
{
    mStop = false;

    int numfds = 0;
    fd_set fdset;
    timeval timeout = { LONG_MAX, 0 }, *tvp = &timeout;
    int block = 0;
    int count = 0;

    while (!needStop()) {
        numfds = 0;
        snmp_select_info(&numfds, &fdset, tvp, &block);
        if (block != 0) {
            tvp = 0;
        }
        count = select(numfds, &fdset, 0, 0, tvp);

        if (count > 0) {
            snmp_read(&fdset);
        } else {
            switch (count) {
            case 0:
                snmp_timeout();
                break;
            case -1:
                if (errno != EINTR) {
                    snmp_log_perror("select");
                }
                break;
            default:
                snmp_log(LOG_ERR, "select returned %d\n", count);
                break;
            }
        }

        msleep(mTimerMs);
    }
}

bool SubagentLoop::needStop() const
{
    QReadLocker locker(&lock);
    return mStop;
}
