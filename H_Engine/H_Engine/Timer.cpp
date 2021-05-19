#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
    // get the time when an instance is constructed
    // this will be the fisrt reference point
    last = steady_clock::now();
}

float Timer::Mark()
{
    // temporarily store the time Mark was previously calle
    const steady_clock::time_point old = last;
    // update last
    last = steady_clock::now();
    // duration between old and new value of 'last'
    duration<float> dur = last - old;
    // returns a float vales that is the number of seconds
    // usually very very small
    return dur.count();
}
