#include "QWinProcessStarter.h"

#ifdef Q_OS_WIN

#include "Log.h"

#include <QtCore/QFileInfo>

QWinProcessStarter::QWinProcessStarter(const QByteArray &program
                                       , const QByteArray &arguments
                                       , QObject *parent) :
    QObject(parent)
{
    mProgram = program;
    mArguments = program + " " + arguments;
    hToken = 0;
    hProcess = 0;
    dwThreadId = -1;

    QFileInfo fi(program);
    mDir = fi.absolutePath().toUtf8();
}

void QWinProcessStarter::start()
{
    STARTUPINFOA si;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    if (hToken)
    {
        if (!CreateProcessAsUserA(hToken, mProgram.data(), mArguments.data(), 0, 0, false
                                  , DETACHED_PROCESS
                                  , 0, mDir.data(), &si, &pi))
        {
            emit processError(GetLastError());
        } else
        {
            hProcess = pi.hProcess;
            dwThreadId = pi.dwThreadId;
            emit processStarted();
        }
    } else
    {
        if (!CreateProcessA(mProgram.data(), mArguments.data(), 0, 0, false
                           , DETACHED_PROCESS
                           , 0, mDir.data(), &si, &pi))
        {
            emit processError(GetLastError());
        } else
        {
            hProcess = pi.hProcess;
            dwThreadId = pi.dwThreadId;
            emit processStarted();
        }
    }
}

void QWinProcessStarter::stop()
{
    if (hProcess)
    {
        PostThreadMessage(dwThreadId, WM_QUIT, 0, 0);
        DWORD waitCode = WaitForSingleObject(hProcess, 5000);
        DWORD exitCode;
        switch (waitCode)
        {
        case WAIT_OBJECT_0:
            GetExitCodeProcess(hProcess, &exitCode);
            emit processFinished(exitCode, QProcess::NormalExit);
            break;
        case WAIT_TIMEOUT:
            TerminateProcess(hProcess, -1);
            GetExitCodeProcess(hProcess, &exitCode);
            emit processFinished(exitCode, QProcess::CrashExit);
            break;
        case WAIT_FAILED:
            emit processError(GetLastError());
            break;
        }
    }
}

#endif // Q_OS_WIN
