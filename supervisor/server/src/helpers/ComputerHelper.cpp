#include "ComputerHelper.h"

#include <QtCore/QProcess>

ComputerHelper::ComputerHelper()
{
}

void ComputerHelper::reboot()
{
#if defined(Q_OS_LINUX)
            QProcess::startDetached("sudo /sbin/shutdown -r now");
#elif defined(Q_OS_WIN)
            QProcess::startDetached("shutdown -r -t 1");
#endif
}

void ComputerHelper::shutdown()
{
#if defined(Q_OS_LINUX)
            QProcess::startDetached("sudo /sbin/shutdown -h now");
#elif defined(Q_OS_WIN)
            QProcess::startDetached("shutdown -s -t 1");
#endif
}
