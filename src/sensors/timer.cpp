#include "timer.hpp"

#include "rtc.hpp"

void Timer::start()
{
    isActive = true;
    beginT = Rtc::cmillis;
}

void Timer::stop()
{
    isActive = false;
}

bool Timer::alarm(unsigned long interval)
{
    return (isActive && Rtc::cmillis - beginT >= interval);
}
