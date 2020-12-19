#ifndef MARIANI_SILVER_STOPWATCH_H
#define MARIANI_SILVER_STOPWATCH_H

#include <chrono>
#include <cassert>
#include <string>
#include <iostream>

class Stopwatch {
public:
	typedef std::chrono::high_resolution_clock::time_point timePoint_t;
	typedef std::chrono::nanoseconds nanoseconds;

	void start() {
		startTime = now();
	}

	std::chrono::nanoseconds stop() {
		timePoint_t end_time = now();
		duration = end_time - startTime;
		return duration;
	}

private:
	static timePoint_t now() {
		return std::chrono::high_resolution_clock::now();
	}

	timePoint_t startTime;
	nanoseconds duration = nanoseconds::zero();
};

#endif //MARIANI_SILVER_STOPWATCH_H
