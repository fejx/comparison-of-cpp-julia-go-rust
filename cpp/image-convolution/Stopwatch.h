#ifndef IMAGE_CONVOLUTION_STOPWATCH_H
#define IMAGE_CONVOLUTION_STOPWATCH_H

#include <chrono>
#include <cassert>
#include <string>
#include <iostream>

class Stopwatch {
public:
	typedef std::chrono::high_resolution_clock::time_point time_point;
	typedef std::chrono::nanoseconds nanoseconds;

	void start() {
		start_time = now();
	}

	std::chrono::nanoseconds stop() {
		time_point end_time = now();
		duration = end_time - start_time;
		return duration;
	}

private:
	static time_point now() {
		return std::chrono::high_resolution_clock::now();
	}

	time_point start_time;
	nanoseconds duration = nanoseconds::zero();
};

#endif //IMAGE_CONVOLUTION_STOPWATCH_H
