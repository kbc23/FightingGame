#include "stdafx.h"
#include "timer.h"

Timer::Timer()
{

}

Timer::~Timer()
{

}

bool Timer::Start()
{


    return true;
}

void Timer::Update()
{
    CountTimer();
}


void Timer::CountTimer()
{
    if (m_countTimerAdditionOrSubtraction == true) {
        //‰ÁŽZ
        CountAdditionTimer();
    }
    else {
        //Œ¸ŽZ
        CountSubtractionTimer();
    }
}

void Timer::CountAdditionTimer()
{
    ++m_time;
}

void Timer::CountSubtractionTimer()
{
    --m_time;
}