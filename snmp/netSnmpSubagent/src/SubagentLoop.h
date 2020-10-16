#ifndef SUBAGENTLOOP_H
#define SUBAGENTLOOP_H

#include <QtCore/QReadWriteLock>
#include <QtCore/QThread>

class SubagentLoop : public QThread
{
    Q_OBJECT
public:
    explicit SubagentLoop(int timerMs, QObject *parent = 0);

public slots:
    void stop();

protected:
    void run();

private:
    bool needStop() const;

private:
    int mTimerMs;

    bool mStop;
    mutable QReadWriteLock lock;
};

#endif // SUBAGENTLOOP_H
