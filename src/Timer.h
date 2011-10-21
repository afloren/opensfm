#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();

	double getElapsed();
	double getTotal();
	bool isRunning();

	bool running;
	long long frequencyCount;
	long long startCount;
	long long elapsedCount;
	long long totalCount;
};