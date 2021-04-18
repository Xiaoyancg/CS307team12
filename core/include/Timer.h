#ifndef __Timer_h__
#define __Timer_h__

#include <chrono>

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
    std::chrono::time_point<std::chrono::steady_clock> mstartTick;
    // a temp tick record
    std::chrono::time_point<std::chrono::steady_clock> mstamp;

public:
    Timer() { mstartTick = std::chrono::steady_clock::now(); }
    ~Timer() {}

    double getPassedTimeInSec()
    {
        std::chrono::duration<double> diff = std::chrono::steady_clock::now() - mstartTick;
        return diff.count();
    }
    void stamp() { mstamp = std::chrono::steady_clock::now(); }
    double getPassedTimeFromStampInSec()
    {
        std::chrono::duration<double> diff = std::chrono::steady_clock::now() - mstamp;
        return diff.count();
    }
};

#endif