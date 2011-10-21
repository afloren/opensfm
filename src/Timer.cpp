#include "Timer.h"

#include <Windows.h>

Timer::Timer()
{
	LARGE_INTEGER lpFrequency;
	QueryPerformanceFrequency(&lpFrequency);
	frequencyCount = lpFrequency.QuadPart;
	elapsedCount = 0;
	totalCount = 0;
	running = false;
}

Timer::~Timer()
{

}

void Timer::start()
{
	if(!running)//fail silently
	{
		LARGE_INTEGER lpPerformanceCount;
		QueryPerformanceCounter(&lpPerformanceCount);
		startCount = lpPerformanceCount.QuadPart;
		running = true;
	}
}

void Timer::stop()
{
	if(running)//fail silently
	{
		LARGE_INTEGER lpPerformanceCount;
		QueryPerformanceCounter(&lpPerformanceCount);
		elapsedCount = (lpPerformanceCount.QuadPart - startCount);
		totalCount += elapsedCount;
		running = false;
	}
}

bool Timer::isRunning()
{
	return running;
}

double Timer::getElapsed()
{
	bool wasRunning = isRunning();

	if(wasRunning)
		stop();

	double elapsed = elapsedCount / (double)frequencyCount;

	if(wasRunning)
		start();

	return elapsed;
}

double Timer::getTotal()
{
	bool wasRunning = isRunning();

	if(wasRunning)
		stop();

	double total = totalCount / (double)frequencyCount;

	if(wasRunning)
		start();

	return total;
}