#ifndef QWINPROCESSSTARTER_H
#define QWINPROCESSSTARTER_H

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include <windows.h>

class QWinProcessStarter : public QObject
{
    Q_OBJECT
public:
    explicit QWinProcessStarter(const QByteArray &program
                                , const QByteArray &arguments
                                , QObject *parent = 0);
    inline void setUserToken(HANDLE hUserToken)
    {
        hToken = hUserToken;
    }

public slots:
    void start();
    void stop();

signals:
    void processStarted();
    void processFinished(int code, QProcess::ExitStatus status);
    void processError(int code);

protected:
    QByteArray mProgram;
    QByteArray mArguments;
    QByteArray mDir;
    HANDLE hToken;
    HANDLE hProcess;
    DWORD dwThreadId;
};

#endif // Q_OS_WIN

#endif // QWINPROCESSSTARTER_H
