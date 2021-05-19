#pragma once
#include <chrono>

class Timer
{
public:
	// default construct
	Timer();
	// returns the amount of time that has passed since the last time it was called
	float Mark();
private:
	// tracks the last time when the "Mark" function was called
	std::chrono::steady_clock::time_point last;
};

