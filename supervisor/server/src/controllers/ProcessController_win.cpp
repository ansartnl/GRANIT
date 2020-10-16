#include "ProcessController_win.h"

#include "QWinProcessStarter.h"

#include "Log.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <winuser.h>
#include <tlhelp32.h>

ProcessController::ProcessController()
{
    mIsRunning = false;
    mRestartNeeded = true;
    startProcessTimer = new QTimer(this);
    startProcessTimer->setInterval(3000);
    connect(startProcessTimer, SIGNAL(timeout()), SLOT(runProcess()));
    starter = 0;
}

void ProcessController::setPath(const QString &path, const QString &arguments
                                , const QString &user)
{
    if ( path == mPath )
        return;

    try
    {
        if ( !QFile::exists(path) )
            throw tr(" %1 does not exist on the path: %2").arg(name()).arg(path);

        QFileInfo fi(path);
        if ( !fi.isExecutable() )
            throw tr(" %1 is not executable file: %2").arg(name()).arg(path);

        if (starter)
        {
            disconnect(starter);
            delete starter;
        }
        starter = new QWinProcessStarter(path.toUtf8(), arguments.toUtf8(), this);
        connect(starter, SIGNAL(processStarted()), SLOT(on_started()));
        connect(starter, SIGNAL(processFinished(int,QProcess::ExitStatus))
                , SLOT(on_finished(int,QProcess::ExitStatus)));
        connect(starter, SIGNAL(processError(int)), SLOT(on_error(int)));

        mPath = path;
        mArguments = arguments;
        mUserNeeded = user;
        mIsRunning = false;
        mRestartNeeded = true;
        hToken = 0;
    }
    catch ( const QString &ex )
    {
        qLogWarn(tr("Controller")) << ex;
    }
}

Status ProcessController::status() const
{
    return mIsRunning ? Started : Stopped;
}

bool ProcessController::isRunning() const
{
    return starter ? mIsRunning : false;
}

void ProcessController::restart()
{
    mRestartNeeded = true;
    starter->stop();
}

bool ProcessController::stop()
{
    // Close the process.
    if ( isRunning() )
    {
        mRestartNeeded = false;
        starter->stop();
        return true;
    }
    return false;
}

bool ProcessController::start()
{
    mRestartNeeded = true;
    startProcessTimer->start();
    return ( starter );
}

void ProcessController::runProcess()
{
    if ( !starter )
        return;
    QString name = userName();
    if (name == mUserName)
        return;
    mUserName = name;
    if ((mUserNeeded.isEmpty() && !mUserName.isEmpty()) || (mUserNeeded == mUserName))
    {
        startProcessTimer->stop();
        starter->setUserToken(hToken);
        starter->start();
    }
}

void ProcessController::on_started()
{
    mUserName.clear();
    mIsRunning = true;
    emit statusChanged(type(), name(), Started);
    // emitting previous signal sets this to false (don't know why T_T)
    mRestartNeeded = true;
    qLogInfo(tr("Controller")) << tr(" %1 process has successfully started").arg(name());
}

void ProcessController::on_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    mIsRunning = false;

    if ( QProcess::CrashExit == exitStatus )
        qLogInfo(tr("Controller")) << tr(" %1 process crashed").arg(name());
    else if ( QProcess::NormalExit == exitStatus )
        qLogInfo(tr("Controller")) << tr(" %1 process finished with exit code '%2'")
                                      .arg(name()).arg(exitCode);

    emit statusChanged(type(), name(), Stopped);

    if (mRestartNeeded)
        startProcessTimer->start();
}

void ProcessController::on_error(int error)
{
    QString errorString = tr(" Process '%1' error code: %2. Error message: %3")
            .arg(name())
            .arg(error)
            .arg(getErrorMessage(error));
    qLogWarn(tr("Controller")) << errorString;
}

Controller::Info ProcessController::information() const
{
    Info i;
    i.type = type();
    i.name = name();
    i.status = status();
    i.visualName = visualName();
    return i;
}

QString ProcessController::logFilePath() const
{
    QFileInfo fi(mPath);
    QString logPath = QString("%1%2%3.log").arg(fi.absolutePath())
            .arg(QDir::separator()).arg(fi.baseName());
    return logPath;
}

QString ProcessController::userName()
{
    QString ret;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (_wcsicmp(entry.szExeFile, L"explorer.exe") == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE
                                              , entry.th32ProcessID);

                CloseHandle(hToken);
                hToken = 0;
                // minimum required privilege = 0x000B
                if (OpenProcessToken(hProcess, 0x000B, &hToken))
                {
                    DWORD dwSize;
                    GetTokenInformation(hToken, TokenUser, 0, 0, &dwSize);
                    TOKEN_USER *userToken = (PTOKEN_USER)malloc(dwSize);
                    memset(userToken, 0, dwSize);
                    if (!GetTokenInformation(hToken, TokenUser, (LPVOID) userToken
                                             , dwSize, &dwSize))
                    {
                        free(userToken);
                        qLogWarn(tr("Controller")) << tr(" Could not get user token. ")
                                                   << tr("Error code: ")
                                                   << GetLastError()
                                                   << tr(". Error message: ")
                                                   << getErrorMessage();
                    }
                    dwSize = 256;
                    CHAR name[dwSize];
                    DWORD t = 256;
                    CHAR domainName[t];
                    SID_NAME_USE snu = SidTypeUser;
                    if (LookupAccountSidA(0, userToken->User.Sid, name, &dwSize
                                          , domainName, &t, &snu))
                    {
                        ret = name;
                    } else
                    {
                        qLogWarn(tr("Controller")) << tr(" Could not find account sid. ")
                                                   << tr("Error code: ")
                                                   << GetLastError()
                                                   << tr(". Error message: ")
                                                   << getErrorMessage();
                    }
                    free(userToken);
                }
                CloseHandle(hProcess);
                break;
            }
        }
    }

    CloseHandle(snapshot);

    return ret;
}

LPSTR ProcessController::getErrorMessage(DWORD err)
{
    if (err == (DWORD)-1)
        err = GetLastError();
    LPSTR pBuffer = NULL;

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                   , 0, err, 0, (LPSTR)&pBuffer, 0, 0);

    return pBuffer;
}
