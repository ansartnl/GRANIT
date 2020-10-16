#ifndef RESTARTINTERVAL_H
#define RESTARTINTERVAL_H

#include <QObject>

class RestartInterval : public QObject
{
    Q_OBJECT
public:
    RestartInterval(uint minInterval, uint maxInterval, uint step, QObject *parent = 0);
    uint nextInterval();

public slots:
    void reset();

private:
    QList<uint> mInterval;
    int mCurrentIndex;
};

#endif // RESTARTINTERVAL_H
