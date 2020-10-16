#include "RestartInterval.h"

RestartInterval::RestartInterval(uint minInterval, uint maxInterval, uint step, QObject *parent)
    : QObject(parent)
{
    mInterval.append(minInterval);
    for (uint i = minInterval; i < maxInterval; i = i + step)
    {
        mInterval.append(i);
    }
    mInterval.append(maxInterval);

    reset();
}

uint RestartInterval::nextInterval()
{
    uint interval = mInterval[mCurrentIndex];
    if (mCurrentIndex != mInterval.count() - 1)
    {
        mCurrentIndex++;
    }

    return interval;
}

void RestartInterval::reset()
{
    mCurrentIndex = 0;
}
