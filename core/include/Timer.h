#ifndef __Timer_h__
#define __Timer_h__

#include <ctime>
#include <cmath>

// A simple class of timer
// init when construct
// Three member functions:
// -----double getPassedTimeInSec() : return time from init \n
// -----void stamp() : record a time stamp
// -----double getPassedTimeFromStampInSec() : return time from the stamp
// Note c++ provide only clock per sec. So to avoid double rounding, this class only returns time in seconds
class Timer
{
private:
    clock_t mstartTick;
    // a temp tick record
    clock_t mstamp;

public:
    Timer();
    ~Timer();

    double getPassedTimeInSec();
    void stamp();
    double getPassedTimeFromStampInSec();
};

Timer::Timer()
{
    mstartTick = std::clock();
}

Timer::~Timer()
{
}

double Timer::getPassedTimeInSec()
{
    return (double)(std::clock() - mstartTick) / CLOCKS_PER_SEC;
}

void Timer::stamp()
{
    mstamp = clock();
}

double Timer::getPassedTimeFromStampInSec()
{
    return (double)(std::clock() - mstamp) / CLOCKS_PER_SEC;
}
#endif