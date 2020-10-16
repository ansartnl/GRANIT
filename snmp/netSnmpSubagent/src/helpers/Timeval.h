#ifndef TIMEVAL_H
#define TIMEVAL_H

#include <sys/time.h>

inline bool operator ==(const timeval &l, const timeval &r)
{
    if (l.tv_sec != r.tv_sec)   // seconds
        return false;
    if (l.tv_usec != r.tv_usec) // microseconds
        return false;
    return true;
}

#endif // TIMEVAL_H
